//R. Raffin, M1 Informatique, "Surfaces 3D"
//tiré de CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "myopenglwidget.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>

#include <iostream>

#include "segment.h"

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

void myOpenGLWidget::doProjection()
{
	//m_mod.setToIdentity();
	//modelMatrix.ortho( -aratio, aratio, -1.0f, 1.0f, -1.0f, 1.0f );
}


Point discretizeSeg(float s, void * obj)
{
    QVector<Point> p;
    p.append(((Segment *) obj)->getStart());
    p.append(((Segment *) obj)->getEnd());

    float xa = p.data()[0].getX(), ya = p.data()[0].getY(), za = p.data()[0].getZ();
    float xb = p.data()[1].getX(), yb = p.data()[1].getY(), zb = p.data()[1].getZ();

    Point * n_paramPoint = new Point();

    n_paramPoint->setX(((1-s)*xa) + s*xb);
    n_paramPoint->setY(((1-s)*ya) + s*yb);
    n_paramPoint->setZ(((1-s)*za) + s*zb);

    return *n_paramPoint;
}

QVector<GLfloat> vertData;
void myOpenGLWidget::makeGLObjects()
{
#if 0 //TEST SEGMENT
	//1 Nos objets géométriques
	Point A, B;
	float * coord = new float[3];

	coord[0] = 0.0f;
	coord[1] = 0.0f;
	coord[2] = 0.0f;

	A.set (coord);

	coord[0] = 1.0f;
	coord[1] = 0.0f;
	coord[2] = 0.0f;

	B.set(coord);

	Segment S;
	S.setStart(A);
	S.setEnd(B);

	delete [] coord;

	//qDebug() << "segment length " << S.length ();

	//2 Traduction en tableaux de floats
    GLfloat * vertices = new GLfloat[9]; //2 sommets
    GLfloat * colors = new GLfloat[9]; //1 couleur (RBG) par sommet

	Point begin, end;
	float * values = new float[3];

	begin = S.getStart ();
	begin.get(values);
	for (unsigned i=0; i<3; ++i)
		vertices[i] = values[i];

	end = S.getEnd ();
	end.get(values);
	for (unsigned i=0; i<3; ++i)
		vertices[3+i] = values[i];

	delete[] values;

	//couleur0 = rouge
	colors[0] = 1.0;
	colors[1] = 0.0;
	colors[2] = 0.0;

    //violet
    colors[3] = 1.0;
    colors[4] = 0.0;
    colors[5] = 1.0;

	//bleu
    colors[6] = 0.0;
    colors[7] = 0.0;
    colors[8] = 1.0;



    Discretisation discreteSegment(discretizeSeg, 0.5f);
    discreteSegment.paramCompute((void*) (&S));

	//3 spécialisation OpenGL
	QVector<GLfloat> vertData;
    for (int i = 0; i < 3; ++i) { //2 sommets
		// coordonnées sommets
            vertData.append(discreteSegment.paramPoints->data()[i].getX());
            vertData.append(discreteSegment.paramPoints->data()[i].getY());
            vertData.append(discreteSegment.paramPoints->data()[i].getZ());
		// couleurs sommets
		for (int j = 0; j < 3; j++) //1 RGB par sommet
			vertData.append(colors[i*3+j]);

	}

    QVector<GLfloat> rgb;

    for(int i = 0 ; i < 9 ; i++)
        rgb.append(colors[i]);

    discreteSegment.paramToVBO(rgb);
	//destruction des éléments de la phase 2
	delete [] vertices;
	delete [] colors;

	m_vbo.create();
	m_vbo.bind();

	//qDebug() << "vertData " << vertData.count () << " " << vertData.data ();
    m_vbo.allocate(discreteSegment.VBO.constData(), discreteSegment.VBO.count() * sizeof(GLfloat));
    #endif

    //TEST COURBES

    //1 Nos objets géométriques
    Point A;
    QVector<Point> points;
    float test,test1, test2, test3;
    srand(time(nullptr));
    for (int i = 0; i < 4; ++i) {
        test1 = (float)(rand()%(100+100 + 1) -100) / 100;
        test2 = (float)(rand()%(100+100+ 1) -100)/ 100;
        test3 = (float)(rand()%(100+100 + 1) -100)/ 100;;
        A.setX(test1);
        A.setY(test2);
        A.setZ(test3);
        vertData.push_back(A.getX());
        vertData.push_back(A.getY());
        vertData.push_back(A.getZ());
        vertData.push_back(1);
        vertData.push_back(1);
        vertData.push_back(1);
        points.push_back(A);
    }

    courbe c;
    QVector<Point> vertices = c.surfBez(points,0.05,sqrt(ctrlPts));

    //vertices += points;
    //3 spécialisation OpenGL
    for (int i = 0; i < vertices.size(); ++i) { //2 sommets
        // coordonnées sommets
        vertData.push_back(vertices[i].getX());
        vertData.push_back(vertices[i].getY());
        vertData.push_back(vertices[i].getZ());
        for (int j = 0; j < 3; j++){ //1 RGB par sommet
            test = (float)( rand() % 100) / 100;
            vertData.push_back(test);
        }
        //qDebug() << "x : "<<vertices[i].getX()<<" y : "<<vertices[i].getY()<<" z : "<<vertices[i].getZ()<<endl;
    }

    m_vbo.create();
    m_vbo.bind();

    //qDebug() << "vertData " << vertData.count () << " " << vertData.data ();
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));

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

	glPointSize (5.0f);
    glDrawArrays(GL_POINTS, 0, ctrlPts);
    glDrawArrays(GL_POINTS, ctrlPts*6,vertData.size()-(ctrlPts*6));

	m_program->disableAttributeArray("posAttr");
	m_program->disableAttributeArray("colAttr");

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
	}
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

void myOpenGLWidget::onTimeout()
{
	qDebug() << __FUNCTION__ ;

	update();
}




