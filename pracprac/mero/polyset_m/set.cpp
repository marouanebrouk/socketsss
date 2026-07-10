#include "set.hpp"

set::set() : s_bag(nullptr)
{
};

set::set(searchable_bag &b) : s_bag(&b)
{
};
set::set(const set &obj) : s_bag(obj.s_bag)
{
};

set &set::operator=(const set &obj)
{
    if (this != &obj)
    {
        s_bag = obj.s_bag;
    }
    return (*this);
};
set::~set()
{
};

bool set::has(int value) const
{
    return s_bag->has(value);
};

void set::insert(int value)
{
    if (!(this->has(value)))
        s_bag->insert(value);
};

void set::insert (int *data, int size)
{
    for (int i = 0; i < size ; i++)
    {
        if (!s_bag->has(data[i]))
            s_bag->insert(data[i]);
    }
};


void set::print() const
{
    s_bag->print();
}


void set::clear()
{
    s_bag->clear();
}

const searchable_bag &set::get_bag()
{
    return *s_bag;
}
