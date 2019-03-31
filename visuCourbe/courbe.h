#ifndef COURBE_H
#define COURBE_H

#include <QVector>
#include "point.h"
class courbe
{
public:
    courbe();
    QVector <Point> courbeBez(QVector<Point> points, float t);
    float bern(int i, int n, float t);
    int fact (int i);
    QVector<Point> surfBez(QVector<Point> points, float t, int nbcol);
};

#endif // COURBE_H