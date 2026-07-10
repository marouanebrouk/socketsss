#pragma once

#include "array_bag.hpp"
#include "searchable_bag.hpp"

class searchablearraybag : public searchable_bag, public array_bag
{
    public :
    searchablearraybag(){};
    searchablearraybag(const searchablearraybag &obj) : array_bag(obj){};
    searchablearraybag &operator=(const searchablearraybag &obj);
    ~searchablearraybag(){};
    bool has(int value) const;
};