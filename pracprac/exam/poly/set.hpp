#pragma once

#include "searchable_bag.hpp"

class set 
{
    private :
        searchable_bag &s_bag;
    public:
    set(searchable_bag &b);
    void print() const;
    void insert(int value);
    void insert(int *data, int size);
    void clear();
    const searchable_bag &get_bag() const;
    bool has(int value) const;
};