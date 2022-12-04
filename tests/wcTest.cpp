
#include <functional>
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <string>
#include "../AVLTree.h"
#include "../Player.h"
#include "../Team.h"
#include "../worldcup23a1.h"
#include "../wet1util.h"


void team_generator(world_cup_t* wc, int numOfTeams);
void print_error(std::string error, int line);
bool add_team_test();
void run_test(std::function<bool()> test, std::string test_name);
bool remove_team_test();
bool remove_player_test();

int main(){
    run_test(add_team_test, "add team test");
    run_test(remove_team_test, "remove team test");
}

void run_test(std::function<bool()> test, std::string test_name)
{
    if(!test()){
        std::cout<< test_name <<" FAILED."<<std::endl;
        std::cout << std::endl;
        return;
    }
    std::cout<<test_name<<" SUCCEEDED."<<std::endl;
    std::cout << std::endl;

}

void print_error(std::string error, int line){
    std::cout << "error: " << error << " at line: " << line << std::endl;
}

void team_generator(world_cup_t* wc, int numOfTeams = 1000)
{
    int seed = 9187231;
    int numOfPlayers;

    for (int i = 0; i < 100; i++){
        srand(seed + i);
        int newTeamId = rand();
        int points = rand() % 100;
        StatusType status = wc->add_team(newTeamId, points);
        if(status == StatusType::SUCCESS){
            numOfTeams++;
        }
        int seed2 = 654365;
        srand(seed2 + i);
        numOfPlayers = rand() % 20;
        for (int i = 0; i < numOfPlayers; i++)
        {
            int playerId = rand();
            int gamesPlayed = rand() % 7;
            int goals = rand() % 5;
            int cards = rand() % 3;
            bool goalKeeper =!(rand() & 7);
            StatusType status1 = wc->add_player(playerId, newTeamId, gamesPlayed, goals, cards, goalKeeper);
        }
    }
}

bool add_team_test(){
    world_cup_t wc;
    team_generator(&wc);
    int numOfTeams = wc.get_num_of_teams();
    
    // check invalid team id
    StatusType status = wc.add_team(0, 0);
    if(status != StatusType::INVALID_INPUT){
        print_error("inserted invalid team id", __LINE__);
        return false;
    }

    //check invalid team points
    status = wc.add_team(1, -1);
    if(status != StatusType::INVALID_INPUT){
        print_error("inserted invalid team points", __LINE__);
        return false;
    }

    //check SUCCESS and FAILURE
    int addedTeam;
    for(int i=0; i<10; i++){
        if(!wc.is_team_exists(i)){
            addedTeam = i;
            status = wc.add_team(i, 11);
            break;
        }
    }
    if(status != StatusType::SUCCESS){
        print_error("valid team: should get SUCCESS", __LINE__);
        return false;
    }
    status = wc.add_team(addedTeam, 345);
    if(status != StatusType::FAILURE){
        print_error("team already exists", __LINE__);
        return false;
    }
    
    if(wc.get_num_of_teams() != numOfTeams + 1){
        print_error("number of teams doesn't match", __LINE__); 
        return false;
    }
    return true;
}


bool remove_team_test()
{
    bool flag = true;
    world_cup_t wc;
    team_generator(&wc);
    int seed = 87231;
    int numOfPlayers;

    for (int i = 0; i < 1000; i++){
        srand(seed + i);
        int newTeamId = rand();
        int points = rand() % 100;
        StatusType status = wc.add_team(newTeamId, points);
        if(status == StatusType::SUCCESS){
            status = wc.remove_team(newTeamId);
            if(status != StatusType::SUCCESS){
                print_error("should remove the team succecfully", __LINE__);
                flag = false;
            }

            //trys to remove the same team again:
            status = wc.remove_team(newTeamId);
            if(status != StatusType::FAILURE){
                print_error("removed team that is not exist", __LINE__);
                flag = false;
            }
        }
        status = wc.remove_team(-24);
        if(status != StatusType::INVALID_INPUT){
            print_error("should return invalid value", __LINE__);
            flag = false;
        }
    }
    return flag;
}


