/*
    A class that represents a player object.
*/
#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "Team.h"
#include "wet1util.h"
#include "Team.h"
#include <memory>
class Team;

class Player{
public:
    /*
    Player c'tor
    */
    Player(int playerId, Team* team, bool goalKeeper = false,  int gamesPlayed = 0, int goals = 0, int cards = 0);

    /*
    Player d'tor
    */
    ~Player() = default;

    /*
    Player copy c'tor 
    */
    Player(const Player& other) = default;

    /*
    Player assignment and equalization operators
    */
    Player& operator=(const Player& other) = default;

    friend bool operator>(const Player& first, const Player& second);

    friend bool operator<(const Player& first, const Player& second);

    friend bool operator==(const Player& first, const Player& second);

    /*
    getters
    */

    int get_id();

    int get_team_id();

    int get_games_played();

    int get_goals();

    int get_cards();

    bool is_goalkeeper();

    /*
    adds one game to the games played count of the player
    */
    void add_games(int games);

    /*
    add a given number of goals to the player
    */
    void add_goals(int goals);

    /*
    add a given number of cards to the player
    */
    void add_cards(int cards);

private:
    Team* m_team;
    int m_playerId;
    int m_gamesPlayed;
    int m_goals;
    int m_cards;
    bool m_goalKeeper;
};

#endif //PLAYER_H