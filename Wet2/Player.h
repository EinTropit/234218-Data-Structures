#ifndef DATASTRUCTURESWET2_PLAYER_H
#define DATASTRUCTURESWET2_PLAYER_H

#include "Team.h"
#include "wet2util.h"
#include "UnionFind.h"

class Team;

class Player
{
    friend class UnionFind;
public:
    Player(int id, int cards, int gamesPlayed ,int ability,  bool isGoalKeeper, const permutation_t &spirit, Team *team);

    /*
	 * Explicitly telling the compiler to use the default methods or delete them
	*/
    Player(const Player&) = delete;
    ~Player() = default;
    Player& operator=(const Player& other) = delete;

    /*
     * Getter and Setters
    */
    int getId() const;
    int getCards() const;
    int getGamesPlayed();
    bool getIsGoalKeeper() const;
    permutation_t getPartialSpirit();
    Team* getTeam() const;


    void updateGamesPlayed(int amount);
    void updateCards(int amount);
    void setTeam(Team* newTeam);

private:
    int id;
    int cards;
    int gamesPlayed;
    int ability;
    bool isGoalKeeper;
    permutation_t spirit;
    bool isRoot;
    Player* parent;
    Team* team;
    int size;

};


#endif //DATASTRUCTURESWET2_PLAYER_H
