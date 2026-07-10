#pragma once

#include <iostream>



class bigint
{
    private:
        std::string _big;
        static std::string addstrings(const std::string &s1, const std::string &s2);//////////////
        static std::string rmlz(std::string str);//////////////
    public:
        bigint(unsigned int nbr = 0);//////////////
        bigint(const bigint &other);//////////////
        bigint &operator=(const bigint &other);//////////////




        bigint &operator<<=(const bigint &shift);
        bigint operator<<(const bigint &shift) const;
        bigint &operator>>=(const bigint &shift);
        bigint operator>>(const bigint &shift) const;



        bigint operator++(int);////////////
        bigint &operator++(); //////////////

        bigint &operator+=(const bigint &other);//
        bigint operator+(const bigint &other) const;//
        bigint operator+(unsigned int n) const;//



        bool operator<(const bigint &other) const ;//////////////
        bool operator>(const bigint &other) const;//////////////
        bool operator>=(const bigint &other) const;//////////////
        bool operator<=(const bigint &other) const;//////////////
        bool operator==(const bigint &other) const;//////////////
        bool operator!=(const bigint &other) const;//////////////
        friend std::ostream &operator<<(std::ostream &out, const bigint &obj); //
};
































































// class bigint
// {
//     private :
//         std::string _big;
//         static std::string addstrings(const std::string &s1,const std::string &s2);
//         static std::string rmlz(std::string str);
//     public :
//         bigint(unsigned int nbr = 0);
//         bigint(const bigint &other);
//         bigint &operator=(const bigint &other);

//         bigint &operator++();
//         bigint operator++(int);

//         bigint &operator+=(const bigint &obj);
//         bigint operator+(const bigint &obj) const;
//         bigint operator+(unsigned int n) const;

//         bigint &operator<<=(const bigint &shift);
//         bigint operator<<(const bigint &shift) const;

//         bigint &operator>>=(const bigint &shift);
//         bigint operator>>(const bigint &shift) const;


//         bool operator==(const bigint &other) const;
//         bool operator!=(const bigint &other) const;
//         bool operator>=(const bigint &other) const;
//         bool operator<=(const bigint &other) const;
//         bool operator<(const bigint &other) const;
//         bool operator>(const bigint &other) const;
//         friend std::ostream &operator<<(std::ostream &out, const bigint &obj);
// };