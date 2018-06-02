//recommended compiler: Microsoft� Visual Studio 8 (2005) or higher
//mergingTree class interface.

///////////// Header Files /////////////////
#ifndef MERGINGTREE_H
#define MERGINGTREE_H

#include "mergingTreeNode.h"
#include "heap.h"
#include <string>

using namespace std;

#include <ostream>

using std::string;

#include <vector>
using std::vector;

// forward declaration of class heap
template< typename NODETYPE > class Heap;

// mergingTree class-template interface
template< typename NODETYPE > class mergingTree
{
public:
    mergingTree();                                      // constructor
    ~mergingTree();                                     //destructor
    mergingTree &operator=(const mergingTree &);        //assignment operator
    bool operator==(const mergingTree &) const;         //equality operator
    void insertNode( const NODETYPE );                  //insert new item into tree's heap
    int depth() const;                                  //tree's depth
    void createTree();                                  //create tree with items inside heap
    bool isEmpty() const                                //check if tree is empty
         { return rootPtr == NULL; }
    void createHuffmanArrays();                         //create arrays used with huffman compression method
    vector<string> getHuffmanNumbers() const;           //return a vector containing numbers
                                                        //  created with createHuffmanArrays
    vector<NODETYPE> HuffmanElements() const;           //return a vector containing elements
                                                        //  created with createHuffmanArrays
    mergingTreeNode< NODETYPE > *getRootCopy();         //get a copy of created tree
private:
    mergingTreeNode< NODETYPE > *rootPtr;               //root's pointer
    Heap < NODETYPE > *hp;                              //heap
    int size;                                           //size of tree

    vector<NODETYPE> elements;                          //vector containing elements
                                                        //  created with createHuffmanArrays
    vector<string> numbers;                             //vector containing elements
                                                        //  created with createHuffmanArrays
    string tmp;                                         //string object to hold string created with createHuffmanArrays
    bool created;                                       //check if the tree is created or not


    // utility functions      
    void destroy(mergingTreeNode<NODETYPE> *);          //used to destory tree
    int max(int, int);                                  //returns maximum of it's arguments
    void createHuffmanArraysHelper( mergingTreeNode< NODETYPE > * ); //used with createHuffmanArray
    int depthHelper(mergingTreeNode< NODETYPE > *) const;            //used with depth
    //creates copy of tree

    //checks for equality
    bool equal (mergingTreeNode <NODETYPE> *, mergingTreeNode <NODETYPE> *) const;
}; // end class mergingTree

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