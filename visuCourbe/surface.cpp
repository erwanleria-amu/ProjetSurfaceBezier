#include "surface.h"
#include "courbe.h"
#include "point.h"
#include "qdebug.h"

surface::surface()
{

}

QVector<Point> surface::surfBez(QVector<Point> points, float t, int nbcol)
{

    QVector<Point> vecPts;

    for (float u = 0; u <= 1+t; u+=t) {
        for (float v = 0; v <= 1+t; v+=t) {
            Point tmpoint = bernPoint(points,u,v,nbcol);
            vecPts.push_back(tmpoint);
        }
    }
    //    for (int var = 0; var < points.size(); ++var) {
    //        qDebug() << points.size() << endl;
    //    }
    return  vecPts;
}

Point surface::bernPoint(QVector<Point> points, float u, float v, int nbcol){
    Point pt ;
    courbe c;
    pt.setX(0);
    pt.setY(0);
    pt.setZ(0);
    Point tmpoint = pt;
    for (int i = 0; i < nbcol; ++i) {
        for (int j = 0; j < nbcol; ++j) {
            Point mul = points[i*nbcol+j];
            int degre = nbcol - 1;
            tmpoint = tmpoint + (mul * c.bern(i,degre,u) * c.bern(j,degre,v)) ;
        }
    }

    return tmpoint;
}

QVector<Point> surface::CreateControlPoint(int nbCol){
    QVector<Point> points;
    Point p;
    for (float i = -1.f; i < 1.f; i += (2.f/nbCol)) {
        for (float j = -1.f; j < 1.f; j += (2.f/nbCol)) {
            p.setX(i);
            p.setY((float)( rand() % 100) / 100);
            p.setZ(j);
            points.push_back(p);
            //qDebug() << i << " " << j << endl;
        }
    }
    return points;
}
