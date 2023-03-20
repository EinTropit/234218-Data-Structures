#include "Player.h"

Player::Player(int id, int goals, int cards, int gamesPlayed, bool isGoalKeeper, Team *team) :
    id(id), goals(goals), cards(cards),gamesPlayed(gamesPlayed), isGoalKeeper(isGoalKeeper), team(team)
{}


// Getter and Setters ---------------------------------------------------------------


int Player::getGamesPlayed()
{
	return this->gamesPlayed + this->team->getTeamGamesPlayed();
}

int Player::getId() const
{
	return id;
}

int* Player::getIdPtr()
{
    return &id;
}

int Player::getGoals() const
{
	return goals;
}

int Player::getCards() const
{
	return cards;
}

bool Player::getIsGoalKeeper() const
{
	return isGoalKeeper;
}

Team* Player::getTeam() const
{
    return team;
}

void Player::setTeam(Team *newTeam)
{
    team = newTeam;
}

void Player::updateStats(int addGamesPlayed, int addGoals, int addCards)
{
    this->gamesPlayed += addGamesPlayed;
    this->goals += addGoals;
    this->cards += addCards;
}

// End of Getter and Setters ---------------------------------------------------------------

// Operators:
bool Player::operator==(const Player &other) const
{
    return (this->id == other.id);
}
bool Player::operator!=(const Player &other) const
{
    return !((*this) == other);
}
bool Player::operator<(const Player &other) const
{
    if (this->goals < other.goals)
    {
        return true;
    }
    if ((this->goals == other.goals) && (this->cards > other.cards))
    {
        return true;
    }
    if ((this->goals == other.goals) && (this->cards == other.cards))
    {
        return (this->id < other.id);
    }
    return false;
}
bool Player::operator>(const Player &other) const
{
    return (other < (*this));
}