//
// Created by amir on 17/03/2024.
//

#ifndef DS_WET_2_CONTESTANTTREE_H
#define DS_WET_2_CONTESTANTTREE_H

#include "wet2util.h"
#include "NodeContestants.h"
#include "pair.h"


class ContestantTree {
public:
    ContestantTree(): root(nullptr), size(0), maxKey(0), minKey(0) {};
    virtual ~ContestantTree();
    StatusType insert(int key, int value);
    StatusType remove();
    NodeContestants * strengthNewestPlayer(NodeContestants * ptr, int id);
    NodeContestants* find(int key);
    int getMax();
    int getMin();
    int getSize();
    pair<int, int> * getInorder();
    void createEmpty(int size);
    void fillEmpty(pair<int, int> *list);
    void fillByValue(pair<int, int> *list, int &min, int &max);
    NodeContestants * getRoot();

private:
    NodeContestants * root;
    int size;
    int maxKey;
    int minKey;
    int findMax();
    int findMin();
    NodeContestants * findAux(int key);
    int findHalf(NodeContestants * ptr);
    void updateNewestPlayer(NodeContestants * toUpdate);
    void updateSumNodes(NodeContestants * toUpdate);
    void fixTree(NodeContestants * start);
    void rotateRR(NodeContestants * toFix);
    void rotateLL(NodeContestants * toFix);
    void rotateRL(NodeContestants * toFix);
    void rotateLR(NodeContestants * toFix);
    void destructorAux(NodeContestants * cur);
    void getInorderAux(pair<int, int> *list, int &i, NodeContestants *cur);  //unite
    void createEmptyAux(NodeContestants *parent, int height); //unite
    void removeTail(int i);
    void removeTailAux(int &i, NodeContestants * cur);
    void fillEmptyAux(NodeContestants *cur, pair<int, int> *list, int &i);
    void fillByValueAux(pair<int, int>* list, int &min, int &max, NodeContestants* cur, int &i);
};


#endif //DS_WET_2_CONTESTANTTREE_H
