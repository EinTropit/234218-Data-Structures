#include "worldcup23a2.h"

world_cup_t::world_cup_t() : teamsById(), teamsByAbility(), players(), teamCount(0)
{}

world_cup_t::~world_cup_t()
{
	teamsById.releaseValues();
}

StatusType world_cup_t::add_team(int teamId)
{
	if (teamId <= 0)
        return StatusType::INVALID_INPUT;

    if (teamsById.find(&teamId) != nullptr)
        return StatusType::FAILURE;


    Team *team;
    try
    {
        team = new Team(teamId);
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    int *key = team->getIdPtr();
    try
    {
        teamsById.insert(key, team);
        teamsByAbility.insert(team, team);
    }
    catch (const std::bad_alloc &e)
    {
        delete team;
        return StatusType::ALLOCATION_ERROR;
    }

    teamCount++;

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId <= 0)
        return StatusType::INVALID_INPUT;

    Team *team = teamsById.find(&teamId);
    if (team == nullptr)
        return StatusType::FAILURE;

    teamsById.remove(&teamId);
    teamsByAbility.remove(team);

    if (team->getTeamSet() != nullptr)
        team->getTeamSet()->setTeam(nullptr);

    delete team;

    teamCount--;

	return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
	if ((playerId <= 0) || (teamId <= 0) || (!spirit.isvalid()) || (gamesPlayed < 0) || (cards < 0))
        return StatusType::INVALID_INPUT;

    Team *team = teamsById.find(&teamId);
    if ((team == nullptr) || (players.find(playerId) != nullptr))
        return StatusType::FAILURE;

    Player *player;
    try
    {
        player = new Player(playerId, cards, gamesPlayed, ability, goalKeeper, spirit, team);
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    try
    {
        players.insert(player);
    }
    catch (const std::bad_alloc &e)
    {
        delete player;
        return StatusType::ALLOCATION_ERROR;
    }

    if (updateTeamInAbilityTree(team, ability) == StatusType::ALLOCATION_ERROR)
        return StatusType::ALLOCATION_ERROR;

    if (team->getTeamSet() == nullptr)
        team->setTeamSet(player);
    else
        UnionFind::unite(team->getTeamSet(), player);

    team->updateTeamSpirit(spirit);
    team->updateHasGoalKeeper(goalKeeper);

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
        return StatusType::INVALID_INPUT;

    Team *team1 = teamsById.find(&teamId1);
    Team *team2 = teamsById.find(&teamId2);
    if(team1 == nullptr || team2 == nullptr)
        return StatusType::FAILURE;

    if((!team1->isLegal()) || (!team2->isLegal()))
        return StatusType::FAILURE;

    team1->getTeamSet()->updateGamesPlayed(1);
    team2->getTeamSet()->updateGamesPlayed(1);

    int fullAbility1 = team1->getTeamAbility() + team1->getPoints();
    int fullAbility2 = team2->getTeamAbility() + team2->getPoints();

    if(fullAbility1 > fullAbility2)
    {
        team1->updatePoints(3);
        return 1;
    }
    if(fullAbility1 < fullAbility2)
    {
        team2->updatePoints(3);
        return 3;
    }
    if(team1->getTeamSpirit().strength() > team2->getTeamSpirit().strength())
    {
        team1->updatePoints(3);
        return 2;
    }
    if(team1->getTeamSpirit().strength() < team2->getTeamSpirit().strength())
    {
        team2->updatePoints(3);
        return 4;
    }

    team1->updatePoints(1);
    team2->updatePoints(1);
	return 0;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
    if(playerId <= 0)
        return StatusType::INVALID_INPUT;

    Player* player = players.find(playerId);
    if(player == nullptr)
        return StatusType::FAILURE;

    return player->getGamesPlayed();
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
	if(playerId <= 0 || cards < 0)
        return StatusType::INVALID_INPUT;

    Player* player = players.find(playerId);
    if(player == nullptr)
        return StatusType::FAILURE;

    if(UnionFind::find(player)->getTeam() == nullptr)
        return StatusType::FAILURE;

    player->updateCards(cards);
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if(playerId <= 0)
        return StatusType::INVALID_INPUT;

    Player* player = players.find(playerId);
    if(player == nullptr)
        return StatusType::FAILURE;

    return player->getCards();
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	if(teamId <= 0)
        return StatusType::INVALID_INPUT;

    Team* team = teamsById.find(&teamId);
    if(team == nullptr)
        return StatusType::FAILURE;

    return team->getPoints();
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    if(i < 0 || teamCount == 0 || i >= teamCount)
        return StatusType::FAILURE;

    Team *team = teamsByAbility.select(i);

	return team->getId();
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if(playerId <= 0)
        return StatusType::INVALID_INPUT;

    Player* player = players.find(playerId);
    if(player == nullptr)
        return StatusType::FAILURE;

    if(UnionFind::find(player)->getTeam() == nullptr)
        return StatusType::FAILURE;

    return player->getPartialSpirit();
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
        return StatusType::INVALID_INPUT;

    Team* buyerTeam = teamsById.find(&teamId1);
    Team* boughtTeam = teamsById.find(&teamId2);
    if(buyerTeam == nullptr || boughtTeam == nullptr)
        return StatusType::FAILURE;

    if(buyerTeam->getTeamSet() != nullptr && boughtTeam->getTeamSet() != nullptr)
    {
        UnionFind::unite(buyerTeam->getTeamSet(), boughtTeam->getTeamSet());
    }
    else if(boughtTeam->getTeamSet() != nullptr)
    {
        buyerTeam->setTeamSet(boughtTeam->getTeamSet());
        buyerTeam->getTeamSet()->setTeam(buyerTeam);
    }

    buyerTeam->updatePoints(boughtTeam->getPoints());
    buyerTeam->updateTeamSpirit(boughtTeam->getTeamSpirit());
    buyerTeam->updateHasGoalKeeper(boughtTeam->isLegal());

    teamsById.remove(&teamId2);
    teamsByAbility.remove(boughtTeam);
    if (updateTeamInAbilityTree(buyerTeam, boughtTeam->getTeamAbility()) == StatusType::ALLOCATION_ERROR)
        return StatusType::ALLOCATION_ERROR;

    teamCount--;
    delete boughtTeam;

	return StatusType::SUCCESS;
}


//--------------------------------------- private methods ---------------------------------------------------//

StatusType world_cup_t::updateTeamInAbilityTree(Team *team, int ability)
{
    try
    {
        teamsByAbility.remove(team);
        team->updateAbility(ability);
        teamsByAbility.insert(team, team);
    }
    catch (const std::bad_alloc &e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}
