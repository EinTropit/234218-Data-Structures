#include "worldcup23a1.h"

world_cup_t::world_cup_t()
{
    topScorer = nullptr;
    playerCount = 0;
}

world_cup_t::~world_cup_t()
{
    players.releaseValues();
    playersSorted.releaseValues();
    teams.releaseValues();
    playableTeams.releaseValues();
}

StatusType world_cup_t::add_team(int teamId, int points)
{
    if ((teamId <= 0) || (points < 0))
    {
        return StatusType::INVALID_INPUT;
    }
    if (teams.find(&teamId) != nullptr)
    {
        return StatusType::FAILURE;
    }

    Team *newTeam;

    try
    {
        newTeam = new Team(teamId, points);
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    int *newKey = newTeam->getIdPtr();
    try
    {
        teams.insert(newKey, newTeam);
    }
    catch (const std::bad_alloc &e)
    {
        delete newTeam;
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    Team *team = teams.find(&teamId);
    if (team == nullptr) return StatusType::FAILURE;
    if (team->getPlayerCount() != 0) return StatusType::FAILURE;

    teams.remove(&teamId);
    delete team;

    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    if ((playerId <= 0) || (teamId <= 0) || (gamesPlayed < 0) || (goals < 0) || (cards < 0))
    {
        return StatusType::INVALID_INPUT;
    }
    if ((gamesPlayed == 0) && ((goals > 0) || (cards > 0)))
    {
        return StatusType::INVALID_INPUT;
    }

    Team *team = teams.find(&teamId);
    if ((players.find(&playerId) != nullptr) || (team == nullptr))
    {
        return StatusType::FAILURE;
    }
    Player *newPlayer;
    Node<Player> *newPlayerNode;

    try
    {
        newPlayer = new Player(playerId, goals, cards, (gamesPlayed - team->getTeamGamesPlayed()), goalKeeper, team);
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    int *newKey = newPlayer->getIdPtr();
    try
    {
        newPlayerNode = new Node<Player>(newPlayer);
    }
    catch (const std::bad_alloc &e)
    {
        delete newPlayer;
        return StatusType::ALLOCATION_ERROR;
    }

    try
    {
        players.insert(newKey, newPlayer);
        playersSorted.insert(newPlayer, newPlayerNode);
        team->getPlayers()->insert(newKey, newPlayer);
        team->getPlayersSorted()->insert(newPlayer, newPlayerNode);
    }
    catch (const std::bad_alloc &e)
    {
        delete newPlayer;
        delete newPlayerNode;
        return StatusType::ALLOCATION_ERROR;
    }
    playerCount++;
    topScorer = playersSorted.findMax()->value;
    listInsert(newPlayerNode);

    team->updatePlayerCount(1);
    if (goalKeeper)
    {
        team->updateGoalKeeperCount(1);
    }

    team->updateMatchScore(goals - cards);
    team->setTopScorer(team->getPlayersSorted()->findMax()->value);

    if (team->isLegal() && (playableTeams.find(&teamId) == nullptr))
    {
        Node<Team> *teamNode;
        try
        {
            teamNode = new Node<Team>(team);
        }
        catch (const std::bad_alloc &e)
        {
            delete newPlayer;
            delete newPlayerNode;
            return StatusType::ALLOCATION_ERROR;
        }
        try
        {
            playableTeams.insert(team->getIdPtr(), teamNode);
        }
        catch (const std::bad_alloc &e)
        {
            delete newPlayer;
            delete newPlayerNode;
            delete teamNode;
            return StatusType::ALLOCATION_ERROR;
        }

        listInsert(teamNode);
    }

    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
    if (playerId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    Player *player = players.find(&playerId);
    if (player == nullptr) return StatusType::FAILURE;

    Node<Player> *playerNode = playersSorted.find(player);
    Team *team = player->getTeam();

    players.remove(&playerId);
    playersSorted.remove(player);
    team->getPlayers()->remove(&playerId);
    team->getPlayersSorted()->remove(player);

    playerCount--;
    Node<Player> *topNode = playersSorted.findMax();
    if (topNode == nullptr)
        topScorer = nullptr;
    else
        topScorer = topNode->value;
    listRemove(playerNode);

    team->updatePlayerCount(-1);
    if (player->getIsGoalKeeper())
        team->updateGoalKeeperCount(-1);
    team->updateMatchScore(-(player->getGoals() - player->getCards()));

    topNode = team->getPlayersSorted()->findMax();
    if (topNode == nullptr)
        team->setTopScorer(nullptr);
    else
        team->setTopScorer(topNode->value);

    Node<Team> *teamNode = playableTeams.find(team->getIdPtr());
    if ((!team->isLegal()) && (teamNode != nullptr))
    {
        playableTeams.remove(team->getIdPtr());
        listRemove(teamNode);
        delete teamNode;
    }

    delete playerNode;
    delete player;

    return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                            int scoredGoals, int cardsReceived)
{
    if ((playerId <= 0) || (gamesPlayed < 0) || (scoredGoals < 0) || (cardsReceived < 0))
    {
        return StatusType::INVALID_INPUT;
    }

    Player *player = players.find(&playerId);
    if (player == nullptr) return StatusType::FAILURE;

    Team *team = player->getTeam();
    Node<Player> *playerNode = playersSorted.find(player);

    listRemove(playerNode);
    playersSorted.remove(player);
    team->getPlayersSorted()->remove(player);

    player->updateStats(gamesPlayed, scoredGoals, cardsReceived);
    team->updateMatchScore(scoredGoals - cardsReceived);

    try
    {
        playersSorted.insert(player, playerNode);
        team->getPlayersSorted()->insert(player, playerNode);
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    listInsert(playerNode);
    topScorer = playersSorted.findMax()->value;
    team->setTopScorer(team->getPlayersSorted()->findMax()->value);

    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if ((teamId1 <= 0) || (teamId2 <= 0) || (teamId1 == teamId2))
    {
        return StatusType::INVALID_INPUT;
    }

    Team *team1 = teams.find(&teamId1);
    Team *team2 = teams.find(&teamId2);

    if ((team1 == nullptr) || (team2 == nullptr)) return StatusType::FAILURE;
    if (!(team1->isLegal() && team2->isLegal())) return StatusType::FAILURE;

    if (team1->getMatchScore() > team2->getMatchScore())
    {
        team1->updatePoints(3);
    }
    else if (team1->getMatchScore() < team2->getMatchScore())
    {
        team2->updatePoints(3);
    }
    else
    {
        team1->updatePoints(1);
        team2->updatePoints(1);
    }

    team1->updateTeamGamesPlayed();
    team2->updateTeamGamesPlayed();

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if (playerId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    Player *player = players.find(&playerId);
    if (player == nullptr) return StatusType::FAILURE;

    int res = player->getGamesPlayed();
    return res;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    Team *team = teams.find(&teamId);
    if (team == nullptr) return StatusType::FAILURE;

    return team->getPoints();
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    if (newTeamId <= 0 || teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
        return StatusType::INVALID_INPUT;

    Team *team1 = teams.find(&teamId1);
    Team *team2 = teams.find(&teamId2);
    Team *newTeam = teams.find(&newTeamId);

    if (team1 == nullptr || team2 == nullptr)
    {
        return StatusType::FAILURE;
    }
    if (newTeam != nullptr && newTeamId != teamId1 && newTeamId != teamId2)
    {
        return StatusType::FAILURE;
    }

    try
    {
        newTeam = new Team(newTeamId, team1->getPoints() + team2->getPoints());
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    StatusType tatakae1 = mergePlayers(team1, team2, newTeam);
    StatusType tatakae2 = mergePlayersSorted(team1, team2, newTeam);
    if(tatakae1 == StatusType::ALLOCATION_ERROR || tatakae2 == StatusType::ALLOCATION_ERROR )
    {
        delete newTeam;
        return StatusType::ALLOCATION_ERROR;
    }

    teams.remove(&teamId1);
    teams.remove(&teamId2);
    teams.insert(newTeam->getIdPtr(), newTeam);

    Node<Team> *team1Node = playableTeams.find(&teamId1);
    Node<Team> *team2Node = playableTeams.find(&teamId2);
    if (team1Node != nullptr)
    {
        playableTeams.remove(&teamId1);
        listRemove(team1Node);
    }
    if (team2Node != nullptr)
    {
        playableTeams.remove(&teamId2);
        listRemove(team2Node);
    }

    if (newTeam->isLegal())
    {
        Node<Team> *newTeamNode;
        try
        {
            newTeamNode = new Node<Team>(newTeam);
        }
        catch (const std::bad_alloc &e)
        {
            delete newTeam;
            return StatusType::ALLOCATION_ERROR;
        }

        playableTeams.insert(newTeam->getIdPtr(), newTeamNode);
        listInsert(newTeamNode);
    }

    delete team1;
    delete team2;
    delete team1Node;
    delete team2Node;

    return StatusType::SUCCESS;
}


output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if (teamId > 0)
    {
        Team *team = teams.find(&teamId);
        if (team == nullptr) return StatusType::FAILURE;
        if (team->getPlayerCount() == 0) return StatusType::FAILURE;
        return team->getTopScorer()->getId();
    }

    if (teamId < 0)
    {
        if (playerCount == 0) return StatusType::FAILURE;
        return topScorer->getId();
    }

    return StatusType::INVALID_INPUT;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if (teamId > 0)
    {
        Team *team = teams.find(&teamId);
        if (team == nullptr) return StatusType::FAILURE;
        return team->getPlayerCount();
    }

    if (teamId < 0)
    {
        return playerCount;
    }

    return StatusType::INVALID_INPUT;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{

    if ((teamId == 0) || (output == nullptr))
    {
        return StatusType::INVALID_INPUT;
    }

    Node<Player>** playersArr;


    if (teamId > 0)
    {
        Team *team = teams.find(&teamId);
        if (team == nullptr) return StatusType::FAILURE;
        if (team->getPlayerCount() == 0) return StatusType::FAILURE;

        try
        {
            playersArr = new Node<Player>*[team->getPlayerCount()];
        }
        catch (const std::bad_alloc &e)
        {
            return StatusType::ALLOCATION_ERROR;
        }
        team->getPlayersSorted()->arrayInOrder(playersArr);
        for (int i = 0; i < team->getPlayerCount(); ++i)
        {
            output[i] = playersArr[i]->value->getId();
        }
    }
    else
    {
        if(playerCount == 0) return StatusType::FAILURE;

        try
        {
            playersArr = new Node<Player>*[playerCount];
        }
        catch (const std::bad_alloc &e)
        {
            return StatusType::ALLOCATION_ERROR;
        }
        playersSorted.arrayInOrder(playersArr);
        for (int i = 0; i < playerCount; ++i)
        {
            output[i] = playersArr[i]->value->getId();
        }
    }

    delete[] playersArr;
    return StatusType::SUCCESS;

}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    if ((teamId <= 0) || (playerId <= 0))
    {
        return StatusType::INVALID_INPUT;
    }

    if (playerCount <= 1) return StatusType::FAILURE;

    Team *team = teams.find(&teamId);
    if (team == nullptr) return StatusType::FAILURE;

    Player *player = team->getPlayers()->find(&playerId);
    if (player == nullptr) return StatusType::FAILURE;
    Node<Player> *playerNode = team->getPlayersSorted()->find(player);

    Player *next;
    Player *previous;

    if(playerNode->next == nullptr)
    {
        return playerNode->previous->value->getId();
    }
    else
    {
        next = playerNode->next->value;
    }

    if(playerNode->previous == nullptr)
    {
        return playerNode->next->value->getId();
    }
    else
    {
        previous = playerNode->previous->value;
    }

    if (abs(player->getGoals() - next->getGoals()) < abs(player->getGoals() - previous->getGoals()))
    {
        return next->getId();
    }
    else if (abs(player->getGoals() - next->getGoals()) == abs(player->getGoals() - previous->getGoals()))
    {
        if (abs(player->getCards() - next->getCards()) < abs(player->getCards() - previous->getCards()))
        {
            return next->getId();
        }
        else if (abs(player->getCards() - next->getCards()) == abs(player->getCards() - previous->getCards()))
        {
            if (abs(player->getId() - next->getId()) < abs(player->getId() - previous->getId()))
            {
                return next->getId();
            }
            else if (abs(player->getId() - next->getId()) == abs(player->getId() - previous->getId()))
            {
                if (next->getId() > previous->getId())
                {
                    return next->getId();
                }
            }
        }
    }
    return previous->getId();
}


output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    if ((minTeamId < 0) || (maxTeamId < 0) || (minTeamId > maxTeamId))
    {
        return StatusType::INVALID_INPUT;
    }

    Node<Team> *minTeam = playableTeams.findEqOrGreater(&minTeamId);
    Node<Team> *tempNode = minTeam;

    int teamCount = 0;
    while (tempNode != nullptr)
    {
        if (tempNode->value->getId() > maxTeamId) break;
        teamCount++;
        tempNode = tempNode->next;
    }

    if (teamCount == 0) return StatusType::FAILURE;

    Pair *tournament;
    try
    {
        tournament = new Pair[teamCount];
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    tempNode = minTeam;
    Team *team;
    for (int i = 0; i < teamCount; i++)
    {
        team = tempNode->value;
        tournament[i] = Pair(team->getId(), team->getMatchScore());
        tempNode = tempNode->next;
    }

    for (int i = 1; i < teamCount; i *= 2)
    {
        for (int j = 0; j < teamCount; j += (2 * i))
        {
            if ((j + i) < teamCount)
            {
                simulateMatch(tournament[j], tournament[j + i]);
            }
        }
    }


    Pair winner = tournament[0];
    delete[] tournament;

    return winner.first;
}


/* -------------------------Private Functions---------------------*/

int world_cup_t::abs(int a)
{
    if (a >= 0) return a;
    return -a;
}

void world_cup_t::listInsert(Node<Player> *playerNode)
{
    Node<Player> *next = playersSorted.findNext(playerNode->value);
    Node<Player> *previous = playersSorted.findPrevious(playerNode->value);

    playerNode->next = next;
    playerNode->previous = previous;
    if (next != nullptr)
    {
        next->previous = playerNode;
    }
    if (previous != nullptr)
    {
        previous->next = playerNode;
    }
}

void world_cup_t::listRemove(Node<Player> *playerNode)
{
    Node<Player> *next = playerNode->next;
    Node<Player> *previous = playerNode->previous;

    if (next != nullptr)
    {
        next->previous = previous;
    }
    if (previous != nullptr)
    {
        previous->next = next;
    }
}

void world_cup_t::listInsert(Node<Team> *teamNode)
{
    Node<Team> *next = playableTeams.findNext(teamNode->value->getIdPtr());
    Node<Team> *previous = playableTeams.findPrevious(teamNode->value->getIdPtr());

    teamNode->next = next;
    teamNode->previous = previous;
    if (next != nullptr)
    {
        next->previous = teamNode;
    }
    if (previous != nullptr)
    {
        previous->next = teamNode;
    }
}

void world_cup_t::listRemove(Node<Team> *teamNode)
{
    Node<Team> *next = teamNode->next;
    Node<Team> *previous = teamNode->previous;

    if (next != nullptr)
    {
        next->previous = previous;
    }
    if (previous != nullptr)
    {
        previous->next = next;
    }
}


void world_cup_t::simulateMatch(Pair &team1, Pair &team2)
{
    if (team1.second <= team2.second)
    {
        team1.first = team2.first;
    }
    team1.second += (team2.second + 3);
}

StatusType world_cup_t::mergePlayers(Team *team1, Team *team2, Team *newTeam)
{
    int team1Size = team1->getPlayerCount();
    int team2Size = team2->getPlayerCount();
    Player **team1Arr;
    Player **team2Arr;
    Player **newTeamArr;

    try
    {
        team1Arr = new Player *[team1Size];
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    try
    {
        team2Arr = new Player *[team2Size];
    }
    catch (const std::bad_alloc &e)
    {
        delete[] team1Arr;
        return StatusType::ALLOCATION_ERROR;
    }
    try
    {
        newTeamArr = new Player *[team1Size + team2Size];
    }
    catch (const std::bad_alloc &e)
    {
        delete[] team1Arr;
        delete[] team2Arr;
        return StatusType::ALLOCATION_ERROR;
    }

    team1->getPlayers()->arrayInOrder(team1Arr);
    team2->getPlayers()->arrayInOrder(team2Arr);

    newTeam->resetTeamGamesPlayed();
    newTeam->updateGoalKeeperCount(team1->getGoalKeeperCount() + team2->getGoalKeeperCount());
    newTeam->updatePlayerCount(team1->getPlayerCount() + team2->getPlayerCount());

    Player *curPlayer;
    for(int i = 0; i < team1Size; i++)
    {
        curPlayer = team1Arr[i];
        curPlayer->updateStats(team1->getTeamGamesPlayed(),0,0);
        curPlayer->setTeam(newTeam);
        newTeam->updateMatchScore(curPlayer->getGoals() - curPlayer->getCards());
    }
    for(int i = 0; i < team2Size; i++)
    {
        curPlayer = team2Arr[i];
        curPlayer->updateStats(team2->getTeamGamesPlayed(),0,0);
        curPlayer->setTeam(newTeam);
        newTeam->updateMatchScore(curPlayer->getGoals() - curPlayer->getCards());
    }

    int o1 = 0, o2 = 0, oNew = 0;

    while (true)
    {
        if (o1 < team1Size && o2 < team2Size)
        {
            if (team1Arr[o1]->getId() < team2Arr[o2]->getId())
            {
                newTeamArr[oNew++] = team1Arr[o1++];
            }
            else
            {
                newTeamArr[oNew++] = team2Arr[o2++];
            }
        }
        else if (o1 < team1Size)
        {
            newTeamArr[oNew++] = team1Arr[o1++];
        }
        else if (o2 < team2Size)
        {
            newTeamArr[oNew++] = team2Arr[o2++];
        }
        else break;
    }

    newTeam->setPlayers(new AVLTree<int, Player>(newTeamArr, team1Size+team2Size,(int *(Player::*)() const) &Player::getIdPtr));

    delete[] team1Arr;
    delete[] team2Arr;
    delete[] newTeamArr;


    return StatusType::SUCCESS;

}

StatusType world_cup_t::mergePlayersSorted(Team *team1, Team *team2, Team *newTeam)
{
    int team1Size = team1->getPlayerCount();
    int team2Size = team2->getPlayerCount();
    Node<Player> **team1Arr;
    Node<Player> **team2Arr;
    Node<Player> **newTeamArr;

    try
    {
        team1Arr = new Node<Player> *[team1Size];
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    try
    {
        team2Arr = new Node<Player> *[team2Size];
    }
    catch (const std::bad_alloc &e)
    {
        delete[] team1Arr;
        return StatusType::ALLOCATION_ERROR;
    }
    try
    {
        newTeamArr = new Node<Player> *[team1Size + team2Size];
    }
    catch (const std::bad_alloc &e)
    {
        delete[] team1Arr;
        delete[] team2Arr;
        return StatusType::ALLOCATION_ERROR;
    }

    team1->getPlayersSorted()->arrayInOrder(team1Arr);
    team2->getPlayersSorted()->arrayInOrder(team2Arr);

    int o1 = 0, o2 = 0, oNew = 0;

    while (true)
    {
        if (o1 < team1Size && o2 < team2Size)
        {
            if (*(team1Arr[o1]->value) < *(team2Arr[o2]->value))
            {
                newTeamArr[oNew++] = team1Arr[o1++];
            }
            else
            {
                newTeamArr[oNew++] = team2Arr[o2++];
            }
        }
        else if (o1 < team1Size)
        {
            newTeamArr[oNew++] = team1Arr[o1++];
        }
        else if (o2 < team2Size)
        {
            newTeamArr[oNew++] = team2Arr[o2++];
        }
        else break;
    }


    newTeam->setPlayersSorted(new AVLTree<Player, Node<Player>>(newTeamArr, team1Size+team2Size,(Player *(Node<Player>::*)() const) &Node<Player>::getValue));
    Node<Player> *topNode = newTeam->getPlayersSorted()->findMax();
    if (topNode == nullptr)
        newTeam->setTopScorer(nullptr);
    else
        newTeam->setTopScorer(topNode->value);

    delete[] team1Arr;
    delete[] team2Arr;
    delete[] newTeamArr;

    return StatusType::SUCCESS;

}

