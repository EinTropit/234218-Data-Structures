
#include "UnionFind.h"

Player *UnionFind::find(Player *player)
{
    Player* cur = player;
    int sumGM = 0, toSubGM = 0;
    permutation_t sumSpirit = permutation_t::neutral(), toSubSpirit = permutation_t::neutral();
    while(!(cur->isRoot))  //root finding
    {
        sumGM += cur->gamesPlayed;
        sumSpirit = cur->spirit * sumSpirit;
        cur = cur->parent;
    }
    Player* root = cur;
    cur = player;
    while(!(cur->isRoot)) //path shortening
    {

        Player* temp = cur;
        cur = cur->parent;
        temp->parent = root;
        int tempGM = temp->gamesPlayed;
        permutation_t tempS = temp->spirit;
        temp->gamesPlayed = sumGM - toSubGM;
        temp->spirit = sumSpirit * toSubSpirit.inv();
        toSubGM += tempGM;
        toSubSpirit = tempS * toSubSpirit;
    }
    return root;
}

Player *UnionFind::unite(Player *buyingRoot, Player *boughtRoot)
{
    if(buyingRoot->size >= boughtRoot->size)
    {
        boughtRoot->isRoot = false;
        boughtRoot->parent = buyingRoot;
        buyingRoot->size += boughtRoot->size;
        boughtRoot->team = nullptr;

        boughtRoot->gamesPlayed -= buyingRoot->gamesPlayed;
        boughtRoot->spirit = buyingRoot->spirit.inv() * buyingRoot->team->getTeamSpirit() * boughtRoot->spirit;

        return buyingRoot;
    }

    buyingRoot->isRoot = false;
    buyingRoot->parent = boughtRoot;
    boughtRoot->size += buyingRoot->size;
    boughtRoot->team = buyingRoot->team;
    buyingRoot->team->setTeamSet(boughtRoot);

    buyingRoot->gamesPlayed -= boughtRoot->gamesPlayed;
    boughtRoot->spirit = buyingRoot->team->getTeamSpirit() * boughtRoot->spirit;
    buyingRoot->spirit = boughtRoot->spirit.inv() * buyingRoot->spirit;

    buyingRoot->team = nullptr;

    return boughtRoot;

}


