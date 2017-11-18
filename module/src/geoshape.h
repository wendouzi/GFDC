
#include <iostream>
#include <math.h>

#ifndef GEOSHAPE_H
#define GEOSHAPE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif
class Point
{
    private:
        float x,y;
    public:
        Point(float a, float b);
        float getX();
        float getY();
        void print();
    bool operator==(Point & p) const {
        return (x==p.getX())&&(y==p.getY());
    }
};

class Line
{
    private:
        Point p1, p2;
    public:
        Line(Point &, Point &);
        void print();
//        friend float dis2point(Line & l, Point & p);
//        void test();
};

float dis2point(float li[],float pi[]);

float point2point(float * point1, float * point2);


#endif /* GEOSHAPE_H */
/*EOF*/
