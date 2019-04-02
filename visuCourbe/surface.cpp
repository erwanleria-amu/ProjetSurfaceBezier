#include "surface.h"
#include "courbe.h"
#include "point.h"
#include "qdebug.h"

surface::surface()
{

}

QVector<Point> surface::surfBez(QVector<Point> points, float t, int nbcol)
{
    courbe c;
    QVector<Point> vecPts;
    Point pt ;
    pt.setX(0);
    pt.setY(0);
    pt.setZ(0);
    for (float var = 0; var <= 1+t; var+=t) {
        for (float var2 = 0; var2 <= 1+t; var2+=t) {
            Point tmpoint = pt;
            for (int i = 0; i < nbcol; ++i) {
                for (int j = 0; j < nbcol; ++j) {
                    Point mul = points[i*nbcol+j];
                    int degre = nbcol - 1;
                    tmpoint = tmpoint + (mul * c.bern(i,degre,var) * c.bern(j,degre,var2)) ;
                }
            }
            vecPts.push_back(tmpoint);
        }
    }
    //    for (int var = 0; var < points.size(); ++var) {
    //        qDebug() << points.size() << endl;
    //    }
    return  vecPts;
}

QVector<Point> surface::CreateControlPoint(int nbCol){
    QVector<Point> points;
    Point p;
    for (float i = -1.f; i < 1.f; i += (2.f/nbCol)) {
        for (float j = -1.f; j < 1.f; j += (2.f/nbCol)) {
            p.setX(i);
            p.setY((float)( rand() % 50) / 100);
            p.setZ(j);
            points.push_back(p);
            qDebug() << i << " " << j << endl;
        }
    }
    return points;
}
