#ifndef DISCRETISATION_H
#define DISCRETISATION_H

#include <QVector>
#include <QDebug>

#include "point.h"

typedef union _function_i_params //Fonction à i paramètres (exemple segment 1 paramètre, courbes 2 paramètres)
{
    Point (*f1)(float s, void *obj);
    Point (*f2)(float s, float t, void *obj);

    ~_function_i_params() {} // needs to know which member is active, only possible in union-like class
}f_param;

typedef enum _display_mode
{
    POINTS,
    QUADS,
    TRIANGLES
}disp_mode;

class Discretisation
{
public:
    Discretisation(Point (*f)(float s, void *obj), float step);
    Discretisation(Point (*f)(float s, float t, void *obj), float step);
    Discretisation(Point (*f)(float s, void *obj), float step, disp_mode m);
    Discretisation(Point (*f)(float s, float t, void *obj), float step, disp_mode m);

    void paramCompute(void *obj);
    void paramsCompute2(void * obj);

    void paramToVBO(QVector<float> colors);
    QVector<Point> * paramPoints;
    QVector<float> VBO;

    float getStep() const;
    void setStep(float value);

    void setMODE(disp_mode m);
    disp_mode getMODE();
    void nextMODE();

    void clearBuffers();

private:
    f_param f;
    disp_mode MODE = TRIANGLES;
    float step;
};

#endif // DISCRETISATION_H
