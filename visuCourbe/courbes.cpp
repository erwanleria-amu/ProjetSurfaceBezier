#include "courbes.h"

Courbes::Courbes()
{

}

int Courbes::fact(int i)
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

float Courbes::bern(int i, int n, float t)
{
    float bern = (fact(n)/(fact(i)*fact(n-i)))*pow(t,i)*pow((1-t),n-i);
    return bern;
}




