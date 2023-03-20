#ifndef PLAYER_H_
#define PLAYER_H_
#include "Team.h"

class Team;

class Player
{
public:
    Player(int id, int goals, int cards, int gamesPlayed, bool isGoalKeeper, Team *team);

    /*
	 * Explicitly telling the compiler to use the default methods
	*/
    Player(const Player&) = delete;
    ~Player() = default;
    Player& operator=(const Player& other) = delete;


    /*
     * Getter and Setters
    */
	int getGamesPlayed();
	int getId() const;
	int* getIdPtr();
	int getGoals() const;
	int getCards() const;
	bool getIsGoalKeeper() const;
	Team* getTeam() const;
    void setTeam(Team *newTeam);
    void updateStats(int addGamesPlayed, int addGoals, int addCards);

    bool operator==(const Player &other) const;
    bool operator!=(const Player &other) const;
    bool operator<(const Player &other) const;
    bool operator>(const Player &other) const;

	
private:
	int id;
	int goals;
	int cards;
	int gamesPlayed;
	bool isGoalKeeper;
	Team *team;
};

#endif //PLAYER_H_
