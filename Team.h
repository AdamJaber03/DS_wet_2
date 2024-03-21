//
// Created by amir on 17/03/2024.
//

#ifndef DS_WET_2_TEAM_H
#define DS_WET_2_TEAM_H

#include "ContestantTree.h"

class Team {
public:
    explicit Team(int id) : id(id), strength(0), size(0), conestants(nullptr){};
    ~Team();
    int getId();
    int getStrength();
    int getSize();
    void setStrength(int strength);
    StatusType insertContestant(int strength);
    StatusType removeContestant();
    StatusType unite(Team &team2);
    int getSize();

private:
    int id;
    int strength;
    int size;
    ContestantTree* conestants;
    void updateId(pair<int, int> * team2, int size2, int size1);
};


#endif //DS_WET_2_TEAM_H
