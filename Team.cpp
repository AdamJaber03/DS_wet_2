//
// Created by amir on 17/03/2024.
//

#include "Team.h"
#include "pair.h"
#include "math.h"

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
    this->strength = conestants->findHalf(conestants->getRoot(), std::ceil(conestants->getSize()/2) +1)*conestants->getSize();
    return status;
}

StatusType Team::removeContestant() {
    if (!size) return StatusType::FAILURE;
    StatusType status = conestants->remove();
    if(status == StatusType::SUCCESS) size--;
    strength = conestants->findHalf(conestants->getRoot(), std::ceil(conestants->getSize()/2) +1)*conestants->getSize();
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
    updateId(team2array, team2.size, size);
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
    size+=team2.size;
    delete[] merged.getP1();
    this->strength = conestants->findHalf(conestants->getRoot(), std::ceil(conestants->getSize()/2) +1)*conestants->getSize();
    return StatusType::SUCCESS;
}

void Team::updateId(pair<int, int> * team2, int size2, int size1){
    for(int i = 0; i < size2; i++){
        team2[i].setP2(team2[i].getP2() + size1);
    }
}

int Team::getSize() {
    return size;
}

int Team::getMedals() {
    return medals;
}

void Team::setMedals(int newMedals) {
    medals = newMedals;
}

Team::~Team() {
    delete conestants;
}
