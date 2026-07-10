#pragma once

#include "tree_bag.hpp"
#include "searchable_bag.hpp"

class searchabletreebag : public searchable_bag, public tree_bag
{
    public :
    searchabletreebag(){};
    searchabletreebag(const searchabletreebag &obj) : tree_bag(obj){};
    searchabletreebag &operator=(const searchabletreebag &obj);
    ~searchabletreebag(){};
    bool has(int value) const;
};