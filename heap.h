//recommended compiler: Microsoft� Visual Studio 8 (2005)
//Heap class interface.
#ifndef HEAP_H
#define HEAP_H

///////////// Header Files /////////////////
#include <vector>
#include <ostream>


using std::vector;
using namespace std;

enum HeapStatus {MAX,MIN};

// Heap class-template interface
template< class NODETYPE > class Heap
{
    template<NODETYPE> friend ostream& operator << (ostream&, const Heap <NODETYPE> &);
public:
    Heap(int = 1);                              //constructor: positive for max-heap,
    //negative for min-heap
    Heap( Heap<NODETYPE> & );                   //copy constructor
    Heap &operator=(const Heap &);              //copy of the heap
    bool operator==(const Heap &) const;        //check for equality
    bool operator!= (const Heap &right) const   //check for inequality
    {return !(*this == right);}

    void insert( const NODETYPE );              //insert new item to heap
    NODETYPE omit();                            //omit the root of the heap and return omited
    int depth() const;                          //return the depth of the heap

    void printStatus() const;                   //print if the heap is min or max heap
    int getStatus() const;                      //get the status in int format
    void changeStatus();                        //change the max-heap to min-heap
                                                //  and vice versa

    size_t getSize() const                      //get the size of the heap
    {return root.size();} //inline function
    bool empty() const                        //check if the heap is empty
    { return root.empty(); } //inline function
    void print() const;                         //print the elements of the heap
private:
    vector < NODETYPE > root;                   //the vector containing the elements
    HeapStatus HS;                              //status of the heap

    //utility functions
    bool compare(int, int) const;               //depending on status compares two items

    int father( int ) const;                    //returns father's location of an item
    int leftChild( int ) const;                 //returns left child's location of an item
    int rightChild( int ) const;                //returns right child's location of an item

    void adjust(int);                           //adjust algorithm
    void heapify();							    //heapify algorithm
    void swap(int,int);                         //swap two items with given locations
    double logbase(double, double) const;       // generates the logarithm of any base 
                                                //log base b of a == natural log (a) / natural log (b)
                                                //   used from MSDN (2005)
}; // end class Heap

#include "heap.cpp" //load the definitions!
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