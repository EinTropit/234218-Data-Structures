
#ifndef DATASTRUCTURESWET2_UNIONFIND_H
#define DATASTRUCTURESWET2_UNIONFIND_H

#include "Hash.h"
#include "Player.h"

class UnionFind
{
public:
    static Player* find(Player* player);
    static Player* unite(Player *buyingRoot, Player *boughtRoot);
};


#endif //DATASTRUCTURESWET2_UNIONFIND_H
