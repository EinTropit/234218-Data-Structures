#ifndef TEAM_H_
#define TEAM_H_

#include "Player.h"
#include "AVLTree.h"
#include "NP_Util.h"

class Player;

class Team
{
public:
    Team(int id, int points);

    /*
	 * Explicitly telling the compiler to use the default methods
	*/
    Team(const Team&) = delete;
    ~Team();
    Team& operator=(const Team& other) = delete;

	//returns if the team is legal for playing
	bool isLegal() const;


    /*
    * Getter and Setters
    */
    int getId() const;
    int* getIdPtr();
    int getPoints() const;
    void updatePoints(int amount);
    int getMatchScore() const;
    void updateMatchScore(int amount);
    int getPlayerCount() const;
    void updatePlayerCount(int amount);
    int getGoalKeeperCount() const;
    void updateGoalKeeperCount(int amount);
    AVLTree<int, Player> *getPlayers();
    void setPlayers(AVLTree<int, Player>* tree);
    AVLTree<Player, Node<Player>> *getPlayersSorted();
    void setPlayersSorted(AVLTree<Player, Node<Player>>* tree);
    Player *getTopScorer() const;
    void setTopScorer(Player *newTopScorer);
    int getTeamGamesPlayed() const;
    void updateTeamGamesPlayed();
    void resetTeamGamesPlayed();


private:
	int id;
    int points;
	int matchScore;
	int playerCount;
	int goalKeeperCount;
    AVLTree<int, Player> *players;
    AVLTree<Player, Node<Player>> *playersSorted;
	Player *topScorer;
	int teamGamesPlayed;

    static const int MIN_PLAYER_AMOUNT = 11;
};

#endif //TEAM_H_
