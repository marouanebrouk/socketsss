#include "searchablearraybag.hpp"


    searchablearraybag &searchablearraybag::operator=(const searchablearraybag &obj)
    {
        if (this != &obj)
            array_bag::operator=(obj);
        return (*this);
    };

    bool searchablearraybag::has(int value) const
    {
        for (int i = 0; i < size;i++)
        {
            if (this->data[i] == value)
                return true;
        }
        return false;
    }