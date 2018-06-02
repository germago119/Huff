/* 


Definition of the mergingTree Class


*/

#ifndef MERGINGTREE_CPP
#define MERGINGTREE_CPP

///////////// Header Files /////////////////
#include "mergingTree.h"
#include "mergingTreeNode.h"

#include <string>
using std::string;

#include <stdexcept> // stdexcept header file contains runtime_error 
using std::runtime_error; // standard C++ library class runtime_error

///////////// Functions Definition ////////////////
////////////////////////////////////////////////////////////
//constructor
template< typename NODETYPE >
mergingTree< NODETYPE >::mergingTree()
{
    rootPtr = NULL; // indicate mergingTree is initially empty
    hp = new Heap<NODETYPE>(-1);
    size = 0;
    created = false;
} // end mergingTree constructor

////////////////////////////////////////////////////////////
//destructor
template< typename NODETYPE >
mergingTree< NODETYPE >::~mergingTree()
{
    destroy(rootPtr);
} // end mergingTree destructor

////////////////////////////////////////////////////////////
// destroy nodes
template< typename NODETYPE >
void mergingTree< NODETYPE >::destroy(mergingTreeNode<NODETYPE> *ptr)
{
    if(ptr != NULL)
    {
        destroy(ptr->leftPtr);
        destroy(ptr->rightPtr);
        delete ptr;
    }
}

////////////////////////////////////////////////////////////
// insert node in mergingTree
template< typename NODETYPE >
void mergingTree< NODETYPE >::insertNode( const NODETYPE value )
{
    hp->insert(value);
    size++;
} // end function insertNode

////////////////////////////////////////////////////////////
// create the mergingTree
template< typename NODETYPE >
void mergingTree< NODETYPE >::createTree()
{
    NODETYPE l,r; //value to hold minimums of heap
    int rootsSize = 0;
    bool rFound, lFound;
    //pointers to create tree
    mergingTreeNode<NODETYPE> *lChild,*rChild,*newRoot;
    //vector to hold created seperate roots so 
    //  we don't lost any created root
    vector<mergingTreeNode<NODETYPE> *> roots;

    l = hp->omit(); //l is first minimum
    r = hp->omit(); //r is second minimum
    //create nodes with l and r
    rChild = new mergingTreeNode<NODETYPE> (r);
    lChild = new mergingTreeNode<NODETYPE> (l);

    //create root with cost(l) + cost(r)
    newRoot = new mergingTreeNode<NODETYPE> (l + r);
    //link childs to father(dad)
    newRoot->leftPtr = lChild;
    newRoot->rightPtr = rChild;
    //inset root to vector
    roots.push_back(newRoot);
    //also into heap
    hp->insert(l + r);

    while( (hp->getSize()) > 1) //until the last element
    {
        rFound = lFound = false;
        l = hp->omit(); //l is first minimum
        r = hp->omit(); //r is second minimum

        //to prevent overhead of size() function
        rootsSize = (int)roots.size();
        //hold location of left and right roots inside
        // the vector if found
        int rLocation, lLocation;
        //first try to find cost inside roots
        for(int w = 0; w < rootsSize; w++)
        {
            //if the root inside the vector at this location
            //  holds a root with data equal to l so
            //  set this root at lChild and use it as left child
            //  of new root, also set boolean variable lFound
            //  to true so we do not replace lChild with another 
            //  root and just search for right child
            if( (l == roots[w]->data) &&
                !lFound )
            {
                lFound = true;
                lChild = roots[w];
                lLocation = w;
            }
            //just as left child
            else if( (r == roots[w]->data) &&
                !rFound)
            {
                rFound = true;
                rChild = roots[w];
                rLocation = w;
            }
        }

        //if we didn't found the left or right child
        // inside roots vecor so create new nodes
        if(!rFound)
            rChild = new mergingTreeNode<NODETYPE> (r);
        if(!lFound)
            lChild = new mergingTreeNode<NODETYPE> (l);

        //create new root
        newRoot = new mergingTreeNode<NODETYPE> (l + r);
        newRoot->leftPtr = lChild;
        newRoot->rightPtr = rChild;

        //if we used just one root of vector replace new root
        // with that root, but if we used both of childs of 
        // the vector roots so we must replace one with new root
        // and delete another from the vector
        if( rFound && !lFound )
            roots[rLocation] = newRoot;
        else if( !rFound && lFound )
            roots[lLocation] = newRoot;
        else if( rFound && lFound )
        {
            roots[lLocation] = newRoot;
            roots[rLocation] = roots[roots.size() - 1];
            roots.pop_back();
        }
        else
            roots.push_back(newRoot);

        //insert sum of costs inside the heap
        hp->insert(l + r);
    }
    rootPtr = roots[0];
}

