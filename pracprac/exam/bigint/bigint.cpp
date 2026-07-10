#include "bigint.hpp"
std::string bigint::addStrings(const std::string &a,
                               const std::string &b)
{
    std::string res;
    int i = a.size() - 1;
    int j = b.size() - 1;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry)
    {
        int sum = carry;

        if (i >= 0)
            sum += a[i--] - '0';
        if (j >= 0)
            sum += b[j--] - '0';

        res.insert(res.begin(), sum % 10 + '0');
        carry = sum / 10;
    }
    return (removeLeadingZeros(res));
}

#include <sstream>
bigint::bigint(unsigned int n)
{
    /*
        std::stringstream ss;
        ss << number;          // Feed the number into the stream
        std::string s = ss.str(); // Extract the string
    */

    // this methode is the same as the one above but more simple and faster
    _big = std::to_string(n);
}

bigint::bigint(const bigint &other)
{
    _big = other._big;
}

bigint &bigint::operator=(const bigint &other)
{
    _big = other._big;
    return (*this);
}

std::string bigint::removeLeadingZeros(std::string str)
{
    while (str.size() > 1 && str[0] == '0')
        str.erase(0, 1); //explaination: erase the first character of the string, starting from index 0, and remove 1 character
    return (str);
}


bigint &bigint::operator+=(const bigint &other)
{
    _big = addStrings(_big, other._big);
    return (*this);
}

bigint bigint::operator+(const bigint &other) const
{
    bigint tmp(*this);
    tmp += other;
    return (tmp);
}

bigint bigint::operator+(unsigned int n) const
{
    return (*this + bigint(n));
}

bigint &bigint::operator++()
{
    *this += bigint(1); //explanation:
                        // what is bigint(1)? it is a temporary object of type bigint that is created with the value 1,
                        //  and then added to the current object using the operator+= function. This effectively increments the current object by 1.
    return (*this);
}

bigint bigint::operator++(int)
{
    bigint tmp(*this);
    ++(*this);
    return (tmp);
}

bigint bigint::operator<<(const bigint &shift) const
{
    bigint tmp(*this);
    tmp <<= shift;
    return (tmp);
}

bigint &bigint::operator<<=(const bigint &shift)
{
    unsigned int n = std::stoi(shift._big);

    if (_big == "0")
        return (*this);

    while (n--)
        _big += '0';
    return (*this);
}

bigint bigint::operator>>(const bigint &shift) const
{
    bigint tmp(*this);
    tmp >>= shift;
    return (tmp);
}

bigint &bigint::operator>>=(const bigint &shift)
{
    unsigned int n = std::stoi(shift._big);

    if (n >= _big.size())
        _big = "0";
    else
        _big.erase(_big.size() - n);

    _big = removeLeadingZeros(_big);
    return (*this);
}

bool bigint::operator<(const bigint &other) const
{
    if (_big.size() != other._big.size())
        return (_big.size() < other._big.size());
    return (_big < other._big);
}

bool bigint::operator<=(const bigint &other) const {return (!(*this > other));}

bool bigint::operator>(const bigint &other) const {return (other < *this);}

bool bigint::operator>=(const bigint &other) const {return (!(*this < other));}

bool bigint::operator==(const bigint &other) const{return (_big == other._big);}

bool bigint::operator!=(const bigint &other) const {return (!(*this == other));}

std::ostream &operator<<(std::ostream &o, const bigint &n)
{
    o << n._big;
    return (o);
}