#include "searchabletreebag.hpp"

searchabletreebag &searchabletreebag::operator=(const searchabletreebag &obj)
    {
        if (this != &obj)
            tree_bag::operator=(obj);
        return (*this);
    };

    bool searchabletreebag::has(int value) const
    {
        node *c;
        c = tree;
        while (c)
        {
            if (c->value == value)
                return true;
            if (value < c->value)
                c = c->l;
            else
                c = c->r;
        }
        return false;
    };