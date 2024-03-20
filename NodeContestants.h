//
// Created by amir on 17/03/2024.
//

#ifndef DS_WET_2_NODECONTESTANTS_H
#define DS_WET_2_NODECONTESTANTS_H

#include "pair.h"

class NodeContestants {
public:
    virtual ~NodeContestants()= default;;  //TODO - might be problem;
    NodeContestants(): key(0), value(0), height(0),newestPlayer(0), sumNodes(0), left(nullptr), right(nullptr), parent(nullptr){};;
    NodeContestants(int key, int value) : key(key), value(value), height(0), newestPlayer(value), sumNodes(1), left(nullptr), right(nullptr), parent(nullptr) {
    }
    void setParent(NodeContestants * parent);
    void setRight(NodeContestants * right);
    void setLeft(NodeContestants * left);
    int getKey();
    void setKey(int key);
    void setValue(int value);
    int getNewestPlayer();
    void setNewestPlayer(int newestPlayer);
    int getSumNodes();
    void setSumNodes(int sumNodes);
    NodeContestants * getLeft();
    NodeContestants * getRight();
    NodeContestants * getParent();
    int getHeight();
    void updateHeight();
    int getBf();
    int getValue();

private:
    int key;
    int value;
    int height;
    int newestPlayer;
    int sumNodes;
    NodeContestants * left;
    NodeContestants * right;
    NodeContestants * parent;
};



#endif //DS_WET_2_NODECONTESTANTS_H
