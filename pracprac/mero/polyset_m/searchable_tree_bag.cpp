#include "searchable_tree_bag.hpp"



        searchable_tree_bag::searchable_tree_bag()
        {};
        searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag&obj) : tree_bag(obj)
        {};
        searchable_tree_bag &searchable_tree_bag::operator=(const searchable_tree_bag&obj)
        {
            if (this != &obj)
                tree_bag::operator=(obj);
            return(*this);
        };
        searchable_tree_bag::~searchable_tree_bag()
        {};
        bool searchable_tree_bag::has(int value) const
        {
            node *c = tree;
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