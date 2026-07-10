#pragma once


#include <iostream>

class vect2 
{
    private:
        int x;
        int y;
    public:
    vect2();
    vect2(int num1, int num2);
    vect2(const vect2 &obj);
    vect2 &operator=(const vect2 &obj);
    ~vect2();


    int operator[](int index) const;
    int &operator[](int index);

    vect2 operator++(int);
    vect2 &operator++();
    vect2 operator--(int);
    vect2 &operator--();

    vect2 operator+(const vect2 &obj) const;
    vect2 operator-(const vect2 &obj) const;
    vect2 operator*(const vect2 &obj) const;

    vect2 &operator+=(const vect2 &obj);
    vect2 &operator-=(const vect2 &obj);
    vect2 &operator*=(const vect2 &obj);

    bool operator==(const vect2 &obj) const ;
    bool operator!=(const vect2 &obj) const ;

    vect2 operator-() const;
    vect2 operator*(int num) const;
    vect2 &operator*=(int num);
};

vect2 operator*(int value, const vect2 &obj);

std::ostream &operator<<(std::ostream &mycout,const vect2 &obj);