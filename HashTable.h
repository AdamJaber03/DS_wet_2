#include "avl.h"
#include <iostream>

#ifndef UNTITLED_HASHTABEL_H
#define UNTITLED_HASHTABEL_H

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
    int hashFunction(int num);
    void updateSize();
};



#endif //UNTITLED_HASHTABEL_H
