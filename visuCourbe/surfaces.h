#ifndef SURFACES_H
#define SURFACES_H


class Surfaces
{
public:
    Surfaces();

    QVector<Point> surfBez(QVector<Point> points, float t, int nbcol);

};

#endif // SURFACES_H
