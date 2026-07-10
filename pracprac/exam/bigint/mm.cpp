#include "mm.hpp"

// #include <algorithm>
// #include <sstream>
// bigint::bigint(unsigned int nbr = 0)
// {
//     _big = std::to_string(nbr);
//     //or
//     // std::stringstream ss;
//     // ss << nbr;
//     // _big = ss.str();
// };

// bigint::bigint(const bigint &other)
// {
//     _big = other._big;
// }
// bigint &bigint::operator=(const bigint &other)
// {
//     if (this != &other)
//         _big = other._big;
//     return (*this);
// }

// bool bigint::operator<(const bigint &other) const
// {
//     if (_big.size() != other._big.size())
//         return (_big.size() < other._big.size());
//     return (_big < other._big);
// }


// bool bigint::operator>(const bigint &other) const{ return ((other  < *this));}

// bool bigint::operator==(const bigint &other) const{return (_big == other._big);}

// bool bigint::operator!=(const bigint &other) const{return (!(*this == other));}

// bool bigint::operator>=(const bigint &other) const{ return (!(*this < other));}
// bool bigint::operator<=(const bigint &other) const{ return (!(*this > other));}




// std::string bigint::rmlz(std::string str)
// {
//     while (str.size() > 1 && str[0] == '0')
//         str.erase(0, 1);
//     return (str);
// }

// std::string bigint::addstrings(const std::string &s1,const std::string &s2)
// {
//     std::string result;
//     int carry = 0;
//     int i = s1.size() - 1;
//     int j = s2.size() - 1;
//     while (i >= 0 || j >= 0 || carry)
//     {
//         int sum = carry;
//         if (i >= 0)
//             sum = sum + s1[i--] - '0';
//         if (j >= 0)
//             sum = sum + s2[j--] - '0';
//         result.insert(result.begin(),sum % 10 + '0');
//         carry = sum / 10;
//     }
//     return (rmlz(result));
// }


// bigint &bigint::operator++()
// {
//     (*this) += bigint(1);
//     return (*this);
// };

// bigint bigint::operator++(int)
// {
//     bigint tmp(*this);
//     ++(*this);
//     return (tmp);
// };



//         bigint &bigint::operator+=(const bigint &obj)
//         {
//             _big = addstrings(_big, obj._big);
//             return (*this);
//         };
//         bigint bigint::operator+(const bigint &obj) const
//         {
//             bigint tmp(*this);
//             tmp += obj;
//             return (tmp);
//         };
//         bigint bigint::operator+(unsigned int n) const
//         {
//             return (*this + bigint(n));
//         };



//std::ostream &operator<<(std::ostream &out, const bigint &obj)
//         {
//             out <<  obj._big;
//             return (out);
//         };






















bigint::bigint(unsigned int nbr)
{
    _big = std::to_string(nbr);
}
bigint::bigint(const bigint &other)
{
    _big = other._big;
}
bigint &bigint::operator=(const bigint &other)
{
    if (this != &other)
        this->_big = other._big;
    return (*this);
}

std::string bigint::addstrings(const std::string &s1,const std::string &s2)
{
    std::string result;
    int i = s1.size() - 1;
    int j = s2.size() - 1;
    int carry = 0;
    while (i >= 0 || j >= 0 || carry)
    {
        int sum = carry;
        if (i>= 0)
            sum = sum + s1[i--] - '0';
        if (j >= 0)
            sum = sum + s2[j--] - '0';

        result.insert(result.begin(),sum % 10 + '0');
        carry = sum / 10;
    }
    return (rmlz(result));
}


std::string bigint::rmlz(std::string str)
{
    while (str.size() > 1 && str[0] == '0')
        str.erase(0,1);
    return (str);
}




bigint bigint::operator++(int)
{
    bigint tmp(*this);
    ++(*this);
    return (tmp);
}


bigint &bigint::operator++()
{
    *this += bigint(1);
    return (*this);
}


bool bigint::operator<(const bigint &other) const
{
    if (_big.size() != other._big.size())
        return (_big.size() < other._big.size());
    return (_big < other._big);
}

bool bigint::operator>(const bigint &other) const { return (other < *this);}
bool bigint::operator<=(const bigint &other) const { return (!(*this > other));}
bool bigint::operator>=(const bigint &other) const { return (!(*this < other));}
bool bigint::operator==(const bigint &other) const { return (_big == other._big);}
bool bigint::operator!=(const bigint &other) const { return (!(*this == other));}


 std::ostream &operator<<(std::ostream &out, const bigint &obj)
 {
    out << obj._big;
    return (out);
 };





bigint bigint::operator+(unsigned int n) const
{
    return (*this + bigint(n));
}

bigint bigint::operator+(const bigint &other) const
{
    bigint tmp(*this);
    tmp += other;
    return (tmp);
}

bigint &bigint::operator+=(const bigint &other)
{
    _big = addstrings(_big,other._big);
    return (*this);
}





bigint &bigint::operator<<=(const bigint &shift)
{
    unsigned int n = std::stoi(shift._big);

    if (_big == "0")
        return (*this);
    while (n--)
        _big = _big + '0';
    return (*this);
};

bigint bigint::operator<<(const bigint &shift) const
{
    bigint tmp(*this);
    tmp <<= shift;
    return (tmp);
};

bigint bigint::operator>>(const bigint &shift) const
{
    bigint tmp(*this);
    tmp >>= shift;
    return (tmp);
};

bigint &bigint::operator>>=(const bigint &shift)
{
    unsigned int n = std::stoi(shift._big);

    if ( n >= _big.size())
        _big = "0";
    else
        _big.erase(_big.size() - n);

    _big = rmlz(_big);
    return (*this);
};