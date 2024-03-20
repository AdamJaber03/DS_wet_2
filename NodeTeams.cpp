#include "NodeTeams.h"



void NodeTeams::setValue(Team *t_value) {
    value = t_value;
}

void NodeTeams::setKey(const pair<int, int>& t_key) {
    key = t_key;
}

void NodeTeams::setParent(NodeTeams *t_parent) {
    parent = t_parent;
}

void NodeTeams::setRight(NodeTeams *t_right) {
    right = t_right;
}

void NodeTeams::setLeft(NodeTeams *t_left) {
    left = t_left;
}

void NodeTeams::updateHeight() {
    int lheight = left ? left->height: -1;
    int rheight = right? right->height: -1;
    height = (lheight > rheight ? lheight: rheight) + 1;
}

pair<int, int> &NodeTeams::getKey() {
    return key;
}

NodeTeams *NodeTeams::getLeft() {
    return left;
}

NodeTeams *NodeTeams::getRight() {
    return right;
}

NodeTeams *NodeTeams::getParent() {
    return parent;
}

int NodeTeams::getHeight() const {
    return height;
}

int NodeTeams::getBf() {
    int lheight = left ? left->height: -1;
    int rheight = right? right->height: -1;
    return lheight - rheight;
}

Team* NodeTeams::getValue() {
    return value;
}

void NodeTeams::updateMedals(int newMedals) {
    medals += newMedals;
}

int NodeTeams::getMedals() {
    return medals;
}

void NodeTeams::updateSubTreeSize() {
    subTreeSize = (left? left->getSubTreeSize(): 0) + (right? right->getSubTreeSize(): 0) + 1;
}

int NodeTeams::getSubTreeSize() const {
    return subTreeSize;
}

int NodeTeams::getMaxPowerSubTree() const {
    return maxRankSubTree;
}

void NodeTeams::updateMaxPowerSubTree() {
    maxRankSubTree = max3((left? left->getMaxPowerSubTree(): 0), (right? right->getMaxPowerSubTree(): 0), key.getP1()) + medals;
}

int NodeTeams::max3(int num1, int num2, int num3) {
    if (num1 >= num2){
        if (num1 >= num3) return num1;
        return num3;
    } else{
        if (num2 >= num3) return num2;
        return num3;
    }
}