////////////////////////////////////////////////////////////
// get the array for huffman coding
template< typename NODETYPE >
void mergingTree< NODETYPE >::createHuffmanArrays()
{
    createHuffmanArraysHelper( rootPtr ); //begin from root
    created = true;
} // end function getHuffmanArray


////////////////////////////////////////////////////////////
// utility function to perform inorder traversal of mergingTree
template< typename NODETYPE >
void mergingTree< NODETYPE >::createHuffmanArraysHelper( mergingTreeNode< NODETYPE > *ptr )
{
    if(ptr != NULL)
    {
        //if this is a leaf
        if( (ptr->leftPtr == NULL) && 
            (ptr->rightPtr == NULL) )
        {
            //add element and it's string to arrays
            elements.push_back(ptr->data);
            numbers.push_back(tmp);
        }
        else
        {
            //while going to left add 0 to string
            tmp += "0";
            createHuffmanArraysHelper(ptr->leftPtr);
            //while going to right add 1 to string
            tmp += "1";
            createHuffmanArraysHelper(ptr->rightPtr);
        }
    }
    string newTmp = tmp.substr(0,tmp.size() - 1);
    tmp = newTmp;
} // end function getHuffmanArrayHelper

////////////////////////////////////////////////////////////
//begin from head to find the deapth
template< typename NODETYPE >
int mergingTree<NODETYPE>::depth() const
{
    //begin from root
    return depthHelper(rootPtr);
}

////////////////////////////////////////////////////////////
//utility function to find the maximum of two number
template< typename NODETYPE >
int mergingTree<NODETYPE>::max(int a, int b)
{
    return (a > b) ? a : b;
}

////////////////////////////////////////////////////////////
//utility function to find the deapth of the mergingTree
template< typename NODETYPE >
int mergingTree<NODETYPE>::depthHelper(mergingTreeNode< NODETYPE > *ptr) const
{
    if(ptr == NULL) //if NULL so 0 depth
        return 0;
    else //recursively calculate the depth
        return 1 + max(depthHelper(ptr->leftPtr),
    depthHelper(ptr->rightPtr));
}


////////////////////////////////////////////////////////////
//copy one mergingTree onto another
// recursive implementation


////////////////////////////////////////////////////////////
//return a copy of the root
////////////////////////////////////////////////////////////
//operator = to copy one mergingTree to another


////////////////////////////////////////////////////////////
//return true if equal, false if not
template< typename NODETYPE >
bool mergingTree<NODETYPE>::equal (mergingTreeNode <NODETYPE> *n1, mergingTreeNode <NODETYPE> *n2) const
{
    mergingTreeNode <NODETYPE> *L1 = n1->leftPtr,
        *L2 = n2->leftPtr,
        *R1 = n1->rightPtr,
        *R2 = n2->rightPtr;

    if(n1->data != n2->data)
        return false;

    if( (L1 && L2) && (R1 && R2) )
        return ( equal(L1,L2) && equal(R1, R2) );

    if( (L1 && !L2) ||
        (!L1 && L2) ||
        (R1 && !R2) ||
        (!R1 && R2) )
        return false;

    return true;
}

////////////////////////////////////////////////////////////
template< typename NODETYPE >
bool mergingTree< NODETYPE >::operator==(const mergingTree< NODETYPE > &right) const
{
    return equal(rootPtr, right.rootPtr); //uses equal function
}

////////////////////////////////////////////////////////////
//return strings created with createHuffmanArraysHelper function
template< typename NODETYPE >
vector<string> mergingTree< NODETYPE >::getHuffmanNumbers() const
{
    if(!created)
        throw runtime_error( "Array's Not Created!");
    else
        return numbers;
}

////////////////////////////////////////////////////////////
//return elements created with createHuffmanArraysHelper function
template< typename NODETYPE >
vector<NODETYPE> mergingTree< NODETYPE >::HuffmanElements() const
{
    if(!created)
        throw runtime_error( "Array's Not Created!");
    else
        return elements;
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