#include "vect2.hpp"


  vect2::vect2() : x(0), y(0)
  {
  };
    vect2::vect2(int num1, int num2) : x(num1), y(num2)
    {
    };
    vect2::vect2(const vect2 &obj)
    {
        *this = obj;
    };
    vect2 &vect2::operator=(const vect2 &obj)
    {
        if (this != &obj)        
        {
            x = obj.x;
            y = obj.y;
        }
        return (*this);
    };
    vect2::~vect2(){};


    int vect2::operator[](int index) const
    {
        if (index == 0)
            return x;
        return y;
    };
    int &vect2::operator[](int index)
    {
        if (index == 0)
            return x;
        return y;
    };

    vect2 vect2::operator++(int)
    {
        vect2 tmp = *this;
        this->x++;
        this->y++;
        return (tmp);
    };
    vect2 &vect2::operator++()
    {
        this->x++;
        this->y++;
        return (*this);
    };

    vect2 vect2::operator--(int)
    {
        vect2 tmp = *this;
        this->x--;
        this->y--;
        return (tmp);
    };
    vect2 &vect2::operator--()
    {
        this->x--;
        this->y--;
        return (*this);
    };

    vect2 vect2::operator+(const vect2 &obj) const
    {
        vect2 tmp = *this;
        tmp.x = this->x + obj.x;
        tmp.y = this->y + obj.y;
        return tmp;
    };
    vect2 vect2::operator-(const vect2 &obj) const
    {
        vect2 tmp = *this;
        tmp.x = this->x - obj.x;
        tmp.y = this->y - obj.y;
        return tmp;
    };
        vect2 vect2::operator*(const vect2 &obj) const
    {
        vect2 tmp = *this;
        tmp.x = this->x * obj.x;
        tmp.y = this->y * obj.y;
        return tmp;
    };

    vect2 &vect2::operator+=(const vect2 &obj)
    {
        this->x = this->x + obj.x;
        this->y = this->y+ obj.y;
        return (*this);
    };

    vect2 &vect2::operator-=(const vect2 &obj)
    {
        this->x = this->x - obj.x;
        this->y = this->y - obj.y;
        return (*this);
    };


    vect2 &vect2::operator*=(const vect2 &obj)
    {
        this->x = this->x * obj.x;
        this->y = this->y * obj.y;
        return (*this);
    };

    bool vect2::operator==(const vect2 &obj) const 
    {
        if ((this->x == obj.x) && (this->y == obj.y))
            return true;
        return false;
    };
    bool vect2::operator!=(const vect2 &obj) const 
    {
        if (*this == obj)
            return false;
        return true;
    };

    vect2 vect2::operator-() const
    {
        vect2 tmp = *this;
        tmp[0] = -tmp[0];
        tmp[1] = -tmp[1];
        return (tmp);
    };

    vect2 vect2::operator*(int num) const
    {
        vect2 tmp = *this;
        tmp.x = this->x * num;
        tmp.y = this->y * num;
        return tmp;
    };

    vect2 &vect2::operator*=(int num)
    {
        this->x = this->x * num;
        this->y = this->y * num;
        return (*this);
    };


    vect2 operator*(int value, const vect2 &obj)
    {
        vect2 tmp(obj);
        tmp *= value;
        return (tmp);
    };

std::ostream &operator<<(std::ostream &mycout,const vect2 &obj)
{
    mycout << "{" << obj[0] << ", " << obj[1] << "}";
    return mycout;
};