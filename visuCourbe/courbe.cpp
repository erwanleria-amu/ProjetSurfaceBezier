#include "courbe.h"
#include "math.h"
#include "qdebug.h"


courbe::courbe()
{

}

QVector<Point> courbe::courbeBez(QVector<Point> points, float t)
{
    QVector<Point> vecPts;
    for (float var = 0; var <= 1; var+=t) {
        Point tmpoint ;
        for (int i = 0; i < points.size(); ++i) {
            Point mul = points[i];
            tmpoint =tmpoint + (mul * bern(i,points.size()-1,var)) ;
        }
        vecPts.push_back(tmpoint);
    }
    //    for (int var = 0; var < points.size(); ++var) {
    //        qDebug() << points.size() << endl;
    //    }
    return  vecPts;
}

QVector<Point> courbe::surfBez(QVector<Point> points, float t, int nbcol)
{
    QVector<Point> vecPts;
    Point pt ;
    pt.setX(0);
    pt.setY(0);
    pt.setZ(0);
    for (float var = 0; var <= 1; var+=t) {
        for (float var2 = 0; var2 <= 1; var2+=t) {
            Point tmpoint = pt;
            for (int i = 0; i < nbcol; ++i) {
                for (int j = 0; j < points.size()/nbcol; ++j) {
                    Point mul = points[i*nbcol+j];
                    tmpoint = tmpoint + (mul * bern(i,nbcol-1,var) * bern(j,(points.size()/nbcol)-1,var2)) ;
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

float courbe::bern(int i, int n, float t)
{
    float bern = (fact(n)/(fact(i)*fact(n-i)))*pow(t,i)*pow((1-t),n-i);
    return bern;
}

int courbe::fact(int i)
{
    int tmp = 1;
    if (i == 0){
        return tmp;
    }
    for (int var = 1; var <= i; ++var) {
        tmp *= var;
    }
    return tmp;
}



