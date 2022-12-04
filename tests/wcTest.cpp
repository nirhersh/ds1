#ifndef WC_TEST
#define WC_TEST

#include <functional>
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "AVLTree.h"
#include "Player.h"
#include "Team.h"
#include "worldcup23a1.h"

bool add_team_test(){
    world_cup_t* wc = new world_cup_t();
    int numOfTeams = 0;
    int numOfInserst = 1000;
    int seed = 9187231;
    int teamIds[numOfInserst] = {0};

    for (int i = 0; i < numOfInserst; ++i){
        srand(seed + i);
        int newTeamId = rand();
        int points = rand();
        teamIds[i] = newTeamId;
        StatusType status = wc->add_team(newTeamId, points);
        if(status == StatusType::SUCCESS){
            numOfTeams++;
        }
    }

    assert(numOfTeams == wc->get_num_of_teams());

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

int main(){

}


#endif WC_TEST