//Point.cpp
#include "Point.h"

Point::Point()
{
   mX = 0.0;
   mY = 0.0;
   mZ = 0.0;
}
Point::Point(double x, double y, double z)
{
   mX = x;
   mY = y;
   mZ = z;
}

void Point::Move(Point p)
{
    mX = p.x();
    mY = p.y();
    mZ = p.z();
}

void Point::Move(double x, double y, double z)
{
   mX = x;
   mY = y;
   mZ = z;
}
