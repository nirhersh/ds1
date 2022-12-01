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
		if(m_bestPlayer == nullptr || (*newPlayer) > (*m_bestPlayer)){
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
		int teamId = playerToRemove->get_team()->get_id();
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
		Team* team = player->get_team();
		//remove before the update because the key is the player itself
		m_allPlayersGoals.remove(*player);
		team->remove_player(*player);
		player->add_games(gamesPlayed);
		player->add_cards(cardsReceived);
		player->add_goals(scoredGoals);
		//update the player in the team trees
		team->add_player(player);	
		//update the player in the players tree
		m_allPlayersGoals.push(player, *player);

	}catch(KeyDoesntExists& e){
		return StatusType::FAILURE;
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	//check input
	if(teamId1 <=0 || teamId2 <=0 || teamId1 == teamId2){
		return StatusType::INVALID_INPUT;
	}
	//check kosher (qualified)
	if(!(m_qualifiedTeams.exists(teamId1)) || !(m_qualifiedTeams.exists(teamId2))){
		return StatusType::FAILURE;
	}
	//apply match
	Team* team1 = m_qualifiedTeams.search(teamId1);
	Team* team2 = m_qualifiedTeams.search(teamId2);
	if(team2->get_team_score() > team2->get_team_score()){
		team1->add_points(3);
	} else if(team2->get_team_score() < team2->get_team_score()){
		team2->add_points(3);
	} else{
		team1->add_points(1);
		team2->add_points(1);
	}
	team1->add_game();
	team1->add_game();
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	if(playerId <= 0){
		return StatusType::INVALID_INPUT;
	}
	if(!m_allPlayersId.exists(playerId)){
		return StatusType::FAILURE;
	}
	Player* player1 = m_allPlayersId.search(playerId);
	return(player1->get_games_played() + player1->get_team()->get_games_played());
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	//check input
	if(teamId <=0){
		return StatusType::INVALID_INPUT;
	}
	//check existence
	if(!(m_teams.exists(teamId))){
		return StatusType::FAILURE;
	}
	return m_teams.search(teamId)->get_team_points();
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	//check input
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2 || newTeamId <= 0){
		return StatusType::INVALID_INPUT;
	}
	//check newteamId
	if(m_teams.exists(newTeamId) && newTeamId != teamId1 && newTeamId != teamId2){
		return StatusType::FAILURE;
	}
	//check existence of tree 1 and 2
	if(!(m_teams.exists(teamId1)) || !(m_teams.exists(teamId2))){
		return StatusType::FAILURE;
	}
	Team* team1 = m_teams.search(teamId1);
	Team* team2 = m_teams.search(teamId2);
	Team* newTeamTreeById = new Team(newTeamId);
	newTeamTreeById->merge_teams(team1, team2);
	team1->empty_team();
	team2->empty_team();
	remove_team(teamId1);
	remove_team(teamId2);
	m_teams.push(newTeamTreeById, newTeamId);
	if(newTeamTreeById->is_qulified()){
		m_qualifiedTeams.push(newTeamTreeById, newTeamId);
	}
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	if(teamId == 0){
		return StatusType::INVALID_INPUT;
	}
	if(teamId > 0){
		if(!m_teams.exists(teamId)){
			return StatusType::FAILURE;
		}
		Team* currentTeam = m_teams.search(teamId);
		if(currentTeam->get_total_players() == 0){
			return StatusType::FAILURE;
		} else return currentTeam->get_top_scorer_id();
	} else if(!m_bestPlayer){
		return StatusType::FAILURE;
	} else return m_bestPlayer->get_id();
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	if(teamId == 0){
		return StatusType::INVALID_INPUT;
	}
	int num_of_players = 0;
	if(teamId > 0){
		try{
			num_of_players = m_teams.search(teamId)->get_total_players();
		}catch(KeyDoesntExists& e){
			return StatusType::FAILURE;
		}catch(std::bad_alloc& e){
			return StatusType::ALLOCATION_ERROR;
		}
	}else{
		num_of_players = m_allPlayersId.get_size();
	}
	return num_of_players;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	if(output == NULL || teamId == 0){
		return StatusType::INVALID_INPUT;
	}
	if(teamId < 0){
		if(m_allPlayersGoals.is_empty()){
			return StatusType::FAILURE;
		}
		try{
			Player** playersArray = new Player*[m_allPlayersGoals.get_size()];
			m_allPlayersGoals.in_order(playersArray);
			for(int i=0; i < m_allPlayersGoals.get_size(); i++){
				output[i] = playersArray[i]->get_id();
			}
			delete[] playersArray;
		}catch(std::bad_alloc& e){
			return StatusType::ALLOCATION_ERROR;
		}
	}else{
		try{
			Team* team = m_teams.search(teamId);
			if(team->get_total_players() == 0){
				return StatusType::FAILURE;
			}
			Player** playersArray = new Player*[team->get_total_players()];
			team->players_by_goals(playersArray);
			for(int i=0; i<team->get_total_players(); i++){
				output[i] = playersArray[i]->get_id();
			}
			delete[] playersArray;
		}catch(KeyDoesntExists& e){
			return StatusType::FAILURE;
		}catch(std::bad_alloc& e){
			return StatusType::ALLOCATION_ERROR;
		}
	}
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	if(playerId <=0 || teamId <= 0){
		return StatusType::INVALID_INPUT;
	}
	try{
		Team* team = m_teams.search(teamId);
		Player* close = team->get_player(playerId)->get_close_to_me();
		if(!close){
			return StatusType::FAILURE;
		}else{
			return close->get_id();
		}
	}catch(KeyDoesntExists& e){
		return StatusType::FAILURE;
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}
}

// output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
// {
// 	if(minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId){
// 		return StatusType::INVALID_INPUT;
// 	}
// 	if(m_qualifiedTeams.is_empty()){
// 		return StatusType::FAILURE;
// 	}
// 	try{
// 		Team** qualified = new Team*[m_qualifiedTeams.get_size()];
// 		m_qualifiedTeams.in_order(qualified);
		
// 		bool winner = false;
// 		int steps = 1, minIndex = 0, maxIndex= 0;

// 		for(int i = 0; i<m_qualifiedTeams.get_size(); i++){
// 			if(qualified[i]->get_id() >= minTeamId){
// 				minIndex = i;
// 				break;
// 			}
// 		}
		
// 		for(int i = 0; i<m_qualifiedTeams.get_size(); i++){
// 			if(qualified[i]->get_id() > maxTeamId){
// 				maxIndex = i;
// 				break;
// 			}
// 		}
// 	}
// }

// 		SimulateTeam* knockout = new SimulateTeam[maxIndex - minIndex];
// 		for(int i=0; )
		
// 		while(!winner){
// 			for(int i=minIndex; i<maxIndex; i+=steps){
// 				if(i + steps < maxIndex){
					
// 				}
// 			}
// 		}
// 		delete[] qualified;
// 	}
// }

