
#ifndef DATASTRUCTURESWET2_TEAM_H
#define DATASTRUCTURESWET2_TEAM_H

#include "wet2util.h"
#include "Player.h"

class Player;

class Team
{
public:
    explicit Team(int id);

    /*
	 * Explicitly telling the compiler to use the default methods or delete them
	*/
    Team(const Team&) = delete;
    ~Team() = default;
    Team& operator=(const Team& other) = delete;

    //returns if the team is legal for playing
    bool isLegal() const;

    int getId() const;
    int* getIdPtr();
    int getPoints() const;
    int getTeamAbility() const;
    permutation_t& getTeamSpirit();
    Player* getTeamSet();

    void updatePoints(int amount);
    void updateAbility(int amount);
    void setTeamSet(Player *set);
    void updateTeamSpirit(const permutation_t &spirit);

    void updateHasGoalKeeper(bool gk);

    bool operator==(const Team &other) const;
    bool operator!=(const Team &other) const;
    bool operator<(const Team &other) const;
    bool operator>(const Team &other) const;

private:
    int id;
    int points;
    int teamAbility; // sum of all player's abilities and points
    bool hasGoalKeeper;
    permutation_t teamSpirit;
    Player *teamSet;


};


#endif //DATASTRUCTURESWET2_TEAM_H
