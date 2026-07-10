#ifndef SEARCHABLE_ARRAY_BAG
#define SEARCHABLE_ARRAY_BAG


#include "searchable_bag.hpp"
#include "array_bag.hpp"

class searchable_array_bag : public array_bag, public searchable_bag
{
    public:
    searchable_array_bag();
    searchable_array_bag(const searchable_array_bag &obj);
    searchable_array_bag &operator=(const searchable_array_bag &obj);
    ~searchable_array_bag();
    bool has(int) const;
};



#endif