//R. Raffin, M1 Informatique, "Surfaces 3D"
//tiré de CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "myopenglwidget.h"

static const QString vertexShaderFile   = ":/basic.vsh";
static const QString fragmentShaderFile = ":/basic.fsh";


myOpenGLWidget::myOpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init myOpenGLWidget" ;

    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
    // cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
    setFormat(sf);

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

myOpenGLWidget::~myOpenGLWidget()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}

void myOpenGLWidget::initializeGL()
{
    qDebug() << __FUNCTION__ ;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    pointsCtrl = surface::CreateControlPoint(nbCol);
    makeGLObjects();

    //shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);

    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }
}

Point discretizeSurfBez(float s, float t, void * obj)
{
    QVector<Point> vecPts;
    Point pt ;
    pt.setX(0);
    pt.setY(0);
    pt.setZ(0);
    Point tmpoint = pt;
    int nbCol = ((SurfacesBezier *) obj)->n;
    for (int i = 0; i < nbCol; ++i) {
        for (int j = 0; j < nbCol ; ++j) {
            Point mul = ((SurfacesBezier *) obj)->ptsCtrl.data()[i* nbCol +j];
            tmpoint = tmpoint + (mul * Bernstein::bern(i, nbCol-1 , s) * Bernstein::bern(j, nbCol-1, t)) ;
        }
    }
    return  tmpoint;
}

void myOpenGLWidget::makeGLObjects()
{

    //Si le nombre de points de contrôle (initialisés dans initializeGL) change alors on les récrée,
    //sinon le maillage reste inchangé
    if(sizeChanged){
        sizeChanged = false;
        pointsCtrl = surface::CreateControlPoint(nbCol);
    }


    //Discrétisation de la surface à partir des points de controle
    //précédemment créés

    SurfacesBezier sb(&pointsCtrl);
    qDebug() << "sb points" << endl;

    Discretisation *discreteSurfBez = nullptr;

    if(modeChanged)
    {
        if(stepChanged)
        {
            discreteSurfBez = new Discretisation(discretizeSurfBez, step); //Le deuxième argument est le pas des paramètres
            discreteSurfBez->setMODE(curDiscreteObj->getMODE());
            discreteSurfBez->nextMODE(); //Le deuxième argument est le pas des paramètres
            delete curDiscreteObj;
            this->curDiscreteObj = discreteSurfBez; //On indique la structure discrète utilisée pour le programme

        }

        else {
            curDiscreteObj->nextMODE(); //Le deuxième argument est le pas des paramètres
            curDiscreteObj->clearBuffers();
        }

        setModeChanged(false);
    }

    else
    {

        if(curDiscreteObj == nullptr || stepChanged)
        {
            discreteSurfBez = new Discretisation(discretizeSurfBez, step); //Le deuxième argument est le pas des paramètres
            this->curDiscreteObj = discreteSurfBez; //On indique la structure discrète utilisée pour le programme
        }

        else
        {
            curDiscreteObj->setStep(step);
            curDiscreteObj->clearBuffers(); //On décharge les points de discrétisations crées ainsi que les points pour le vbo
        }

        qDebug() << "include discretefunc ok" << endl;
    }

    //MODE D'AFFICHAGE

    curDiscreteObj->paramsCompute2((void *) &sb);
    qDebug() << "compute params ok" << endl;

    QVector<float> colors;
    colors.push_back(1); colors.push_back(0); colors.push_back(0);
    qDebug() << "colors OK" << endl;

    curDiscreteObj->paramToVBO(colors);

    //Création et ajout au VBO du point qui se déplace sur le carreau
    deplacement = setDeplacementPoint(u, v, &sb);

    vertData.push_back(deplacement.getX());
    vertData.push_back(deplacement.getY());
    vertData.push_back(deplacement.getZ());

    vertData.push_back(0);
    vertData.push_back(0);
    vertData.push_back(1);

    //Ajout des points de controles au VBO
    for (int var = 0; var < pointsCtrl.size(); ++var) {
        vertData.push_back(pointsCtrl[var].getX());
        vertData.push_back(pointsCtrl[var].getY());
        vertData.push_back(pointsCtrl[var].getZ());
        for (int var2 = 0; var2 < 3; ++var2) {
            vertData.push_back(1);
        }
    }

    //Ajout des points de discrétisations au VBO
    vertData.append(curDiscreteObj->VBO);

    m_vbo.create();
    m_vbo.bind();

    m_vbo.allocate(vertData.data(), vertData.count() * sizeof(GLfloat));
}


