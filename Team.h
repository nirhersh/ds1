

#ifndef TEAM_H
#define TEAM_H

#include "wet1util.h"
#include "AVLTree.h"
#include "Player.h"

class Player;

class Team {
public:
    Team(int teamId, int points = 0, int goals = 0, int cards = 0, int gamesPlayed = 0, bool hasGoalkeeper = false); 

    /*
        getters:
    */
    int get_team_points();
    int get_total_goals();
    int get_team_cards();
    int get_top_scorer_id();
    int get_total_players();
    int get_id();
    int get_games_played();
    int get_team_score();
    Team* get_left();
    Team* get_right();
    Player* get_player(int playerId) const;
    bool is_qulified();
    void merge_teams(Team* team1, Team* team2);
    void players_by_goals(Player** playerArray);
    void empty_team();
    /*
        setters:
    */
    void add_points(int points);
    void add_goals(int goals);
    void add_cards(int cards);
    void set_top_scorer_id(int playerId);
    void set_left(Team* team);
    void set_right(Team* team);
    void add_player(Player* newPlayer);
    void add_game();
    void remove_player(Player player);

private:
    int m_teamId;
    int m_points;
    int m_totalGoals;
    int m_cards;
    int m_gamesPlayed;
    int m_num_of_players;
    int m_goalkeeperCounter;
    bool m_hasGoalkeeper;
    Player* m_topScorer;
    Team* m_left;
    Team* m_right;
    AVLTree<Player, int> playersById;
    AVLTree<Player, Player> PlayersByGoals;
};



#endif
