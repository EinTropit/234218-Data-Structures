
#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <iostream> //-----------------------------------------------------------------------------------------
#include <exception>
#include "AVLTreeNode.h"


template<class T, class S>
class AVLTree
{
private:
    AVLTreeNode<T, S> *root;


public:
    AVLTree();
    AVLTree(S **playersArr, int size, T* (S::*ChooseKey)() const);
    ~AVLTree();

    // Explicitly telling the compiler to delete this methods
    AVLTree(const AVLTree &) = delete;

    // assignment operator
    AVLTree &operator=(const AVLTree &other);

    /**
     * Finds a node using a given key and returns the value stored in it.
     * @param key
     * @return
     */
    S *find(const T *key);

    /**
     * Inserts a new node to the tree via a key and attaches a value to it.
     * Throws an exception if the key already exist.
     * @param key
     * @param value
     */
    void insert(T *key, S *value);

    /**
     * Removes the node from the tree using a given key.
     * Throws an exception if the key does not exist.
     * @param key
     */
    void remove(T *key);

    /**
     * Returns the value of the first next key with a value greater than the given key
     * @param key
     * @return
     */
    S* findNext(const T *key);

    /**
     * Returns the value of the first previous key with a value smaller than the given key
     * @param key
     * @return
     */
    S* findPrevious(const T *key);

    /**
     * Returns the value of the first next key with a value equal or greater than the given key
     * @param key
     * @return
     */
    S* findEqOrGreater(const T *key);

    /**
     * Returns the value of the largest key in the tree
     * @return
     */
    S* findMax();

    /**
     * Puts the tree inorder to the array
     * (Required that the given array is large enough)
     * @param output
     */
    void arrayInOrder(S **const output);

    /**
     * Releases the values from the tree
     */
    void releaseValues();



    //possible exceptions to be thrown
    class KeyExists : public std::exception {};

    class KeyDoesNotExist : public std::exception {};

private:

    //Releases the nodes in the tree recursively using a postorder route
    void release(AVLTreeNode<T, S> *node);

    //Finds a node recursively using a given key
    AVLTreeNode<T, S> *findNode(const T *key, AVLTreeNode<T, S> *node);
    AVLTreeNode<T, S> *findParentNode(const T *key, AVLTreeNode<T, S> *node);

    //Decides which of the balancing rotations to use, rotates only once
    bool balance(AVLTreeNode<T, S> *parent);

    //Recursively puts inorder the tree to a given array
    int arrayInOrderRecursive(S **const output, AVLTreeNode<T, S>* curNode, int offset);

    //Recursively releases the values from the tree
    void releaseValuesRecursive(AVLTreeNode<T, S>* curNode);

    //Auxiliary functions for insert
    void insertBin(AVLTreeNode<T, S> *newNode, AVLTreeNode<T, S> *nodeRec);
    void balanceInsert(AVLTreeNode<T, S> *curNode);

    //Auxiliary functions for remove
    AVLTreeNode<T, S> *removeBin(AVLTreeNode<T, S> *toRemove);
    void balanceRemove(AVLTreeNode<T, S> *curNode);

    //Swaps the key and value of the two nodes
    static void swapNodes(AVLTreeNode<T, S> *node1, AVLTreeNode<T, S> *node2);

    //Rotations for tree balancing
    void rotateLL(AVLTreeNode<T, S> *parent);
    void rotateLR(AVLTreeNode<T, S> *parent);
    void rotateRR(AVLTreeNode<T, S> *parent);
    void rotateRL(AVLTreeNode<T, S> *parent);

    //Auxiliary function for updating the parent
    void updateParent(AVLTreeNode<T, S> *node, AVLTreeNode<T, S> *toUpdate, enum SonType sonType);

    AVLTreeNode<T, S>* generateTree(S **playersArr, int size, T* (S::*chooseKey)() const);
};


template<class T, class S>
AVLTree<T, S>::AVLTree() : root(nullptr)
{}

template<class T, class S>
AVLTree<T, S>::~AVLTree()
{
    release(root);
}


template<class T, class S>
AVLTree<T, S>::AVLTree(S **playersArr, int size, T *(S::*chooseKey)() const)
{
    root = generateTree(playersArr, size, chooseKey);
}

