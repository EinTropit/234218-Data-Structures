#include "Team.h"

Team::Team(int id, int points) :
    id(id), points(points), matchScore(points), playerCount(0), goalKeeperCount(0),
    players(new AVLTree<int, Player>), playersSorted(new AVLTree<Player, Node<Player>>),
    topScorer(nullptr), teamGamesPlayed(0)
{}

Team::~Team()
{
    delete players;
    delete playersSorted;
}

bool Team::isLegal() const
{
	if (goalKeeperCount > 0 && playerCount >= MIN_PLAYER_AMOUNT)
	{
		return true;
	}
	return false;
}

// Getter and Setters ---------------------------------------------------------------

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

void Team::updatePoints(int amount)
{
    points += amount;
    matchScore += amount;
}

int Team::getMatchScore() const
{
    return matchScore;
}

void Team::updateMatchScore(int amount)
{
    matchScore += amount;
}

int Team::getPlayerCount() const
{
    return playerCount;
}

void Team::updatePlayerCount(int amount)
{
    playerCount += amount;
}

int Team::getGoalKeeperCount() const
{
    return goalKeeperCount;
}

void Team::updateGoalKeeperCount(int amount)
{
    goalKeeperCount += amount;
}

AVLTree<int, Player> *Team::getPlayers()
{
    return players;
}

void Team::setPlayers(AVLTree<int, Player>* tree)
{
    delete players;
    players = tree;
}

AVLTree<Player, Node<Player>> *Team::getPlayersSorted()
{
    return playersSorted;
}

void Team::setPlayersSorted(AVLTree<Player, Node<Player>>* tree)
{
    delete playersSorted;
    playersSorted = tree;
}

Player *Team::getTopScorer() const
{
    return topScorer;
}

void Team::setTopScorer(Player *newTopScorer)
{
    this->topScorer = newTopScorer;
}

int Team::getTeamGamesPlayed() const
{
    return teamGamesPlayed;
}

void Team::updateTeamGamesPlayed()
{
    teamGamesPlayed += 1;
}

void Team::resetTeamGamesPlayed()
{
    teamGamesPlayed = 0;
}

// End of Getter and Setters ---------------------------------------------------------------