#include "bigint.hpp"

// bigint &bigint::operator+=(const bigint &other)
// {
//     std::string result;
//     int carry = 0;
//     int i = _big.size() - 1;
//     int j = other._big.size() - 1;

//     while (i >= 0 || j >= 0 || carry)
//     {
//         int sum = carry;

//         if (i >= 0)
//             sum += _big[i--] - '0';
//         if (j >= 0)
//             sum += other._big[j--] - '0';

//         result.push_back(sum % 10 + '0');
//         carry = sum / 10;
//     }

//     std::reverse(result.begin(), result.end());
//     _big = result;
//     return *this;
// }







// bigint bigint::operator+(const bigint &other) const
// {
//     bigint res(*this);
//     return res += other;
// }

// bigint &bigint::operator++()
// {
//     return *this += bigint(1);
// }

// bigint bigint::operator++(int)
// {
//     bigint tmp(*this);
//     ++(*this);
//     return tmp;
// }

// bigint &bigint::operator<<=(unsigned int amount)
// {
//     if (_big != "0")
//         _big += std::string(amount, '0');
//     return *this;
// }

// bigint bigint::operator<<(unsigned int amount) const
// {
//     bigint res(*this);
//     return res <<= amount;
// }

// bigint &bigint::operator>>=(const bigint &other)
// {
//     unsigned long amount = std::atol(other._big.c_str());

//     if (amount >= _big.size())
//         _big = "0";
//     else
//         _big.erase(_big.size() - amount);

//     return *this;
// }

// bool bigint::operator<(const bigint &other) const
// {
//     if (_big.size() != other._big.size())
//         return _big.size() < other._big.size();
//     return _big < other._big;
// }

// bool bigint::operator>(const bigint &other) const  {return other < *this;} // trifling


bool bigint::operator>(const bigint &other) const { return (*this > other);}




bool bigint::operator>=(const bigint &other) const { return (!(*this < other));}

bool bigint::operator<=(const bigint &other) const { return (!(*this > other));}

bool bigint::operator==(const bigint &other) const {return _big == other._big;}

bool bigint::operator!=(const bigint &other) const {return (!(*this == other));}

std::ostream &operator<<(std::ostream &out,const bigint &other)
{
    out << other.getBig();
    return out;
}
