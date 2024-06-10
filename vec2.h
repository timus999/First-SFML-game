#ifndef VEC2_H
#define VEC2_H
class Vec2{

  public:
    float x, y;
    Vec2();
    Vec2(float xin, float yin);

    bool operator == ( const Vec2 & rhs) const;
    bool operator != ( const Vec2 & rhs) const;

    Vec2 operator +(const Vec2 & rhs) const;
    Vec2 operator -(const Vec2 & rhs) const;
    Vec2 operator *(const float val) const;
    Vec2 operator /(const float val) const;

    void operator += (const Vec2 & rhs);
    void operator -= (const Vec2 & rhs);
    void operator *= (const float val);
    void operator /= (const float val);

    float distance(const Vec2 & rhs) const;
    float length() const;
    void normalize();

};



#endif 