void myOpenGLWidget::tearGLObjects()
{
    m_vbo.destroy();
}


void myOpenGLWidget::resizeGL(int w, int h)
{
    qDebug() << __FUNCTION__ << w << h;

    //C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
    //doProjection();
}

void myOpenGLWidget::paintGL()
{
    qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind(); // active le shader program


    /// Ajout RR pour gérer les 3 matrices utiles
    /// à mettre dans doProjection() pour clarifier
    /// -----------------------------------------
    m_modelView.setToIdentity();
    m_modelView.lookAt(QVector3D(0.0f, 0.0f, 3.0f),    // Camera Position
                       QVector3D(0.0f, 0.0f, 0.0f),    // Point camera looks towards
                       QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

    m_projection.setToIdentity ();
    m_projection.perspective(70.0, width() / height(), 0.1, 100.0); //ou m_ratio

    //m_model.translate(0, 0, -3.0);

    // Rotation de la scène pour l'animation
    m_model.rotate(m_angle, 0, 1, 0);

    QMatrix4x4 m = m_projection * m_modelView * m_model;
    ///----------------------------

    m_program->setUniformValue("matrix", m);

    m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    m_program->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    m_program->enableAttributeArray("posAttr");
    m_program->enableAttributeArray("colAttr");

    //pouvoir choisir montrer cacher points de controles
    glPointSize (5.0f);
    glDrawArrays(GL_POINTS, 0, nbCol*nbCol+1);

    if(curDiscreteObj->getMODE() == QUADS || curDiscreteObj->getMODE() == TRIANGLES)
    {
        glPointSize (2.0f);
        glDrawArrays(GL_LINES, nbCol*nbCol+1, curDiscreteObj->paramPoints->length());
    }

    else
    {
        glPointSize (2.0f);
        glDrawArrays(GL_POINTS, nbCol*nbCol+1, curDiscreteObj->paramPoints->length());
    }

    m_program->disableAttributeArray("posAttr");
    m_program->disableAttributeArray("colAttr");

    vertData.clear();
    m_program->release();
}

void myOpenGLWidget::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
    case Qt::Key_Z :
        m_angle += 1;
        if (m_angle >= 360) m_angle -= 360;
        update();
        break;
    case Qt::Key_A :
        if (m_timer->isActive())
            m_timer->stop();
        else m_timer->start();
        break;
    case Qt::Key_R :
        break;

    case Qt::Key_Space :
        setModeChanged(true);
        makeGLObjects();
        update();
        break;
    }
}

Point myOpenGLWidget::setDeplacementPoint(float u, float v, void* obj){

    surface s;
    Point p = discretizeSurfBez(u, v, obj); //s.bernPoint(*curDiscreteObj->paramPoints, u, v, nbCol);
    return p;
}

void myOpenGLWidget::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}

void myOpenGLWidget::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

bool myOpenGLWidget::getModeChanged() const
{
    return modeChanged;
}

void myOpenGLWidget::setModeChanged(bool value)
{
    modeChanged = value;
}

void myOpenGLWidget::onTimeout()
{
    qDebug() << __FUNCTION__ ;

    update();
}

void myOpenGLWidget::setNbCol(int i){
    nbCol = i;
}

void myOpenGLWidget::setSizeChanged(){
    sizeChanged = true;
}

void myOpenGLWidget::setStep(double d)
{
    step = d;
}

int myOpenGLWidget::getNbCol(){
    return nbCol;
}

/*
 * cette fonction permet de mettre a jour a la fois le VBO et l'affichage a l'ecran
 * "m_angle" est remis a 0 pour eviter des rotation du carreau si on change le nombre
 * de points de controle ou si l'on deplace le point sur le carreau
 */
void myOpenGLWidget::upd(){
    makeGLObjects();
    m_angle = 0;
    update();
}

void myOpenGLWidget::setU(float _u)
{
    u = _u;
}

void myOpenGLWidget::setV(float _v)
{
    v = _v;
}
