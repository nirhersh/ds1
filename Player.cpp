#include "Player.h"
#include "wet1util.h"
#include "Team.h"
#include "AVLTree.h"

Player::Player(int playerId, int teamId, bool goalKeeper,  int gamesPlayed, int goals, int cards): m_goalKeeper(goalKeeper){
    if(playerId < 0 || teamId < 0 || gamesPlayed < 0 || goals < 0 || cards < 0){
        throw StatusType::INVALID_INPUT;
    }
    m_playerId = playerId;
    m_team = nullptr;
    m_goals = goals;
    m_cards = cards;
}

int Player::get_id(){
    return m_playerId;
}

int Player::get_team_id(){
    assert(m_team != nullptr);
    return m_team->get_id();
}

int Player::get_games_played(){
    return m_gamesPlayed;
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
    return first.m_playerId == second.m_playerId;
}