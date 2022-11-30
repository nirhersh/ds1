#include "worldcup23a1.h"

world_cup_t::world_cup_t(): m_bestPlayer(nullptr){}

world_cup_t::~world_cup_t()
{
	Player** playersToDelete = new Player*[m_allPlayersId.get_size()];
	m_allPlayersId.in_order(playersToDelete);
	for(int i = 0; i < m_allPlayersId.get_size(); i++){
		delete playersToDelete[i];
	}

	delete[] playersToDelete;

	Team** teamsToDelete = new Team*[m_teams.get_size()];
	m_teams.in_order(teamsToDelete);
	for(int i = 0; i < m_teams.get_size(); i++){
		delete teamsToDelete[i];
	}

	delete[] teamsToDelete;
}


StatusType world_cup_t::add_team(int teamId, int points)
{
	try{
		Team* newTeam(new Team(teamId, points));
		m_teams.push(newTeam, teamId);
	} catch(std::bad_alloc& exc) {
		return StatusType::ALLOCATION_ERROR;
	}
	catch(KeyAlreadyExists& e) {
		return StatusType::FAILURE;
	}catch(InvalidArguments& e){
		return StatusType::INVALID_INPUT;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}
	try{
		Team* teamToRemove = m_teams.search(teamId);
		if(teamToRemove->get_total_players() != 0){
			return StatusType::FAILURE;
		}else{
			m_teams.remove(teamId);
		}
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}catch(KeyDoesntExists& e){
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}
	try{
		Team* playersTeam = m_teams.search(teamId);
		Player* newPlayer(new Player(playerId, playersTeam, goalKeeper, gamesPlayed, goals, cards));
		//push new player to world cup trees
		m_allPlayersId.push(newPlayer, playerId);
		m_allPlayersGoals.push(newPlayer, *newPlayer);
		//push new player to team tree
		playersTeam->add_player(newPlayer);
		//update top scorer if needed
		if((*newPlayer) > (*m_bestPlayer)){
			m_bestPlayer = newPlayer;
		}
		//update close to and close to me
		Player* closeToNewPlayerLeft = m_allPlayersGoals.get_preceding_value(*newPlayer);
		Player* closeToNewPlayerRight = m_allPlayersGoals.get_following_value(*newPlayer);
		
		if(closeToNewPlayerLeft != nullptr){
			closeToNewPlayerLeft->update_right(newPlayer);
		}
		newPlayer->update_left(closeToNewPlayerLeft);
		if(closeToNewPlayerRight != nullptr){
			closeToNewPlayerRight->update_left(newPlayer);
		}
		newPlayer->update_left(closeToNewPlayerRight);
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}catch(InvalidArguments& e){
		return StatusType::INVALID_INPUT;
	}catch(KeyAlreadyExists& e){
		return StatusType::FAILURE;
	}catch(KeyDoesntExists& e){
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if(playerId<=0){
		return StatusType::INVALID_INPUT;
	}
	try{
		Player* playerToRemove = m_allPlayersId.search(playerId);
		//update total top scorer
		if(m_bestPlayer == playerToRemove){
			m_bestPlayer = playerToRemove->get_close_to_me();
		}
		//update close player left
		Player* closePlayerLeft = playerToRemove->get_left();
		if(closePlayerLeft){
			closePlayerLeft->update_right(playerToRemove->get_right());
		}
		//update close player right
		Player* closePlayerRight = playerToRemove->get_right();
		if(closePlayerRight){
			closePlayerRight->update_left(playerToRemove->get_left());
		}
		//remove from team and world cup trees
		int teamId = playerToRemove->get_team_id();
		Team* team = m_teams.search(teamId);
		bool wasQualified = team->is_qulified();
		team->remove_player(*playerToRemove);
		m_allPlayersGoals.remove(*playerToRemove);
		m_allPlayersId.remove(playerId);
		//check if team is still qualified
		bool qualified = team->is_qulified();
		if(wasQualified && !qualified){
			m_qualifiedTeams.remove(teamId);
		}
	}catch(KeyDoesntExists& e){
		return StatusType::FAILURE;
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	if(playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0){
		return StatusType::INVALID_INPUT;
	}
	try{
		//update the player stats
		Player* player = m_allPlayersId.search(playerId);
		player->add_games(gamesPlayed);
		player->add_cards(cardsReceived);
		player->add_goals(scoredGoals);
		//update the player in the team trees
		Team* team = m_teams.search(player->get_team_id());
		team->remove_player(*player);
		team->add_player(player);	
		//update the player in the players tree


	}catch(KeyDoesntExists& e){
		return StatusType::FAILURE;
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2 || newTeamId <= 0){
		return StatusType::INVALID_INPUT;
	}
	if(m_teams.exists(newTeamId) && newTeamId != teamId1 && newTeamId != teamId2){
		return StatusType::FAILURE;
	}
	if(!(m_teams.exists(teamId1)) || !(m_teams.exists(teamId2))){
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

