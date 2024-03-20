//
// Created by amir on 17/03/2024.
//

#include "Team.h"
#include "pair.h"

pair<pair<int, int>*, int> merge(pair<int, int> *arr1, int size1, pair<int, int> *arr2, int size2){
    pair<int, int>* newList = new pair<int, int>[size1+size2];
    int i1 = 0, i2 = 0, duplicates = 0;
    while(i1 + i2 < size1+size2){
        if (i1 == size1){
            newList[i1 + i2 - duplicates] = arr2[i2];
            i2++;
            continue;
        }
        if (i2 == size2){
            newList[i1 + i2 - duplicates] = arr1[i1];
            i1++;
            continue;
        }
        if(arr1[i1] == arr2[i2]){
            duplicates++;
            i1++;
            continue;
        }
        if (arr1[i1] < arr2[i2]){
            newList[i1 + i2 - duplicates] = arr1[i1];
            i1++;
        }else{
            newList[i1 + i2 - duplicates] = arr2[i2];
            i2++;
        }
    }
    int newSize = size1 + size2 - duplicates;
    pair<int, int>* noDupList = new pair<int, int>[newSize];
    for (int i = 0; i < newSize; ++i) {
        noDupList[i] = newList[i];
    }
    delete[] newList;
    return pair<pair<int, int>*, int>(noDupList, newSize);
}

int Team::getId() {
    return id;
}

int Team::getStrength() {
    return strength;
}

void Team::setStrength(int strength) {
    this->strength = strength;
}

StatusType Team::insertContestant(int strength) {
    StatusType status = conestants->insert(strength, size + 1);
    if(status == StatusType::SUCCESS) size++;
    return status;
}

StatusType Team::removeContestant() {
    StatusType status = conestants->remove();
    if(status == StatusType::SUCCESS) size--;
return status;
}

StatusType Team::unite(Team &team2) {
    pair<int, int> *team1array, *team2array;
    try {
        team1array = conestants->getInorder();
    }catch (std::bad_alloc& err){
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        team2array = team2.conestants->getInorder();
    }catch (std::bad_alloc& err){
        delete[] team1array;
        return StatusType::ALLOCATION_ERROR;
    }
    updateId(team2array, id);
    pair<pair<int, int>*, int> merged = merge(team1array, size, team2array, team2.size);
    delete[] team1array;
    delete[] team2array;
    ContestantTree * newTeam = nullptr;
    try{
        newTeam = new ContestantTree();
    }catch (std::bad_alloc& err){
        delete[] merged.getP1();
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        newTeam->createEmpty(merged.getP2()); //TODO : check the size
        newTeam->fillEmpty(merged.getP1());
    }catch (std::bad_alloc& err){
        delete[] merged.getP1();
        delete newTeam;
        return StatusType::ALLOCATION_ERROR;
    }
    delete conestants;
    conestants = newTeam;
    delete[] merged.getP1();
}

