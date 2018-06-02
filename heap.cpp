/* 
recommended compiler: Microsoft� Visual Studio 8 (2005)

Definition of the Heap Class


*/

//with these pre-processor commands we ensure user
// that can use the file just with including one file (.H file) or
// two files (.cpp and .h files) or just their own file with the
// #include "heap.h" line!
// all of these forms are possible
#ifndef HEAP_CPP
#define HEAP_CPP

///////////// Header Files /////////////////
#include "heap.h"
#include <iterator>
#include <cmath>
#include <iostream>

using namespace std;

using std::cout;
using std::flush;

#include <stdexcept> // stdexcept header file contains runtime_error 
using std::runtime_error; // standard C++ library class runtime_error



///////////// Functions Definition ////////////////
////////////////////////////////////////////////////////////
//constructor: if the value of order is above or equal to zero
// the heap is a max-heap, else the heap is a min-heap
template< typename NODETYPE >
Heap< NODETYPE >::Heap(int order)
{
    if(order >= 0)
        HS = MAX;
    else
        HS = MIN;
}

////////////////////////////////////////////////////////////
//copy constructor
template< typename NODETYPE >
Heap< NODETYPE >::Heap( Heap<NODETYPE> &right )
{
    *this = right;
}

////////////////////////////////////////////////////////////
//change the enum variable's (the HS: abbreviation of Heap Status)
// value so we can change a min-heap to a max-heap
// at the end use the heapify to ensure that the 
// heap is in compatible format
template< typename NODETYPE >
void Heap< NODETYPE >::changeStatus()
{
    //change the status
    if(HS == MAX)
        HS = MIN;
    else
        HS = MAX;

    heapify(); //use the heapify function
}

////////////////////////////////////////////////////////////
//using cout, print the status
template< typename NODETYPE >
void Heap< NODETYPE >::printStatus() const
{
    if(HS == MAX)
        cout << "MaxHeap" << flush;
    else
        cout << "MinHeap" << flush;
}

////////////////////////////////////////////////////////////
//returns the status of the heap
template< typename NODETYPE >
int Heap< NODETYPE >::getStatus() const
{
    if(HS == MAX)
        return 1; // this is max-heap
    else
        return -1; //this is a min-heap
}

////////////////////////////////////////////////////////////
// compares two elements of the heap with given location
// then returns the comparision of them depending on
// the status of the heap
//Note: we use the at function to get access to the elements
// because if the location is illegal, the function throws 
// an exception and we can use this exception.
template< typename NODETYPE >
bool Heap< NODETYPE >::compare(int l, int r) const
{
    if( HS == MAX )
        return root.at(l) > root.at(r); //for max-heap use >
    else
        return root.at(l) < root.at(r); //for min-heap use <
}

////////////////////////////////////////////////////////////
//copies one heap to another
template< typename NODETYPE >
Heap< NODETYPE > &Heap< NODETYPE >::operator=(const Heap< NODETYPE > &right)
{
    root = right.root; //use the = operator of
    HS = right.HS;
    return *this;
}

////////////////////////////////////////////////////////////
//checks if heaps are equal or not!
template< typename NODETYPE >
bool Heap< NODETYPE >::operator==(const Heap< NODETYPE > &right) const
{
    if( root != right.root) //check if roots are equal
        return false;

    if( HS != right.HS )  // check equality of status of two heap
        return false;

    return true; //if everything is equal then return true
}

////////////////////////////////////////////////////////////
//swap two elements of the root with given locations
template< typename NODETYPE >
void Heap< NODETYPE >::swap(int sub1,int sub2)
{
    NODETYPE tmp = root.at(sub1);
    root.at(sub1) = root.at(sub2);
    root.at(sub2) = tmp;
}

////////////////////////////////////////////////////////////
//return location of the father of an element with given location
template< typename NODETYPE >
int Heap< NODETYPE >::father( int sub ) const
{
    if( (sub % 2) == 0 ) //if this is the right child
    {
        sub /= 2;
        sub--;
    }
    else //this is the left child
        sub /= 2;

    return sub;
}

////////////////////////////////////////////////////////////
//return location of the left child of an element with given location
template< typename NODETYPE >
int Heap< NODETYPE >::leftChild( int sub )  const
{
    return 2 * sub + 1;
}

////////////////////////////////////////////////////////////
//return location of the right child of an element with given location
template< typename NODETYPE >
int Heap< NODETYPE >::rightChild( int sub ) const
{
    return 2 * sub + 2 ;
}

