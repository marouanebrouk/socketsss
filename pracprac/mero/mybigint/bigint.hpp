#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <algorithm>

class bigint
{
    private:
        std::string _big;
    public:

        bigint(unsigned int nbr = 0) : _big(std::to_string(nbr)){};
        bigint(const bigint &other) : _big(other._big){};
        std::string getBig() const  {return _big;}
        
        bigint &operator+=(const bigint &other);
        bigint operator+(const bigint &other) const;
        
        bigint &operator>>=(const bigint &other) ;
        bigint operator<<(unsigned int amount) const;
        bigint &operator<<=(unsigned int amount);

        bigint operator++(int);
        bigint &operator++();

        bool operator>(const bigint &other) const;
        bool operator<(const bigint &other) const;
        bool operator>=(const bigint &other) const;
        bool operator<=(const bigint &other) const;
        bool operator==(const bigint &other) const;
        bool operator!=(const bigint &other) const;
};

std::ostream &operator<<(std::ostream &out,const bigint &other);


#endif
