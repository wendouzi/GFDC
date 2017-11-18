
#include <stdio.h>
#include "geoshape.h"

Point::Point(float a, float b)
{
    x = a;
    y = b;
}

float Point::getX()
{
    return x;
}

float Point::getY()
{
    return y;
}

void Point::print()
{
    printf("Point(%f,%f)\n", x,y);
}

Line::Line(Point & _p1, Point & _p2):p1(_p1),p2(_p2)
{
}

void Line::print()
{
    printf("Line is made up of Point(%f,%f) and Point(%f,%f)\n", p1.getX(), p1.getY(), p2.getX(), p2.getY());
}
/*
float dis2point(Line & l,Point & p)
{
    float x1 = l.p1.getX();
    float y1 = l.p1.getY();
    float x2 = l.p2.getX();
    float y2 = l.p2.getY();
    float x = p.getX();
    float y = p.getY();
    return ((x-x1)*(y2-y1)-(x2-x1)*(y-y1))/sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}*/

float dis2point(float li[], float pi[])
{
    float x1 = li[0];
    float y1 = li[1];
    float x2 = li[2];
    float y2 = li[3];
    float x = pi[0];
    float y = pi[1];
    return ((x-x1)*(y2-y1)-(x2-x1)*(y-y1))/sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

float point2point(float * point1, float * point2)
{
    return sqrt(pow(point1[0]-point2[0],2) + pow(point1[1]-point2[1],2));
}
/*
void Line::test()
{
    Point p1(2,4);
    Point p2(7,9);
    Point p(0,0);
    Line L(p1, p2);
    p.print();
    L.print();
    printf("distance:%f\n",dis2point(L,p));
}

*/