template<class T, class S>
AVLTreeNode<T, S> *AVLTree<T, S>::generateTree(S **playersArr, int size, T *(S::*chooseKey)() const)
{
    if (size <= 0)
    {
        return nullptr;
    }

    int mid = size/2;
    S* player = playersArr[mid];
    AVLTreeNode<T,S> *curNode = new AVLTreeNode<T, S>((player->*chooseKey)(), player);
    curNode->left = generateTree(playersArr, mid, chooseKey);
    curNode->right = generateTree(playersArr + mid+1, size-mid-1, chooseKey);

    if (curNode->left != nullptr)
        curNode->left->parent = curNode;
    if (curNode->right != nullptr)
        curNode->right->parent = curNode;

    curNode->updateHeight();

    return curNode;
}

template<class T, class S>
AVLTree<T, S> &AVLTree<T, S>::operator=(const AVLTree &other)
{
    if (this == &other)
        return *this;
    root = other.root;
    return *this;
}

template<class T, class S>
void AVLTree<T, S>::release(AVLTreeNode<T, S> *node)
{
    if (!node) return;

    release(node->right);
    release(node->left);
    delete node;
}

template<class T, class S>
S *AVLTree<T, S>::find(const T *key)
{
    AVLTreeNode<T, S> *node = findNode(key, this->root);
    if(node == nullptr) return nullptr;
    return node->value;
}

template<class T, class S>
S *AVLTree<T, S>::findNext(const T *key)
{
    AVLTreeNode<T, S>* curNode = findNode(key, root);
    if(curNode == nullptr)
        throw KeyDoesNotExist();
    AVLTreeNode<T, S>* tempNode = curNode;
    if(curNode->right != nullptr)
    {
        curNode = curNode->right;
        while(curNode->left != nullptr)
        {
            curNode = curNode->left;
        }
        return curNode->value;
    }
    curNode = tempNode;
    while(curNode->getSonType() == SonType::RIGHT)
    {
        curNode = curNode->parent;
    }
    if(curNode->getSonType() == SonType::LEFT)
    {
        return curNode->parent->value;
    }
    return nullptr;
}

template<class T, class S>
S *AVLTree<T, S>::findPrevious(const T *key)
{
    AVLTreeNode<T, S>* curNode = findNode(key, root);
    if(curNode == nullptr)
        throw KeyDoesNotExist();
    AVLTreeNode<T, S>* tempNode = curNode;
    if(curNode->left != nullptr)
    {
        curNode = curNode->left;
        while(curNode->right != nullptr)
        {
            curNode = curNode->right;
        }
        return curNode->value;
    }
    curNode = tempNode;
    while(curNode->getSonType() == SonType::LEFT)
    {
        curNode = curNode->parent;
    }
    if(curNode->getSonType() == SonType::RIGHT)
    {
        return curNode->parent->value;
    }
    return nullptr;
}

template<class T, class S>
S *AVLTree<T, S>::findEqOrGreater(const T *key)
{
    if (root == nullptr)
        return nullptr;

    AVLTreeNode<T, S>* node = findNode(key, root);
    if(node != nullptr)
        return node->value;
    node = findParentNode(key, root);
    if(*key < *(node->key))
        return node->value;

    return findNext(node->key);
}

template<class T, class S>
AVLTreeNode<T, S> *AVLTree<T, S>::findParentNode(const T *key, AVLTreeNode<T, S> *node)
{
    if (*key < *(node->key))
    {
        if(node->left == nullptr)
            return node;
        return findParentNode(key, node->left);
    }

    if (*key > *(node->key))
    {
        if(node->right == nullptr)
            return node;
        return findParentNode(key, node->right);
    }

    return nullptr; //shouldn't get here anyways.
}

template<class T, class S>
AVLTreeNode<T, S> *AVLTree<T, S>::findNode(const T *key, AVLTreeNode<T, S> *node)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (*key < *(node->key))
    {
        return findNode(key, node->left);
    }

    if (*key > *(node->key))
    {
        return findNode(key, node->right);
    }

    return node;
}

template<class T, class S>
void AVLTree<T, S>::insert(T *key, S *value)
{
    if (find(key) != nullptr)
    {
        throw KeyExists();
    }
    AVLTreeNode<T, S> *newNode;
    newNode = new AVLTreeNode<T, S>(key, value);
    if (this->root == nullptr)
    {
        root = newNode;
    }
    else
    {
        insertBin(newNode, root);
    }

    balanceInsert(newNode);
}


