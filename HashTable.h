#ifndef UNTITLED_HASHTABLE_H
#define UNTITLED_HASHTABLE_H

#include "avl.h"
#include "wet2util.h"
#include "Team.h"
#include <iostream>


class TeamsHashTable{
public:
    TeamsHashTable() : max_size(2), size(0), teams(nullptr){
        teams = new avl<int, Team*>[2];
    };
    ~TeamsHashTable(); //TODO : destroy the array after changing to dinamic;
    StatusType insert(int id);
    StatusType remove(int id);

private:
    int max_size;
    int size;
    avl<int, Team*>* teams;
    int hashFunction(int num) const;
    StatusType updateSize();
};



#endif //UNTITLED_HASHTABLE_H
