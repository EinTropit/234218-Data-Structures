
#include "Team.h"

Team::Team(int id) :
    id(id), points(0), teamAbility(0), hasGoalKeeper(false), teamSpirit(permutation_t::neutral()), teamSet(nullptr)
{}

bool Team::isLegal() const
{
    return hasGoalKeeper;
}

int Team::getId() const
{
    return id;
}

int* Team::getIdPtr()
{
    return &id;
}


int Team::getPoints() const
{
    return points;
}

int Team::getTeamAbility() const
{
    return teamAbility;
}

permutation_t &Team::getTeamSpirit()
{
    return teamSpirit;
}

Player *Team::getTeamSet()
{
    return teamSet;
}

void Team::updatePoints(int amount)
{
    this->points += amount;
}

void Team::updateAbility(int amount)
{
    this->teamAbility += amount;
}

void Team::setTeamSet(Player *set)
{
    teamSet = set;
}

void Team::updateTeamSpirit(const permutation_t &spirit)
{
    teamSpirit = teamSpirit * spirit;
}


void Team::updateHasGoalKeeper(bool gk)
{
    hasGoalKeeper = (hasGoalKeeper || gk);
}


bool Team::operator==(const Team &other) const
{
    return (this->id == other.id);
}

bool Team::operator!=(const Team &other) const
{
    return !((*this) == other);
}

bool Team::operator<(const Team &other) const
{
    if (this->teamAbility < other.teamAbility)
    {
        return true;
    }
    if (this->teamAbility == other.teamAbility)
    {
        return (this->id < other.id);
    }
    return false;
}

bool Team::operator>(const Team &other) const
{
    return (other < (*this));
}
