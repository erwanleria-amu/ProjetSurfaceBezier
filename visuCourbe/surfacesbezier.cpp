#include "surfacesbezier.h"

SurfacesBezier::SurfacesBezier()
{

}

SurfacesBezier::SurfacesBezier(QVector<Point> *ptsCtrl)
{
    this->n = sqrt(ptsCtrl->count());

    for(int i = 0; i < n*n ; i++)
    {
        this->ptsCtrl.append(ptsCtrl->data()[i]);
    }
}
