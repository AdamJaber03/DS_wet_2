#include "HashTable.h"
#include "Team.h"


int TeamsHashTable::hashFunction(int id) {
    return id%max_size;
}

StatusType TeamsHashTable::insert(int id) {
    int index = hashFunction(id);
    Team *newTeam = new Team(id, 0); //todo - define team
    StatusType status = teams[index].insert(id, newTeam);
    if(status != StatusType::SUCCESS){
        delete newTeam;
        return status;
    }
    size++;
    updateSize();
    return status;
}

StatusType TeamsHashTable::remove(int id) {
    int index = hashFunction(id);
    StatusType status = teams[index].remove(id);
    if(status != StatusType::SUCCESS) return status;
    size--;
    updateSize();
    return status;
}

StatusType TeamsHashTable::updateSize() {
    if(size == max_size){
        max_size*=2;
        avl<int, Team*>* newlist = nullptr;
        try {
            newlist = new avl<int, Team*>[max_size];
        }catch (std::bad_alloc&){
            max_size /= 2;
            return StatusType::ALLOCATION_ERROR;
        }
        for (int i = 0; i < max_size/2; ++i) {
            pair<int, Team*>* tempList = teams[i].getInorder();
            for (int j = 0; j < teams[i].getSize(); ++j) {
                StatusType status = newlist[hashFunction(tempList[j].getP1())].insert(tempList[j].getP1(), tempList[j].getP2());
                if (status != StatusType::SUCCESS){
                    max_size /= 2;
                    delete newlist;
                    return status;
                }
            }
            delete tempList;
        }
        delete teams;
        teams = newlist;
    }
    else if(size == max_size/4){
        max_size/=2;
        avl<int, Team*>* newlist = nullptr;
        try {
            newlist = new avl<int, Team*>[max_size];
        }catch (std::bad_alloc&){
            max_size *= 2;
            return StatusType::ALLOCATION_ERROR;
        }
        for (int i = 0; i < max_size*2; ++i) {
            pair<int, Team*>* tempList = teams[i].getInorder();
            for (int j = 0; j < teams[i].getSize(); ++j) {
                StatusType status = newlist[hashFunction(tempList[j].getP1())].insert(tempList[j].getP1(), tempList[j].getP2());
                if (status != StatusType::SUCCESS){
                    max_size *= 2;
                    delete tempList;
                    delete newlist;
                    return status;
                }
            }
            delete tempList;
        }
        delete teams;
        teams = newlist;
    }
    return StatusType::SUCCESS;
}

TeamsHashTable::~TeamsHashTable() {
    for(int i = 0; i < max_size; i++){
        pair<int, Team*>* tempList = teams[i].getInorder();
        for (int j = 0; j < teams[i].getSize(); ++j) {
            delete tempList[j].getP2();
        }
        delete tempList;
    }
    delete[] teams;
}

