#include "discretisation.h"

Discretisation::Discretisation(Point (*f)(float s, void *obj), float step)
{
    this->f = f;
    this->step = step;

    this->paramPoints = new QVector<Point>();
}

void Discretisation::paramCompute(void * obj)
{
    for(float s = 0 ; s <= 1 ; s += step)
    {
        paramPoints->append(f(s, obj));
    }
}

void Discretisation::paramToVBO(QVector<float> colors)
{
    int n = paramPoints->count();

        for (int i = 0; i < n; ++i) { //n sommets
            // coordonnées sommets
                VBO.append(paramPoints->data()[i].getX());
                VBO.append(paramPoints->data()[i].getY());
                VBO.append(paramPoints->data()[i].getZ());
                qDebug() << "points" << endl;

            // couleurs sommets
                if(colors.count()/3 < n) //Si il n'y a qu'un RGB
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
