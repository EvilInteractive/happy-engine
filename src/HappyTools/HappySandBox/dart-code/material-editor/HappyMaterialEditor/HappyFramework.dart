#library("HappyFramework");

#import('dart:math');

class vec2
{
  double x, y;
  
  vec2(this.x, this.y) {}

  vec2 operator*(double other)
  {
    return new vec2(x + other, y + other);
  }
  vec2 operator+(vec2 other)
  {
    return new vec2(x + other.x, y + other.y);
  }
  vec2 operator-(vec2 other)
  {
    return new vec2(x - other.x, y - other.y);
  }
  
  double dot(vec2 other)
  {
    return x*other.x + y*other.y;
  }
  double lengthSqr()
  {
    return dot(this);
  }
  double length()
  {
    return sqrt(lengthSqr());
  }
}

