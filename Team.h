//
// Created by amir on 17/03/2024.
//

#ifndef DS_WET_2_TEAM_H
#define DS_WET_2_TEAM_H

#include "ContestantTree.h"

class Team {
public:
    Team(int id) : id(id), strength(0), size(0), conestants(nullptr) {};
    ~Team();
    int getId();
    int getStrength();
    void setStrength(int strength);
    StatusType insertContestant(int strength);
    StatusType removeContestant();
    StatusType unite(Team &team2);

private:
    int id;
    int strength;
    int size;
    ContestantTree* conestants;
    void updateId(pair<int, int> * team2, int id1);
};


#endif //DS_WET_2_TEAM_H
