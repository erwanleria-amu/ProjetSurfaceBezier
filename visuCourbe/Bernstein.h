#ifndef BERNSTEIN_H
#define BERNSTEIN_H

#include <QVector>
#include "point.h"
#include "math.h"
#include "qdebug.h"

class Bernstein
{
public:
    Bernstein();
    QVector <Point> courbeBez(QVector<Point> points, float t);
    static float bern(int i, int n, float t);
    static int fact (int i);
    QVector<Point> surfBez(QVector<Point> points, float t, int nbcol);

};

#endif // COURBES_H
