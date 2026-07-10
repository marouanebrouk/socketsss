#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <string>

class bigint
{
    private:
        std::string _big;

        static std::string addStrings(const std::string &a ,const std::string &b);
        static std::string removeLeadingZeros(std::string str);

    public:
        bigint(unsigned int n = 0); //
        bigint(const bigint &other);//
        bigint &operator=(const bigint &other);//

        bigint &operator+=(const bigint &other); //
        bigint operator+(const bigint &other) const; //
        bigint operator+(unsigned int n) const; //

        bigint &operator<<=(const bigint &shift);
        bigint operator<<(const bigint &shift) const;
        bigint &operator>>=(const bigint &shift);
        bigint operator>>(const bigint &shift) const;

        bigint &operator++();   //
        bigint operator++(int); //

        bool operator<(const bigint &other) const;//
        bool operator<=(const bigint &other) const;//
        bool operator>(const bigint &other) const;//
        bool operator>=(const bigint &other) const;//
        bool operator==(const bigint &other) const;//
        bool operator!=(const bigint &other) const;//

        friend std::ostream &operator<<(std::ostream &o,
                                        const bigint &n);//
};
#endif