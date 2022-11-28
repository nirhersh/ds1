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
    return playersById.get_size();
}

int Team::get_games_played(){
    return m_gamesPlayed;
}

int Team::get_team_score(){
    return m_points+m_totalGoals-m_cards;
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
    m_cards += newPlayer->get_cards();
    m_totalGoals += newPlayer->get_goals();
    if(newPlayer->is_goalkeeper()){
        m_goalkeeperCounter++;
    }
    m_hasGoalkeeper = m_goalkeeperCounter > 0 ? true : false;
    if((*newPlayer) > (*m_topScorer)){
        m_topScorer = newPlayer;
    }
    playersById.push(newPlayer, (*newPlayer).get_id());
    PlayersByGoals.push(newPlayer, *newPlayer);
}

void Team::remove_player(Player player){
    if(*m_topScorer == player){
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
}

#endif