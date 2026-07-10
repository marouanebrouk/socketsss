#pragma once

#include "searchable_bag.hpp"
#include "searchable_array_bag.hpp"

class set
{
    private:
        searchable_bag *s_bag;
    public:
        set(searchable_bag &b);
        set();
        set(const set &obj);
        set &operator=(const set &obj);
        ~set();

        bool has(int) const;
        void insert(int);
        void insert(int*,int);
        void clear();
        void print() const;
        const searchable_bag &get_bag();
};