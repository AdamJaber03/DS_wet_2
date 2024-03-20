//
// Created by amir on 17/03/2024.
//

#include "NodeContestants.h"

void NodeContestants::setValue(int value) {
    this->value = value;
}

void NodeContestants::setKey(int key) {
    this->key = key;
}

void NodeContestants::setParent(NodeContestants *parent) {
    this->parent = parent;
}

void NodeContestants::setRight(NodeContestants *right) {
    this->right = right;
}

void NodeContestants::setLeft(NodeContestants *left) {
    this->left = left;
}

void NodeContestants::updateHeight() {
    int lheight = left ? left->height: -1;
    int rheight = right? right->height: -1;
    height = (lheight > rheight ? lheight: rheight) + 1;
}

int NodeContestants::getKey() {
    return key;
}

NodeContestants *NodeContestants::getLeft() {
    return left;
}

NodeContestants *NodeContestants::getRight() {
    return right;
}

NodeContestants *NodeContestants::getParent() {
    return parent;
}

int NodeContestants::getHeight() {
    return height;
}

int NodeContestants::getBf() {
    int lheight = left ? left->height: -1;
    int rheight = right? right->height: -1;
    return lheight - rheight;
}

int NodeContestants::getValue() {
    return value;
}


#include "NodeContestants.h"

int NodeContestants::getNewestPlayer() {
    return getNewestPlayer();
}

int NodeContestants::getSumNodes() {
    return sumNodes;
}

void NodeContestants::setNewestPlayer(int newestPlayer) {
    this->newestPlayer = newestPlayer;
}

void NodeContestants::setSumNodes(int sumNodes) {
    this->sumNodes = sumNodes;
}
