// Point.h
#ifndef POINT_H
#define POINT_H 

#include<ostream>

class Point {
  private:
    double mX;
    double mY;
    double mZ;

  public:
    Point();
    Point(double, double, double);

    double x() const { return mX; }
    double y() const { return mY; }
    double z() const { return mZ; }
    
    void Move(Point);
    void Move(double, double, double);
};

//Define operator '<<' for type Point
inline std::ostream& operator<<(std::ostream& out, const Point& p)
{
  out << '(' << p.x() << ',' << p.y() << "," << p.z() <<')';
  return out;
}
#endif
