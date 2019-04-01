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



