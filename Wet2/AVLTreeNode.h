#ifndef DATASTRUCTURES_AVL_TREE_NODE_H
#define DATASTRUCTURES_AVL_TREE_NODE_H

template<class T, class S>
class AVLTree;

enum class SonType
{
    ROOT = 0,
    RIGHT = 1,
    LEFT = 2
};

template<class T, class S>
class AVLTreeNode
{
    friend class AVLTree<T, S>;

private:
    T *key;
    S *value;
    AVLTreeNode *left;
    AVLTreeNode *right;
    AVLTreeNode *parent;
    int height;
    int nodesInSub;

    AVLTreeNode(T *key, S *value);

    /*
     * Explicitly telling the compiler to use the default methods
    */
    AVLTreeNode(const AVLTreeNode &) = default;
    ~AVLTreeNode() = default;
    AVLTreeNode &operator=(const AVLTreeNode &other) = default;

    //Checks the balance factor of said node in the tree
    int balanceFactor();
    //Updates the height of the node in the tree
    void updateHeight();
    //Updates the rank of the node in the tree
    void updateRank();
    //Return the son type of the node in correlation to it's parent
    SonType getSonType();

};

template<class T, class S>
AVLTreeNode<T, S>::AVLTreeNode(T *key, S *value) :
        key(key), value(value), left(nullptr), right(nullptr), parent(nullptr), height(0), nodesInSub(1)
{}

template<class T, class S>
int AVLTreeNode<T, S>::balanceFactor()
{
    if (!left && !right)
    {
        return 0;
    }
    if (!left)
    {
        return -1 - right->height;
    }
    if (!right)
    {
        return left->height + 1;
    }
    return left->height - right->height;
}

template<class T, class S>
void AVLTreeNode<T, S>::updateHeight()
{
    if (!left && !right)
    {
        this->height = 0;
    }
    else if (!left)
    {
        this->height = right->height + 1;
    }
    else if (!right)
    {
        this->height = left->height + 1;
    }
    else if(left->height > right->height)
    {
        this->height = left->height + 1;
    }
    else
    {
        this->height = right->height + 1;
    }
}

template<class T, class S>
void AVLTreeNode<T, S>::updateRank()
{
    if (!left && !right)
    {
        this->nodesInSub = 1;
    }
    else if (!left)
    {
        this->nodesInSub = right->nodesInSub + 1;
    }
    else if (!right)
    {
        this->nodesInSub = left->nodesInSub + 1;
    }
    else
    {
        this->nodesInSub = left->nodesInSub + right->nodesInSub + 1;
    }
}

template<class T, class S>
SonType AVLTreeNode<T, S>::getSonType()
{
    if (!parent)
        return SonType::ROOT;

    if (this == parent->left)
        return SonType::LEFT;

    return SonType::RIGHT;
}

#endif //DATASTRUCTURES_AVL_TREE_NODE_H
