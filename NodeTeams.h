//
// Created by JaberA on 17/03/2024.
//

#ifndef DS_WET_2_NODETEAMS_H
#define DS_WET_2_NODETEAMS_H

#include "pair.h"
#include "Team.h"

class NodeTeams {
public:
    virtual ~NodeTeams()= default;
    NodeTeams(): key(pair<int, int>()), value(nullptr), height(0), left(nullptr), right(nullptr), parent(nullptr), medals(0), subTreeSize(1),maxRankSubTree(0){};
    NodeTeams(const pair<int, int>& t_key, Team* value) : key(t_key), value(value), height(0), left(nullptr), right(nullptr), parent(nullptr), medals(0), subTreeSize(1), maxRankSubTree(t_key.getP1()) {
    };
    void setParent(NodeTeams * t_parent);
    void setRight(NodeTeams * t_right);
    void setLeft(NodeTeams * t_left);
    pair<int, int> & getKey();
    void setKey(const pair<int, int>& t_key);
    void setValue(Team* t_value);
    void updateMedals(int newMedals);
    int getMedals();
    void updateSubTreeSize();
    int getSubTreeSize() const;

    NodeTeams * getLeft();
    NodeTeams * getRight();
    NodeTeams * getParent();
    int getHeight() const;
    void updateHeight();
    int getBf();
    Team* getValue();
    int getMaxPowerSubTree() const;
    void updateMaxPowerSubTree();

private:
    pair<int, int> key;
    Team *value;
    int height;
    int medals;
    int subTreeSize;
    int maxRankSubTree;
    NodeTeams * left;
    NodeTeams * right;
    NodeTeams * parent;
//    static int max3(int num1, int num2, int num3);
};


#endif //DS_WET_2_NODETEAMS_H
