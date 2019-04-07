#include "discretisation.h"
//Cette classe permet de générer les points de discrétisation du carreau de bezier

Discretisation::Discretisation(Point (*f)(float s, void *obj), float step)
{
    this->f.f1 = f;
    this->step = step;

    this->paramPoints = new QVector<Point>;
}

Discretisation::Discretisation(Point (*f)(float s, float t, void *obj), float step)
{
    this->f.f2 = f;
    this->step = step;

    this->paramPoints = new QVector<Point>;
}

void Discretisation::paramCompute(void * obj)
{
    for(float s = 0 ; s <= 1 ; s += step)
    {
        paramPoints->append(f.f1(s, obj));
    }
}

/*
 * cette fonction permet de générer les points de discrétisation du
 * carreau de bezier et les stock dans le vecteur de points
 * "paramPoints"
 * Selon le mode d'affichage on ne va pas push les meme points
 * (plus de points pour la représentation par triangles par exemple)
 */
void Discretisation::paramsCompute2(void * obj)
{
    float last = 1;
    if(MODE == POINTS)
        last += step;
    for(float s = 0 ; s <= last; s += step)
    {
        for(float t = 0 ; t <= last; t += step)
        {
            paramPoints->append(f.f2(s, t, obj));
            if(MODE == QUADS || MODE == TRIANGLES)
            {
                paramPoints->append(f.f2(s+step, t, obj));

                paramPoints->append(f.f2(s+step, t, obj));
                paramPoints->append(f.f2(s+step, t+step, obj));

                paramPoints->append(f.f2(s+step, t+step, obj));
                paramPoints->append(f.f2(s, t+step, obj));

                paramPoints->append(f.f2(s, t+step, obj));
                paramPoints->append(f.f2(s, t, obj));

                if(MODE == TRIANGLES)
                {
                    paramPoints->append(f.f2(s, t, obj));
                    paramPoints->append(f.f2(s+step, t+step, obj));
                }
            }
        }
    }
}

/*
 * cette fonction permet de mettre les points presents dans "paramPoint" dans un
 * vecteur de float qui sera utiliser pour le VBO
 */
void Discretisation::paramToVBO(QVector<float> colors)
{
    int n = paramPoints->count();
    for (int i = 0; i < n; ++i) { //n sommets
        // coordonnées sommets
        VBO.append(paramPoints->data()[i].getX());
        VBO.append(paramPoints->data()[i].getY());
        VBO.append(paramPoints->data()[i].getZ());

        // couleurs sommets
        if(colors.count() == 3) //Si il n'y a qu'un RGB
        {
            for (int j = 0; j < 3; j++) //Meme RGB pour chaque sommet
                VBO.append(colors[j]);
        }

        else //Sinon on admet qu'il y a autant de RGB qu'il y a de sommets
        {
            for (int j = 0; j < 3; j++) //Différents RGB pour chaque sommet
                VBO.append(colors[i*3+j]);
        }
    }
}

float Discretisation::getStep() const
{
    return step;
}

void Discretisation::setStep(float value)
{
    step = value;
}

void Discretisation::setMODE(disp_mode m)
{
    MODE = m;
}

disp_mode Discretisation::getMODE()
{
    return MODE;
}

void Discretisation::nextMODE()
{
    switch (MODE) {

    case POINTS :
        MODE = QUADS;
        break;

    case QUADS :
        MODE = TRIANGLES;
        break;

    case TRIANGLES:
        MODE = POINTS;
        break;
    }
}

void Discretisation::clearBuffers()
{
    if(!paramPoints->empty())
        paramPoints->clear();

    if(!VBO.empty())
        VBO.clear();
}
