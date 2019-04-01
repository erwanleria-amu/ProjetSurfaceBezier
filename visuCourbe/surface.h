#ifndef SURFACE_H
#define SURFACE_H
#include <QVector>
#include "point.h"

class surface
{
public:
    surface();
    QVector<Point> surfBez(QVector<Point> points, float t, int nbcol);
    QVector<Point> CreateControlPoint(int nbCol);
};

#endif // SURFACE_H