////////////////////////////////////////////////////////////
//insert new item
template< typename NODETYPE >
void Heap< NODETYPE >::insert( const NODETYPE item )
{
    root.push_back( item ); //add new item at the end of the vector
    int i = (int)root.size() - 1, j; //i is location of the
    //inserted item

    //compare with father until we get to the root
    // Note: if we get to the root, because we use the
    // at function we can check if we reached to the root
    // or not with the thrown exception, if at throws an exception
    // the we find out that the location of the father is illegal
    // so the item location is the root!
    while (true)
    {
        try
        {
            j = father(i); // j is location of father of the item

            if(compare(i,j)) //compare two elements
                swap(i, j); //swap them

            i = j; //now repeat this for the father
        }
        catch (...)
        {
            return; //we're at the root!
        }
    }
}

////////////////////////////////////////////////////////////
//print the elements of the vector begining at 0 to size of vector
template<typename NODETYPE>
ostream& operator<<(ostream& out,const Heap<NODETYPE>& hp)
{
    if( hp.empty() ) //if there is no item in the vector
    {
        if(hp.HS == MAX) //the algorithms is just like
            // the printStatus() function
            out << "MaxHeap" << flush;
        else
            out << "MinHeap" << flush;

        out << " is empty!";
        return out;
    }

    int size = (int)hp.root.size(); //get the size to prevent
    // the overhead of size() function

    out << hp.root[0]; // print the first item so we can print
    //the comma for other items

    for(int i = 1; i < size; i++)
        out << " , " << hp.root[i];

    out.flush(); //empty buffer

    return out;
}


////////////////////////////////////////////////////////////
//using the overloaded << operator and cout object
//  to print the elements
template< typename NODETYPE >
void Heap< NODETYPE >::print() const
{
    cout << *this;
}

////////////////////////////////////////////////////////////
//adjust:
// begining of the element with given location, 
// compare each item with left and right children,
// then replace it with the proper one.
// the proper one means that if this is the max-heap
// so we must swap it with the bigger one and if 
// this is a min-heap with the smaller one.
template< typename NODETYPE >
void Heap< NODETYPE >::adjust(int item)
{
    int left, right;

    //use expception handling to check if we reach
    // to the end of the heap
    while (true)
    {
        try
        {
            //find the location of the left and right children
            left = leftChild(item);
            right = rightChild(item);

            int sz = (int)root.size();

            if( left >= sz )
            {
                if ( right >= sz )
                    throw runtime_error( "no change needed");
                else if(compare(right,item))
                    swap(right,item);
            }
            else if ( right >= sz )
                if(compare(left,item))
                    swap(left,item);

            //compare and decide which one must be swapped
            if( compare(right,left) )
            {
                swap(item,right);
                item = right;
            }
            else
            {
                swap(item,left);
                item = left;
            }
        }
        catch (...)
        {
            return;
        }
    }
}

////////////////////////////////////////////////////////////
//beginng of the first element with child use adjust algorithm
// so at the end we have a heap!
template< typename NODETYPE >
void Heap< NODETYPE >::heapify()
{
    for(int i = (int)root.size() / 2; i > 1; i--)
        adjust(i);
}

////////////////////////////////////////////////////////////
//omits the root
template< typename NODETYPE >
NODETYPE Heap< NODETYPE >::omit()
{
    if(root.empty()) //check if the vector is empty
    {
        NODETYPE *a;
        a = new NODETYPE;
        return *a;
    }

    NODETYPE b = root[0];
    swap(0,(int)root.size() - 1); //swap the last element with root
    root.pop_back(); // delete the last element (with value of root)
    adjust(0); //use adjust for root
    return b;
}

////////////////////////////////////////////////////////////
//originally on MSDN:
//ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.VisualStudio.v80.en/dv_vccrt/html/7adc77c2-04f7-4245-a980-21215563cfae.htm
template< typename NODETYPE >
double Heap< NODETYPE >::logbase(double a, double base) const
{
    return log(a) / log(base);
}

////////////////////////////////////////////////////////////
// calculate the depth of the heap
// if the heap is a full tree, the depth is log(size)
// else the depth is log(size) + 1
//Note: because the size is number of elements 
// we must add 1 to it to get the logarithm
template< typename NODETYPE >
int Heap< NODETYPE >::depth() const
{
    if(root.empty()) //if empty return 0
        return 0;

    double dtmp = logbase((double)root.size()+1,2.0); //log with base 2
    int temp = static_cast<int>(dtmp), //integer of logarithm
        dep;

    if( (dtmp - (double)temp) == 0.0 ) //if the tree is full
        dep = static_cast<int>(floor(dtmp));
    else
        dep = static_cast<int>(floor(dtmp)) + 1;

    return dep;
}

#endif
/***************************************************************************\
|                                                                           |
|                                                                           |
|                                                                           |
|                             Alireza Noori �                               |
|                                                                           |
|                                                                           |
|                                                                           |
|                                                                           |
\***************************************************************************/