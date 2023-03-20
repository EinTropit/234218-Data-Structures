#ifndef DATASTRUCTURES_NODE_H
#define DATASTRUCTURES_NODE_H

template <class T>
class Node
{
public:
    T* value;
    Node* next;

    Node() = default;
    explicit Node(T* value) : value(value) {}

};

#endif //DATASTRUCTURES_NODE_H
