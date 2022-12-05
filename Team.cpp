#ifndef TEAM_CPP
#define TEAM_CPP

#include "Team.h"

Team::Team(int teamId, int points, int goals, int cards, int gamesPlayed, bool hasGoalkeeper): 
            m_totalGoals(goals), m_cards(cards), m_gamesPlayed(gamesPlayed), m_hasGoalkeeper(hasGoalkeeper) {
    if(teamId <= 0 || points < 0){
        throw InvalidArguments();
    }
    m_teamId = teamId;
    m_points = points;
    m_goalkeeperCounter = 0;
    m_num_of_players = 0;
    m_topScorer = nullptr;
    m_left = nullptr;
    m_right = nullptr;
}

int Team::get_id(){
    return m_teamId;
}

int Team::get_team_points(){
    return m_points;
}

int Team::get_total_goals(){
    return m_totalGoals;
}

int Team::get_team_cards(){
    return m_cards;
}

int Team::get_top_scorer_id(){
    return m_topScorer->get_id();
}

int Team::get_total_players(){
    return m_num_of_players;
}

int Team::get_games_played(){
    return m_gamesPlayed;
}

int Team::get_team_score(){
    return m_points+m_totalGoals-m_cards;
}

Player* Team::get_player(int playerId) const{
    return playersById.search(playerId);
}

bool Team::is_qulified(){
    if(m_hasGoalkeeper && playersById.get_size() >= 11){
        return true;
    }else{
        return false;
    }
}

void Team::add_points(int points){
    m_points += points;
}

void Team::add_game(){
    m_gamesPlayed++;
}

void Team::add_goals(int goals){
    m_totalGoals += goals;
}

void Team::add_cards(int cards){
    m_cards += cards;
}

void Team::add_player(Player* newPlayer){
    if(newPlayer == nullptr){
        return;
    }
    m_cards += newPlayer->get_cards();
    m_totalGoals += newPlayer->get_goals();
    if(newPlayer->is_goalkeeper()){
        m_goalkeeperCounter++;
    }
    m_hasGoalkeeper = m_goalkeeperCounter > 0 ? true : false;
    if(m_topScorer && newPlayer){
        if((*newPlayer) > (*m_topScorer)){
            m_topScorer = newPlayer;
        }
    }else{
        m_topScorer = newPlayer;
    }
    playersById.push(newPlayer, (*newPlayer).get_id());
    PlayersByGoals.push(newPlayer, *newPlayer);
    m_num_of_players++;
}

void Team::remove_player(Player player){
    if(!m_topScorer && *m_topScorer == player){
        m_topScorer = PlayersByGoals.get_preceding_value(player);
    }
    playersById.remove(player.get_id());
    PlayersByGoals.remove(player);
    m_cards -= player.get_cards();
    m_totalGoals -= player.get_goals();
    if(player.is_goalkeeper()){
        m_goalkeeperCounter--;
        m_hasGoalkeeper = m_goalkeeperCounter > 0 ? true : false;
    }
    --m_num_of_players;
}

void Team::merge_teams(Team* team1, Team* team2){
    AVLTree<Player, int>::unite_trees(team1->playersById, team2->playersById, playersById);
    AVLTree<Player, Player>::unite_trees(team1->PlayersByGoals, team2->PlayersByGoals, PlayersByGoals);
    add_points(team1->get_team_points() + team2->get_team_points());
    add_goals(team1->get_total_goals() + team2->get_total_goals());
    add_cards(team1->get_team_cards() + team2->get_team_cards());
    m_gamesPlayed = 0;
    m_goalkeeperCounter = team1->m_goalkeeperCounter + team2->m_goalkeeperCounter;
    m_hasGoalkeeper = m_goalkeeperCounter;
    m_num_of_players = team1->m_num_of_players + team2->m_num_of_players;
    if(!team1->m_topScorer){
        m_topScorer = team2->m_topScorer;   
    } else if(!team2->m_topScorer){
        m_topScorer = team1->m_topScorer;   
    } else{
        m_topScorer = *(team1->m_topScorer) > *(team2->m_topScorer)? team1->m_topScorer : team2->m_topScorer; 
    }

    //updating player's games played and teams pointer
    Player** playerOfTree1 = new Player*[team1->get_total_players()];
    playersById.in_order(playerOfTree1);
    int team1Games = team1->get_games_played();
    for (int i = 0; i < team1->get_total_players(); i++)
    {
        playerOfTree1[i]->set_games_played(playerOfTree1[i]->get_games_played() + team1Games);
        playerOfTree1[i]->set_team(this);
    }

    Player** playerOfTree2 = new Player*[team2->get_total_players()];
    int team2Games = team2->get_games_played();
    for (int i = 0; i < team2->get_total_players(); i++)
    {
        playerOfTree2[i]->set_games_played(playerOfTree2[i]->get_games_played() + team2Games);

        playerOfTree2[i]->set_team(this);

    }
}

void Team::players_by_goals(Player** playerArray){
    PlayersByGoals.in_order(playerArray);
}

void Team::empty_team()
{
    m_topScorer = nullptr;
    m_num_of_players = 0;
    m_left = nullptr;
    m_right = nullptr;
}

Team* Team::get_left(){
    return m_left;
}


Team* Team::get_right(){
    return m_right;
}

void Team::set_right(Team* team){
    m_right = team;
}

void Team::set_left(Team* team){
    m_left = team;
}
#endif