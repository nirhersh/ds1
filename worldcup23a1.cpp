#include "worldcup23a1.h"

int world_cup_t::get_num_of_teams(){
	return m_teams.get_size();
}

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
	if(teamId <= 0 || points < 0){
        return StatusType::INVALID_INPUT;
    }
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
			delete teamToRemove;
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
	if(teamId <= 0 || playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || goals < 0 || cards < 0){
		return StatusType::INVALID_INPUT;
	}
	if(gamesPlayed == 0 && (cards != 0 || goals != 0)){
        return StatusType::INVALID_INPUT;
    }
	try{
		Team* playersTeam = m_teams.search(teamId);
		bool wasQualified = playersTeam->is_qulified();
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
		newPlayer->update_right(closeToNewPlayerRight);
		if(playersTeam->is_qulified() && !wasQualified){
			m_qualifiedTeams.push(playersTeam, teamId);
			Team* newLeft = m_qualifiedTeams.get_preceding_value(playersTeam->get_id());
			playersTeam->set_left(newLeft);
			if(newLeft){
				newLeft->set_right(playersTeam);
			}
			Team* newRight = m_qualifiedTeams.get_following_value(playersTeam->get_id());
			playersTeam->set_right(newRight);
			if(newRight){
				newRight->set_left(playersTeam);
			}
		}
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
			if(team->get_left()){
				team->get_left()->set_right(team->get_right());
			}
			if(team->get_right()){
				team->get_right()->set_left(team->get_left());	
			}
			m_qualifiedTeams.remove(teamId);
		}
		delete playerToRemove;
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
		Player tempPlayer = *player;
		remove_player(playerId);

		tempPlayer.add_games(gamesPlayed);
		tempPlayer.add_cards(cardsReceived);
		tempPlayer.add_goals(scoredGoals);

		int newGamesPlayed = tempPlayer.get_games_played() - team->get_games_played();

		add_player(playerId, team->get_id(), newGamesPlayed, tempPlayer.get_goals(), tempPlayer.get_cards(), tempPlayer.is_goalkeeper());

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
		Team* newLeft = m_qualifiedTeams.get_preceding_value(newTeamId);
		newTeamTreeById->set_left(newLeft);
		if(newLeft){
			newLeft->set_right(newTeamTreeById);
		}
		Team* newRight = m_qualifiedTeams.get_following_value(newTeamId);
		newTeamTreeById->set_right(newRight);
		if(newRight){
			newRight->set_left(newTeamTreeById);
		}
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

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	if(minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId){
		return StatusType::INVALID_INPUT;
	}
	if(m_qualifiedTeams.is_empty()){
		return StatusType::FAILURE;
	}
	try{
		//get qualified teams in order
		Team* minTeam = m_qualifiedTeams.binary_search_closest(minTeamId);
		if(minTeam == nullptr){
			return StatusType::FAILURE;
		}
		bool winner = false;
		int steps = 1, knockout_size = 0;
		SimulateTeam* knockout = new SimulateTeam[maxTeamId - minTeamId];
		Team* currentTeam = minTeam;
		while(currentTeam){
			if(currentTeam->get_id() > maxTeamId){
				break;
			}
			knockout[knockout_size] = SimulateTeam(currentTeam->get_id(), currentTeam->get_team_score());
			knockout_size++;
			currentTeam = currentTeam->get_right();
		}

		if(knockout_size == 0){
			delete[] knockout;
			return StatusType::FAILURE;
		}
		
		//knockout teams
		while(!winner){
			//if two steps are bigger than the knockout array, than only two teams remained
			if((knockout_size % 2 == 0&& steps >= (knockout_size/2)) || (knockout_size % 2 == 1 && steps > (knockout_size/2))){
				winner = true;
			}
			for(int i=0; i<knockout_size; i+=steps){
				if(i + steps < knockout_size){
					if(knockout[i].m_teamPoints > knockout[i+steps].m_teamPoints){
						knockout[i].m_teamPoints += (knockout[i+steps].m_teamPoints + 3);
					}else{
						knockout[i].m_teamId = knockout[i+steps].m_teamId;
						knockout[i].m_teamPoints += (knockout[i+steps].m_teamPoints + 3);
					}
				}
			}
			steps *= 2;
		}
		//winner's id will be saved in the first index in knockout
		int winnerId = knockout[0].m_teamId; 
		delete[] knockout;
		return winnerId;
	}catch(KeyDoesntExists& e){
		return StatusType::FAILURE;
	}catch(std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}
}

int world_cup_t::is_qualified(int teamId)
{
	return m_qualifiedTeams.exists(teamId);
}

bool world_cup_t::is_team_exists(int teamId)
{
	return m_teams.exists(teamId);
}





