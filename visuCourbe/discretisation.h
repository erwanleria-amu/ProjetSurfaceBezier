#ifndef DISCRETISATION_H
#define DISCRETISATION_H

#include <QVector>
#include <QDebug>

#include "point.h"


class Discretisation
{
public:
    Discretisation(Point (*f)(float s, void *obj), float step);
    void paramCompute(void *obj);
    void paramToVBO(QVector<float> colors);
    QVector<Point> * paramPoints;
    QVector<float> VBO;

private:
    Point (*f)(float s, void *obj);
    float step;
};

#endif // DISCRETISATION_H