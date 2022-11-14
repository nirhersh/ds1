

#ifndef TEAM_H
#define TEAM_H

#include "wet1util.h"


class Team {
public:
    Team(int teamId, int points, int goals = 0, int cards = 0, int topScorerId = 0, int totalPlayers = 0); 


    /*
        getters:
    */
    int get_team_points();
    int get_total_goals();
    int get_team_cards();
    int get_top_scorer_id();
    int get_total_players();
    int get_id();

    /*
        setters:
    */
    void add_points(int points);
    void add_goals(int goals);
    void add_cards(int cards);
    void set_top_scorer_id(int playerId);
    void add_players(int num);

private:
    int m_teamId;
    int m_points;
    int m_totalGoals;
    int m_cards;
    int m_topScorerId;
    int m_totalPlayers;


};



#endif
