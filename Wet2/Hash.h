
#ifndef DATASTRUCTURESWET2_HASH_H
#define DATASTRUCTURESWET2_HASH_H

#include "exception"
#include "Player.h"
#include "Team.h"
#include "Node.h"

class Player;
class Team;

class Hash
{
private:
    int size;
    int arrSize;
    Node<Player>** players;

    const static int STARTING_SIZE = 16;

    int h(int playerID) const;

    void increaseSize();

public:
    Hash();

    ~Hash();
    Hash(const Hash&) = delete;
    Hash& operator=(const Hash&) = delete;

    void insert(Player* player);
    Player* find (int playerID);


    class KeyExists : public std::exception {};

};


#endif //DATASTRUCTURESWET2_HASH_H
