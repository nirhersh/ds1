#ifndef TEAM_CPP
#define TEAM_CPP

#include "Team.h"

Team::Team(int teamId, int points, int goals, int cards, int topScorerId, int totalPlayers): 
            m_totalGoals(goals), m_cards(cards), m_topScorerId(topScorerId), m_totalPlayers(totalPlayers) {
    if(teamId <= 0 || points < 0){
        throw   StatusType::INVALID_INPUT;
    }
    m_teamId = teamId;
    m_points = points;
}

int Team::get_id(){
    return m_teamId
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
    return m_topScorerId;
}

int Team::get_total_players(){
    return m_totalPlayers;
}

void Team::add_points(int points){
    m_points += points;
}

void Team::add_goals(int goals){
    m_totalGoals += goals;
}

void Team::add_cards(int cards){
    m_cards += cards;
}

void Team::set_top_scorer_id(int playerId){
    m_topScorerId = playerId;
}

void Team::add_players(int num){
    m_totalPlayers += num;
}

#endif