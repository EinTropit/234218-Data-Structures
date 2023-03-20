
#include "Player.h"


Player::Player(int id, int cards, int gamesPlayed,int ability,  bool isGoalKeeper, const permutation_t &spirit, Team *team) :
        id(id), cards(cards), gamesPlayed(gamesPlayed), ability(ability), isGoalKeeper(isGoalKeeper), spirit(spirit),
        isRoot(true), parent(nullptr), team(team), size(1)
{}


// Getter and Setters ---------------------------------------------------------------



int Player::getId() const
{
    return id;
}

int Player::getCards() const
{
    return cards;
}

bool Player::getIsGoalKeeper() const
{
    return isGoalKeeper;
}

void Player::updateGamesPlayed(int amount)
{
    gamesPlayed += amount;
}

void Player::updateCards(int amount)
{
    cards += amount;
}

permutation_t Player::getPartialSpirit()
{
    UnionFind::find(this); // for path shortening.

    if (this->isRoot)
        return this->spirit;

    return (this->parent->spirit * this->spirit);
}

void Player::setTeam(Team* newTeam)
{
    team = newTeam;
}

Team *Player::getTeam() const
{
    return team;
}

int Player::getGamesPlayed()
{
    UnionFind::find(this); // for path shortening.

    if (this->isRoot)
        return this->gamesPlayed;

    return this->gamesPlayed + this->parent->gamesPlayed;
}



// End of Getter and Setters ---------------------------------------------------------------
