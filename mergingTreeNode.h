//recommended compiler: Microsoft® Visual Studio 8 (2005) or higher
//mergingTreeNode class interface.
#ifndef MERGINGTREENODE_H
#define MERGINGTREENODE_H


// forward declaration of class Tree
template< typename NODETYPE > class mergingTree;

// TreeNode class-template definition
template< typename NODETYPE >
class mergingTreeNode
{
    friend class mergingTree< NODETYPE >;
    friend class HuffmanCoding;
public:
    // constructor
    mergingTreeNode( const NODETYPE &d )
        : leftPtr( 0 ), // pointer to left subtree
        rightPtr( 0 ) // pointer to right substree
    {
        data = d;
    } // end TreeNode constructor

    // return copy of node's data
    NODETYPE getData() const
    {
        return data;
    } // end getData function
private:
    mergingTreeNode< NODETYPE > *leftPtr; // pointer to left subtree
    NODETYPE data;
    mergingTreeNode< NODETYPE > *rightPtr; // pointer to right subtree
}; // end class TreeNode
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