template<class T, class S>
void AVLTree<T, S>::insertBin(AVLTreeNode<T, S> *newNode, AVLTreeNode<T, S> *nodeRec)
{
    if (*(newNode->key) < *(nodeRec->key))
    {
        if (nodeRec->left == nullptr)
        {
            nodeRec->left = newNode;
            newNode->parent = nodeRec;
        }
        else
        {
            insertBin(newNode, nodeRec->left);
        }

    }
    if (*(newNode->key) > *(nodeRec->key))
    {
        if (nodeRec->right == nullptr)
        {
            nodeRec->right = newNode;
            newNode->parent = nodeRec;
        }
        else
        {
            insertBin(newNode, nodeRec->right);
        }
    }

}

template<class T, class S>
void AVLTree<T, S>::balanceInsert(AVLTreeNode<T, S> *curNode)
{
    while (curNode != this->root)
    {
        AVLTreeNode<T, S> *parent = curNode->parent;
        if (parent->height >= curNode->height + 1)
        {
            break;
        }
        parent->height = curNode->height + 1;
        bool isBalanced = balance(parent);
        if (isBalanced) break;
        curNode = parent;
    }
}

template<class T, class S>
bool AVLTree<T, S>::balance(AVLTreeNode<T, S> *parent)
{
    if (parent->balanceFactor() == 2)
    {

        if (parent->left->balanceFactor() == -1)
        {
            rotateLR(parent);
        }
        else
        {
            rotateLL(parent);
        }

        return true;
    }
    if (parent->balanceFactor() == -2)
    {
        if (parent->right->balanceFactor() == 1)
        {
            rotateRL(parent);
        }
        else
        {
            rotateRR(parent);
        }

        return true;
    }
    return false;
}

template<class T, class S>
void AVLTree<T, S>::remove(T *key)
{
    AVLTreeNode<T, S> *toDelete = findNode(key, root);
    if (toDelete == nullptr)
    {
        throw KeyDoesNotExist();
    }
    toDelete = removeBin(toDelete);
    balanceRemove(toDelete->parent);
    delete toDelete;
}

template<class T, class S>
AVLTreeNode<T, S> *AVLTree<T, S>::removeBin(AVLTreeNode<T, S> *toRemove)
{
    if (!(toRemove->left || toRemove->right))
    {
        updateParent(toRemove, nullptr, toRemove->getSonType());
    }
    else if (!toRemove->right)
    {
        updateParent(toRemove, toRemove->left, toRemove->getSonType());
        toRemove->left->parent = toRemove->parent;
    }
    else if (!toRemove->left)
    {
        updateParent(toRemove, toRemove->right, toRemove->getSonType());
        toRemove->right->parent = toRemove->parent;
    }
    else
    {
        AVLTreeNode<T, S> *toSwap = toRemove->right;
        while (toSwap->left != nullptr)
        {
            toSwap = toSwap->left;
        }
        swapNodes(toRemove, toSwap);
        return removeBin(toSwap);
    }
    return toRemove;
}

template<class T, class S>
void AVLTree<T, S>::swapNodes(AVLTreeNode<T, S> *node1, AVLTreeNode<T, S> *node2)
{
    T *tempKey = node1->key;
    S *tempValue = node1->value;
    node1->key = node2->key;
    node1->value = node2->value;
    node2->key = tempKey;
    node2->value = tempValue;
}

template<class T, class S>
void AVLTree<T, S>::balanceRemove(AVLTreeNode<T, S> *curNode)
{
    int previousHeight;
    while (curNode != nullptr)
    {
        previousHeight = curNode->height;
        curNode->updateHeight();
        balance(curNode);
        if (previousHeight == curNode->height) break;
        curNode = curNode->parent;
    }
}


template<class T, class S>
void AVLTree<T, S>::rotateLL(AVLTreeNode<T, S> *parent)
{
    enum SonType sonType = parent->getSonType();
    AVLTreeNode<T, S> *B = parent;
    AVLTreeNode<T, S> *A = parent->left;
    AVLTreeNode<T, S> *Ar = A->right;
    B->left = Ar;
    if (Ar != nullptr)
    {
        Ar->parent = B;
    }
    A->right = B;
    A->parent = B->parent;
    B->parent = A;
    updateParent(A, A, sonType);
    B->updateHeight();
    A->updateHeight();
}

