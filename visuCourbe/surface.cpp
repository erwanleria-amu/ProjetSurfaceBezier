#include "surface.h"
#include "point.h"
#include "qdebug.h"

surface::surface()
{

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
        }
    }
    return points;
}
