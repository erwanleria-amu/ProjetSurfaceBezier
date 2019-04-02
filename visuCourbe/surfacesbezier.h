#ifndef SURFACESBEZIER_H
#define SURFACESBEZIER_H

#include <QVector>
#include "point.h"
#include "courbes.h"

class SurfacesBezier
{
public:
    SurfacesBezier();
    SurfacesBezier(QVector<Point> *ptsCtrl);
    int n; //n*n = nb points de controle
    QVector<Point> ptsCtrl; //n*n points de controle
};

#endif // SURFACESBEZIER_H