template<class T, class S>
void AVLTree<T, S>::rotateLR(AVLTreeNode<T, S> *parent)
{
    enum SonType sonType = parent->getSonType();
    AVLTreeNode<T, S> *C = parent;
    AVLTreeNode<T, S> *A = parent->left;
    AVLTreeNode<T, S> *B = A->right;
    AVLTreeNode<T, S> *Br = B->right;
    AVLTreeNode<T, S> *Bl = B->left;
    B->parent = C->parent;
    C->left = Br;
    if (Br != nullptr)
    {
        Br->parent = C;
    }
    A->right = Bl;
    if (Bl != nullptr)
    {
        Bl->parent = A;
    }
    B->left = A;
    A->parent = B;
    B->right = C;
    C->parent = B;
    updateParent(B, B, sonType);
    A->updateHeight();
    C->updateHeight();
    B->updateHeight();
}

template<class T, class S>
void AVLTree<T, S>::rotateRR(AVLTreeNode<T, S> *parent)
{
    enum SonType sonType = parent->getSonType();
    AVLTreeNode<T, S> *B = parent;
    AVLTreeNode<T, S> *A = parent->right;
    AVLTreeNode<T, S> *Al = A->left;
    B->right = Al;
    if (Al != nullptr)
    {
        Al->parent = B;
    }
    A->left = B;
    A->parent = B->parent;
    B->parent = A;
    updateParent(A, A, sonType);
    B->updateHeight();
    A->updateHeight();
}

template<class T, class S>
void AVLTree<T, S>::rotateRL(AVLTreeNode<T, S> *parent)
{
    enum SonType sonType = parent->getSonType();
    AVLTreeNode<T, S> *C = parent;
    AVLTreeNode<T, S> *A = parent->right;
    AVLTreeNode<T, S> *B = A->left;
    AVLTreeNode<T, S> *Br = B->right;
    AVLTreeNode<T, S> *Bl = B->left;
    B->parent = C->parent;
    C->right = Bl;
    if (Bl != nullptr)
    {
        Bl->parent = C;
    }
    A->left = Br;
    if (Br != nullptr)
    {
        Br->parent = A;
    }
    B->right = A;
    A->parent = B;
    B->left = C;
    C->parent = B;
    updateParent(B, B, sonType);
    A->updateHeight();
    C->updateHeight();
    B->updateHeight();
}

template<class T, class S>
void AVLTree<T, S>::updateParent(AVLTreeNode<T, S> *node, AVLTreeNode<T, S> *toUpdate, enum SonType sonType)
{
    if (sonType == SonType::ROOT)
    {
        this->root = toUpdate;
    }
    else if (sonType == SonType::RIGHT)
    {
        node->parent->right = toUpdate;
    }
    else
    {
        node->parent->left = toUpdate;
    }
}

template<class T, class S>
void AVLTree<T, S>::arrayInOrder(S **const output)
{
    arrayInOrderRecursive(output,root, 0);
}

template<class T, class S>
int AVLTree<T, S>::arrayInOrderRecursive(S **const output, AVLTreeNode<T, S>* curNode, int offset)
{
    if (curNode == nullptr) return offset;
    offset = arrayInOrderRecursive(output, curNode->left, offset);
    *(output + offset++) = curNode->value;
    offset = arrayInOrderRecursive(output, curNode->right, offset);
    return offset;
}

template<class T, class S>
void AVLTree<T, S>::releaseValues()
{
    releaseValuesRecursive(root);
}

template<class T, class S>
void AVLTree<T, S>::releaseValuesRecursive(AVLTreeNode<T,S>* curNode)
{
    if(curNode == nullptr) return;
    releaseValuesRecursive(curNode->left);
    delete curNode->value;
    releaseValuesRecursive(curNode->right);
}

template<class T, class S>
S *AVLTree<T, S>::findMax()
{
    if (root == nullptr)
        return nullptr;
    AVLTreeNode<T, S> *curNode = root;
    while (curNode->right != nullptr)
        curNode = curNode->right;
    return curNode->value;
}


#endif //AVL_TREE_H_