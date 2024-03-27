#include "HashTable.h"
#include "Team.h"



int TeamsHashTable::hashFunction(int id) const {
    return id%max_size;
}

StatusType TeamsHashTable::insert(int id, Team* newTeam) {
    int index = hashFunction(id);
    StatusType status = teams[index].insert(id, newTeam);
    if(status != StatusType::SUCCESS) return status;
    size++;
    status = updateSize();
    if (status != StatusType::SUCCESS){
        teams[index].remove(id);
    }
    return status;
}

StatusType TeamsHashTable::remove(int id) {
    int index = hashFunction(id);
    StatusType status = teams[index].remove(id);
    if(status != StatusType::SUCCESS) return status;
    size--;
    status = updateSize();
    return status;
}

StatusType TeamsHashTable::updateSize() {
    if(size == max_size){
        max_size*=2;
        avl<int, Team*>* newlist = nullptr;
        try {
            newlist = new avl<int, Team*>[max_size]();
//            for (int i = 0; i < max_size; ++i) {
//                newlist[i] = * new avl<int, Team*>();
//            }
        }catch (std::bad_alloc&){
            max_size /= 2;
            return StatusType::ALLOCATION_ERROR;
        }
        for (int i = 0; i < max_size/2; ++i) {
            pair<int, Team*>* tempList;
            try {
                tempList = teams[i].getInorder();
            }catch (std::bad_alloc&){
                return StatusType::ALLOCATION_ERROR;
            }
            for (int j = 0; j < teams[i].getSize(); ++j) {
                StatusType status = newlist[hashFunction(tempList[j].getP1())].insert(tempList[j].getP1(), tempList[j].getP2());
                if (status != StatusType::SUCCESS){
                    max_size /= 2;
                    delete[] tempList;
                    delete[] newlist;
                    return status;
                }
            }
            delete[] tempList;
        }
        //TODO - fix leaks start here
        delete[] teams;
        teams = newlist;
    }
    else if(size == max_size/4){
        max_size/=2;
        avl<int, Team*>* newlist = nullptr;
        try {
            newlist = new avl<int, Team*>[max_size]();
//            for (int i = 0; i < max_size; ++i) {
//                newlist[i] = * new avl<int, Team*>();
//            }
        }catch (std::bad_alloc&){
            max_size *= 2;
            return StatusType::ALLOCATION_ERROR;
        }
        for (int i = 0; i < max_size*2; ++i) {
            pair<int, Team*>* tempList;
            try {
                tempList = teams[i].getInorder();
            }catch (std::bad_alloc& ){
                return StatusType::ALLOCATION_ERROR;
            }
            for (int j = 0; j < teams[i].getSize(); ++j) {
                StatusType status = newlist[hashFunction(tempList[j].getP1())].insert(tempList[j].getP1(), tempList[j].getP2());
                if (status != StatusType::SUCCESS){
                    max_size *= 2;
                    delete[] tempList;
                    delete[] newlist;
                    return status;
                }
            }
            delete[] tempList;
        }
        delete[] teams;
        teams = newlist;
    }
    return StatusType::SUCCESS;
}

TeamsHashTable::~TeamsHashTable() {
//    for (int i = 0; i < max_size; ++i) {
//        delete &teams[i];       //TODO - this might be a problem
//    }
    delete[] teams;
}

Team *TeamsHashTable::find(int id) const {
    Team** team_p = teams[hashFunction(id)].find(id);
    if (!team_p) return nullptr;
    return *team_p;
}

int TeamsHashTable::getSize() const {
    return size;
}

int TeamsHashTable::getMaxSize() const {
    return max_size;
}

avl<int, Team *> *TeamsHashTable::getTeams() {
    return teams;
}

