#ifndef SURFACE_H
#define SURFACE_H

#include <QVector>

#include "point.h"
#include "point.h"
#include "qdebug.h"

class surface
{
public:
    surface();
    static QVector<Point> CreateControlPoint(int nbCol);
};

#endif // SURFACE_H
