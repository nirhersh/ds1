#include "Player.h"

Player::Player(int playerId, Team* team, bool goalKeeper,  int gamesPlayed, int goals, int cards): m_goalKeeper(goalKeeper){
    if(playerId <= 0 || team->get_id() <= 0 || gamesPlayed < 0 || goals < 0 || cards < 0){
        throw InvalidArguments();
    }
    m_team = team;
    m_playerId = playerId;
    m_goals = goals;
    m_cards = cards;
}

int Player::get_id(){
    return m_playerId;
}

int Player::get_team_id(){
    return m_team->get_id();
}

int Player::get_games_played(){
    return m_gamesPlayed + m_team->get_games_played();
}

int Player::get_goals(){
    return m_goals;
}

int Player::get_cards(){
    return m_cards;
}

bool Player::is_goalkeeper(){
    return m_goalKeeper;
}

Player* Player::get_right(){
    return m_right;
}

Player* Player::get_left(){
    return m_left;
}

Player* Player::get_close_to_me(){
    return closest(m_left, m_right);
}

void Player::update_left(Player* player){
    m_left = player;
}

void Player::update_right(Player* player){
    m_right = player;
}

Player* Player::closest(Player* player1, Player* player2){
    if(player1 == nullptr){
        return player2;
    }else if(player2 == nullptr){
        return player1;
    }
    // return nullptr if both players are nullptr;
    if(std::abs(player1->get_goals() - m_goals) > std::abs(player2->get_goals() - m_goals)){
        return player2;
    }else if(std::abs(player1->get_goals() - m_goals) < std::abs(player2->get_goals() - m_goals)){
        return player1;
    }else{
        if(std::abs(player1->get_cards() - m_cards) > std::abs(player2->get_cards() - m_cards)){
            return player2;
        }else if(std::abs(player1->get_cards() - m_cards) < std::abs(player2->get_cards() - m_cards)){
            return player1;
        }else{
            if(std::abs(player1->get_id() - m_playerId) > std::abs(player2->get_id() - m_playerId)){
                return player2;
            }else if(std::abs(player1->get_id() - m_playerId) < std::abs(player2->get_id() - m_playerId)){
                return player1;
            }else{
                return player1->get_id() > player2->get_id() ? player1 : player2;
            }   
        }
    }
}

void Player::add_games(int games){
    m_gamesPlayed += games;
}

void Player::add_goals(int goals){
    m_goals += goals;
}

void Player::add_cards(int cards){
    m_cards += cards;
}

bool operator>(const Player& first, const Player& second)
{
    if(first.m_goals > second.m_goals) {
        return true;
    }
    if(second.m_goals > first.m_goals) {
        return false;
    }
    if(first.m_cards < second.m_cards) {
        return true;
    }
    if(second.m_cards < first.m_cards) {
        return false;
    }
    return first.m_playerId > second.m_playerId;
}

bool operator<(const Player& first, const Player& second)
{
    return !(first > second || first == second);
}

bool operator==(const Player& first, const Player& second)
{
    return first.m_goals == second.m_goals && first.m_playerId == second.m_playerId && first.m_cards == second.m_cards;
}