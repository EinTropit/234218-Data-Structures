#ifndef DATASTRUCTURES_NP_UTIL_H
#define DATASTRUCTURES_NP_UTIL_H

template <class T>
class Node
{
public:
    T* value;
    Node* previous;
    Node* next;

    Node() = default;
    explicit Node(T* value) : value(value) {}

    T* getValue()
    {
        return value;
    }
};

class Pair
{
public:
    int first;
    int second;

    Pair() = default;
    Pair(int first, int second) : first(first), second (second) {}
};

#endif //DATASTRUCTURES_NP_UTIL_H