bool get_all_players_count(){
    world_cup_t wc;
    int seed = 9187231;
    int numOfPlayers;
    int numOfTeams = 1000;
    int numOfPlayers = 0;

    if(wc.get_all_players_count(1).status() != StatusType::FAILURE){
        print_error("Wrong status", __LINE__);
    }

    for (int i = 0; i < numOfTeams; i++){
        srand(seed + i);
        int newTeamId = rand();
        int points = rand() % 100;
        StatusType status = wc.add_team(newTeamId, points);
        if(status == StatusType::SUCCESS){
            numOfTeams++;
        }
        int seed2 = 654365;
        srand(seed2 + i);
        numOfPlayers = rand() % 20;
        for (int i = 0; i < numOfPlayers; i++)
        {
            int playerId = rand();
            int gamesPlayed = rand() % 7;
            int goals = rand() % 5;
            int cards = rand() % 3;
            bool goalKeeper =!(rand() & 7);
            StatusType status1 = wc.add_player(playerId, newTeamId, gamesPlayed, goals, cards, goalKeeper);
            if(status1 == StatusType::SUCCESS){
                numOfPlayers++;
            }
        }
    }

    if(wc.get_all_players_count(0).status() != StatusType::INVALID_INPUT){
        print_error("Wrong status", __LINE__);
        return false;
    }
    
    
    
    if(numOfPlayers != wc.get_all_players_count(-1).ans()){
        print_error("number of players is wrong", __LINE__);
        return false;
    }

    int numOfNewTeams = 100;
    int seed3 = 876543;
    int newTeams[numOfNewTeams] = {0};
    for(int i=0; i<100; i++){
        srand(seed3 + i);
        newTeams[i] = rand();
    }
    int seed4 = 2345456;
    for(int i=0; i<numOfNewTeams; i++){
        if(newTeams[i] == 0){
            continue;
        }
        int newTeamSize = 0;
        for(int j=0; j<21; j++){
            srand(seed4 + i);
            int playerId = rand();
            StatusType status = wc.add_player(playerId, newTeams[i], 0, 0, 0, false);
            if(status == StatusType::SUCCESS){
                newTeamSize++;
            }
        }
        if(newTeamSize != wc.get_all_players_count(newTeams[i]).ans()){
            print_error("team size is wrong", __LINE__);
            return false;
        }
    }
    return true;
}

bool get_all_players_test(){
    world_cup_t wc;
    int seed = 9187231;
    int numOfPlayers;

    for (int i = 0; i < 100; i++){
        srand(seed + i);
        int newTeamId = rand();
        int points = rand() % 100;
        StatusType status = wc.add_team(newTeamId, points);
        int seed2 = 654365;
        srand(seed2 + i);
        numOfPlayers = rand() % 20;
        for (int i = 0; i < numOfPlayers; i++)
        {
            int playerId = rand();
            int gamesPlayed = rand() % 7;
            int goals = rand() % 5;
            int cards = rand() % 3;
            bool goalKeeper =!(rand() & 7);
            StatusType status1 = wc.add_player(playerId, newTeamId, gamesPlayed, goals, cards, goalKeeper);
        }
    }
    
    
}

bool add_player_test()
{
    bool flag = true;
    StatusType status;
    world_cup_t wc;
    team_generator(&wc, 10);
    int seed2 = 654365;
    int numOfPlayers;
    srand(seed2);
    int newTeamId = rand();
    status = wc.add_team(newTeamId, 0);
    //checks invalid input cases:
    status = wc.add_player(0, newTeamId, 0, 0, 0, 0);
    if(status != StatusType::INVALID_INPUT){
        print_error("insert player with non positive id", __LINE__);
        flag = false;
    }
    status = wc.add_player(5, 0, 0, 0, 0, 0);
    if(status != StatusType::INVALID_INPUT){
        print_error("insert player with non positive Team id", __LINE__);
        flag = false;
    }
    status = wc.add_player(5, 5, -1, 0, 0, 0);
    if(status != StatusType::INVALID_INPUT){
        print_error("insert player with negative games played", __LINE__);
        flag = false;
    }
    status = wc.add_player(5, 5, 5, -1, 0, 0);
    if(status != StatusType::INVALID_INPUT){
        print_error("insert player with negative goals", __LINE__);
        flag = false;
    }
    status = wc.add_player(5, 5, 5, 5, -1, 0);
    if(status != StatusType::INVALID_INPUT){
        print_error("insert player with negative cards", __LINE__);
        flag = false;
    }
    status = wc.add_player(5, 5, 0, 1, 8, 0);
    if(status != StatusType::INVALID_INPUT){
        print_error("insert player with cards and goals althogh zero games played", __LINE__);
        flag = false;
    }
    for (int i = 0; i < numOfPlayers; i++)
    {
        int playerId = rand();
        int gamesPlayed = rand() % 7;
        int goals = rand() % 1000;
        int cards = rand() % 1000;
        bool goalKeeper =!(rand() & 7);
        StatusType status1 = wc.add_player(playerId, newTeamId, gamesPlayed, goals, cards, goalKeeper);
    
    }


}
