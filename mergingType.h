//recommended compiler: Microsoft® Visual Studio 8 (2005)
//mergingType class interface.

/*

we use this class to create new data type
so we can store both character and it's count
into mergingTree and Heap classes and just use
the count element to compare two elements

*/

#ifndef MERGINGTYPE_H
#define MERGINGTYPE_H


// mergingType interface
class mergingType
{
    friend class HuffmanCoding;
public:
    //constructor copy data and count to class
    mergingType(int dat = 0, int cnt = 0)
    { data = dat; count = cnt; }

    //copy right's elements into this class's elements
    mergingType &operator=(const mergingType &right)
    { data = right.data; count = right.count; return *this;}

    //both of items must be equal
    bool operator==(const mergingType &right) const
    { return ( (count == right.count) && (data == right.data) ); }

    //because equality is checked data is useless
    bool operator<=(const mergingType &right) const
    { return count <= right.count; }

    //because equality is checked data is useless
    bool operator>=(const mergingType &right) const
    { return count >= right.count; }

    bool operator<(const mergingType &right) const
    {
        if(count < right.count)
            return true;
        else if(count > right.count)
            return false;
        else //if they are equal use the greater data
            //   so we use roots instead of new roots
            // for example if we have 3 nodes: 1,1,2
            // the root containing sum of 1 and 1 has 
            // greater data because it stores sum of
            // 2 element's data so when the heap is 
            // sorting these elements it returns the
            // element with greater data and this means
            // it returns roots first
            return data > right.data;
    }

    //as for operator<
    bool operator>(const mergingType &right) const
    {
        if(count > right.count)
            return true;
        else if(count < right.count)
            return false;
        else
            return data < right.data;
    }

    //return a new object containing some of both elements
    mergingType operator+(const mergingType &right) const
    { mergingType tmp(data + right.data, count + right.count); return tmp;}
private:
    int data;
    int count;
};
#endif

/***************************************************************************\
|                                                                           |
|                                                                           |
|                                                                           |
|                             Alireza Noori ©                               |
|                                                                           |
|                                                                           |
|                                                                           |
|                                                                           |
\***************************************************************************/