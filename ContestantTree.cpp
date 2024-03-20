//
// Created by amir on 17/03/2024.
//

#include "ContestantTree.h"

NodeContestants *ContestantTree::getRoot() {
    return root;
}

void ContestantTree::fillByValueAux(pair<int, int> *list, int &min, int &max, NodeContestants *cur, int &i) {
    if (!cur) return;
    fillByValueAux(list, min, max, cur->getLeft(), i);
    while (!(min <= list[i].getP2() && list[i].getP2() <= max)){
        i++;
    }
    cur->setKey(list[i].getP1());
    cur->setValue(list[i].getP2());
    i++;
    fillByValueAux(list, min, max, cur->getRight(), i);

}

void ContestantTree::fillByValue(pair<int, int> *list, int& min, int& max) {
    int i = 0;
    fillByValueAux(list, min, max, root, i);
    minKey = findMin();
    maxKey = findMax();
}

void ContestantTree::removeTailAux(int &i, NodeContestants *cur) {
    if (!cur || !i) return;
    NodeContestants * left = cur->getLeft(), *right = cur->getRight();
    removeTailAux(i, right);

    if (!cur->getLeft() && !cur->getRight()){
        if (cur->getParent()->getRight()){
            cur->getParent()->setRight(nullptr);
        }else{
            cur->getParent()->setLeft(nullptr);
        }
        delete cur;
        i--;
    }
    removeTailAux(i, left);
}

void ContestantTree::removeTail(int i) {
    removeTailAux(i, root);
}

void ContestantTree::createEmpty(int treeSize) {
    if (root) return;
    if (!treeSize) return;
    int height = ceil(log2(treeSize+1)) - 1;
    root = new NodeContestants();
    createEmptyAux(root, height);
    removeTail((1<<(height + 1)) - 1 - treeSize);
    size = treeSize;
}

void ContestantTree::fillEmpty(pair<int, int> * list){
    int i = 0;
    fillEmptyAux(root, list, i);
    minKey = findMin();
    maxKey = findMax();
}

void ContestantTree::fillEmptyAux(NodeContestants * cur, pair<int, int> * list, int &i){
    if (!cur) return;
    fillEmptyAux(cur->getLeft(), list, i);
    cur->setKey(list[i].getP1());
    cur->setValue(list[i].getP2());
    i++;
    fillEmptyAux(cur->getRight(), list, i);
}

void ContestantTree::createEmptyAux(NodeContestants * parent, int height){
    if (!height) return;
    NodeContestants *left = new NodeContestants();
    NodeContestants *right = new NodeContestants();
    left->setParent(parent);
    right->setParent(parent);
    parent->setLeft(left);
    parent->setRight(right);
    createEmptyAux(parent->getLeft(), height - 1);
    createEmptyAux(parent->getRight(), height - 1);
}

pair<int, int> * ContestantTree::getInorder() {
    pair<int, int>* list = new pair<int, int>[size];      //TODO - need to delete list
    int i = 0;
    getInorderAux(list, i, root);
    return list;
}

void ContestantTree::getInorderAux(pair<int, int>* list, int &i, NodeContestants * cur) {
    if (!cur) return;
    getInorderAux(list, i, cur->getLeft());
    list[i++] = pair<int,int>(cur->getKey(), cur->getValue());
    getInorderAux(list, i, cur->getRight());
}

int ContestantTree::getSize() {
    return size;
}

StatusType ContestantTree::insert(int key, int value) {
    NodeContestants * newNode;
    try {
        newNode = new NodeContestants(key, value);
    }catch (std::bad_alloc& err){
        return StatusType::ALLOCATION_ERROR;
    }
    if (!root){
        root = newNode;
        size++;
        maxKey = root->getKey();
        minKey = root->getKey();
        return StatusType::SUCCESS;
    }
    if(find(key)){
        delete newNode;
        return StatusType::FAILURE;
    }
    NodeContestants * parent = findAux(key);
    if (key < parent->getKey()){
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }
    newNode->setParent(parent);
    while(parent){
        parent->updateHeight();
        parent = parent->getParent();
    }
    fixTree(newNode);

    size++;
    maxKey = findMax();
    minKey = findMin();
    return StatusType::SUCCESS;
}

NodeContestants *ContestantTree::findAux(int key) {
    NodeContestants * cur = root, *parent = nullptr;
    while (cur){
        if (cur->getKey() == key){
            return parent;
        }
        if (key < cur->getKey()){
            parent = cur;
            cur = cur->getLeft();
        } else{
            parent = cur;
            cur = cur->getRight();
        }
    }
    return parent;
}

NodeContestants *ContestantTree::find(int key) {
    NodeContestants * parent = findAux(key);
    if (!parent && root) return root;
    if (!parent) return nullptr;
    if (parent->getLeft() && parent->getLeft()->getKey() == key){
        return parent->getLeft();
    } else if(parent->getRight() && parent->getRight()->getKey() == key){
        return parent->getRight();
    }
    return nullptr;
}

void ContestantTree::rotateLL(NodeContestants * toFix) {
    NodeContestants * lSon = toFix->getLeft(), *parent = toFix->getParent();
    NodeContestants * lrson = lSon->getRight();
    //update fields:
    if(lrson){
        int sumNodes = toFix->getSumNodes();
        toFix->setSumNodes(toFix->getSumNodes() - (lSon->getSumNodes() - lSon->getRight()->getSumNodes()));
        lSon->setSumNodes(sumNodes);
        if(toFix->getRight()){ //update toFix newest player
            if(toFix->getRight()->getNewestPlayer() > lrson->getNewestPlayer()) toFix->setNewestPlayer(toFix->getRight()->getNewestPlayer());
            else toFix->setNewestPlayer(lrson->getNewestPlayer());
        }
        else{
            toFix->setNewestPlayer(lrson->getNewestPlayer());
        }
    }
    else{
        int sumNodes = toFix->getSumNodes();
        toFix->setSumNodes(toFix->getSumNodes() - lSon->getSumNodes());
        lSon->setSumNodes(sumNodes);
        if(toFix->getRight()) toFix->setNewestPlayer(toFix->getRight()->getNewestPlayer());
        else toFix->setNewestPlayer(toFix->getNewestPlayer());
    }
    if(lSon->getNewestPlayer() < toFix->getNewestPlayer()) lSon->setNewestPlayer(toFix->getNewestPlayer());
    else lSon->setNewestPlayer(lSon->getLeft()->getNewestPlayer());

    //pside is True if tofix is left son
    if (parent){
        bool pside = toFix->getParent()->getKey() > toFix->getKey();
        if (pside){
            parent->setLeft(lSon);
        } else{
            parent->setRight(lSon);
        }
    }else{
        root = lSon;
    }
    lSon->setParent(parent);

    toFix->setLeft(lrson);
    if (lrson) lrson->setParent(toFix);

    lSon->setRight(toFix);
    toFix->setParent(lSon);

    toFix->updateHeight();
    lSon->updateHeight();

}

void ContestantTree::rotateRR(NodeContestants * toFix) {
    NodeContestants * rSon = toFix->getRight(), *parent = toFix->getParent();
    NodeContestants * rlson = rSon->getLeft();

    //update fields:
    if(rlson){
        int sumNodes = toFix->getSumNodes();
        toFix->setSumNodes(toFix->getSumNodes() - (rSon->getSumNodes() - rSon->getLeft()->getSumNodes()));
        rSon->setSumNodes(sumNodes);
        if(toFix->getLeft()){ //update toFix newest player
            if(toFix->getLeft()->getNewestPlayer() > rlson->getNewestPlayer()) toFix->setNewestPlayer(toFix->getLeft()->getNewestPlayer());
            else toFix->setNewestPlayer(rlson->getNewestPlayer());
        }
        else{
            toFix->setNewestPlayer(rlson->getNewestPlayer());
        }
    }
    else{
        int sumNodes = toFix->getSumNodes();
        toFix->setSumNodes(toFix->getSumNodes() - rSon->getSumNodes());
        rSon->setSumNodes(sumNodes);
        if(toFix->getLeft()) toFix->setNewestPlayer(toFix->getLeft()->getNewestPlayer());
        else toFix->setNewestPlayer(toFix->getNewestPlayer());
    }
    if(rSon->getNewestPlayer() < toFix->getNewestPlayer()) rSon->setNewestPlayer(toFix->getNewestPlayer());
    else rSon->setNewestPlayer(rSon->getRight()->getNewestPlayer());

    //pside is True if tofix is left son
    if (parent){
        bool pside = toFix->getParent()->getKey() > toFix->getKey();
        if (pside){
            parent->setLeft(rSon);
        } else{
            parent->setRight(rSon);
        }
    } else{
        root = rSon;
    }
    rSon->setParent(parent);

    toFix->setRight(rlson);
    if (rlson) rlson->setParent(toFix);

    rSon->setLeft(toFix);
    toFix->setParent(rSon);

    toFix->updateHeight();
    rSon->updateHeight();
}

void ContestantTree::rotateLR(NodeContestants * toFix) {
    rotateRR(toFix->getLeft());
    rotateLL(toFix);
}

void ContestantTree::rotateRL(NodeContestants * toFix) {
    rotateLL(toFix->getRight());
    rotateRR(toFix);
}

void ContestantTree::fixTree(NodeContestants *start) {
    NodeContestants * cur = start;
    while (cur){
        cur->updateHeight();
        updateNewestPlayer(cur);
        updateSumNodes(cur);
        int bf = cur->getBf();

        if (abs(bf) < 2){
            cur = cur->getParent();
            continue;
        }
        if (bf == 2){
            if (cur->getLeft()->getBf() >= 0){ // Todo understand if 0 or 1
                rotateLL(cur);
            } else{
                rotateLR(cur);
            }
        } else {
            if (cur->getRight()->getBf() <= 0) { // TODO - understand if 0 or -1
                rotateRR(cur);
            } else {
                rotateRL(cur);
            }
        }
        cur = cur->getParent();
    }
}

/*
StatusType ContestantTree::remove(int &key) {
    NodeContestants * parent = findAux(key);
    NodeContestants * toRemove = nullptr;
    NodeContestants * startfix = nullptr;
    if (!parent){
        //if doesnt exist return failure
        if (!root) return StatusType::FAILURE;
        toRemove = root;
    }
    else if (!parent->getLeft() && !parent->getRight()){
        return StatusType::FAILURE;
    }else{
        bool pside = parent->getKey() > key;
        toRemove = pside? parent->getLeft(): parent->getRight();
    }
    NodeContestants * lson = toRemove->getLeft(), * rson = toRemove->getRight();
    //if toRemove only has one son
    if (!rson && !lson){
        bool isRoot = (toRemove == root);
        if (isRoot){
            root = nullptr;
        }else{
            bool pside = parent->getKey() > key;
            if (pside){
                parent->setLeft(nullptr);
            } else{
                parent->setRight(nullptr);
            }
        }
        startfix = parent;
        delete toRemove;
    }
    else if ((rson && !lson) || (!rson && lson)){
        delete toRemove;
        NodeContestants * replacement = lson? lson: rson;
        if (toRemove == root){
            root = replacement;
            replacement->setParent(nullptr);
        }else{
            bool pside = parent->getKey() > key;
            if (pside){
                parent->setLeft(replacement);
            }else{
                parent->setRight(replacement);
            }
            replacement->setParent(parent);
        }
        startfix = parent;
    }
    else{
        bool first = true;
        NodeContestants * curSmall = rson;
        while (curSmall->getLeft()){
            first = false;
            curSmall = curSmall->getLeft();
        }
        startfix = curSmall->getParent();
        int tempKey = curSmall->getKey();
        int tempValue = curSmall->getValue();
        NodeContestants * smlParent = curSmall->getParent(), *smlrSon = curSmall->getRight();
        delete curSmall;
        toRemove->setKey(tempKey);
        toRemove->setValue(tempValue);
        if (first){
            smlParent->setRight(smlrSon);
        }else{
            smlParent->setLeft(smlrSon);
        }
        if (smlrSon) smlrSon->setParent(smlParent);
    }
    fixTree(startfix);
    size--;
    maxKey = findMax(); //TODO : find why we need this
    minKey = findMin();
    return StatusType::SUCCESS;
}*/

void ContestantTree::updateNewestPlayer(NodeContestants *toUpdate) {
    if(toUpdate->getRight() && toUpdate->getLeft()){
        if(toUpdate->getRight()->getNewestPlayer() > toUpdate->getLeft()->getNewestPlayer()) toUpdate->setNewestPlayer(toUpdate->getRight()->getNewestPlayer());
        else toUpdate->setNewestPlayer(toUpdate->getLeft()->getNewestPlayer());
    }
    else if(toUpdate->getRight()){
        toUpdate->setNewestPlayer(toUpdate->getRight()->getValue());
    }
    else if(toUpdate->getLeft()){
        toUpdate->setNewestPlayer(toUpdate->getLeft()->getNewestPlayer());
    }
    else toUpdate->setNewestPlayer(toUpdate->getValue());

}

void ContestantTree::updateSumNodes(NodeContestants *toUpdate) {
    if(toUpdate->getRight() && toUpdate->getLeft()){
        toUpdate->setSumNodes(toUpdate->getRight()->getSumNodes() + toUpdate->getLeft()->getSumNodes() + 1);
    }
    else if(toUpdate->getRight()){
        toUpdate->setNewestPlayer(toUpdate->getRight()->getSumNodes() + 1);
    }
    else if(toUpdate->getLeft()){
        toUpdate->setNewestPlayer(toUpdate->getLeft()->getSumNodes() + 1);
    }
    else toUpdate->setNewestPlayer(1);

}


ContestantTree::~ContestantTree() {
    destructorAux(root);
}

void ContestantTree::destructorAux(NodeContestants *cur) {
    if (!cur) return;
    destructorAux(cur->getLeft());
    destructorAux(cur->getRight());
    delete cur;
}

int ContestantTree::findMax() {
    if (!root) return -1; //TODO : to check where i call this function
    NodeContestants *cur = root;
    while (cur->getRight()){
        cur = cur->getRight();
    }
    return cur->getKey();
}

int ContestantTree::findMin() {
    if (!root) return -1; //TODO : to check where i call this function
    NodeContestants *cur = root;
    while (cur->getLeft()){
        cur = cur->getLeft();
    }
    return cur->getKey();
}

int ContestantTree::getMax() {
    return maxKey;
}

int ContestantTree::getMin() {
    return minKey;
}

NodeContestants * ContestantTree::strengthNewestPlayer(NodeContestants * ptr, int id) {
    if(!ptr->getRight() && !ptr->getLeft()) return nullptr;
    if(ptr->getRight()->getValue() == id || ptr->getLeft()->getValue()) return ptr;
    if(ptr->getRight() && ptr->getRight()->getNewestPlayer() == id) return strengthNewestPlayer(ptr->getRight(), id);
    else return strengthNewestPlayer(ptr->getLeft(), id);
}


StatusType ContestantTree::remove() {
    NodeContestants * parent = strengthNewestPlayer(root, root->getNewestPlayer());
    NodeContestants * toRemove = nullptr;
    NodeContestants * startfix = nullptr;
    int key;
    if (!parent){
        //if doesnt exist return failure
        if (!root) return StatusType::FAILURE;
        toRemove = root;
        key = root->getKey();
    }
    else if (!parent->getLeft() && !parent->getRight()){
        return StatusType::FAILURE;
    }else{
        if(parent->getRight() && parent->getRight()->getNewestPlayer() == parent->getNewestPlayer()){
            key = parent->getRight()->getKey();
            toRemove = parent->getRight();
        }
        else{
            key = parent->getLeft()->getKey();
            toRemove = parent->getLeft();
        }
    }
    NodeContestants * lson = toRemove->getLeft(), * rson = toRemove->getRight();
    //if toRemove only has one son
    if (!rson && !lson){
        bool isRoot = (toRemove == root);
        if (isRoot){
            root = nullptr;
        }else{
            bool pside = parent->getKey() > key;
            if (pside){
                parent->setLeft(nullptr);
            } else{
                parent->setRight(nullptr);
            }
        }
        startfix = parent;
        delete toRemove;
    }
    else if ((rson && !lson) || (!rson && lson)){
        delete toRemove;
        NodeContestants * replacement = lson? lson: rson;
        if (toRemove == root){
            root = replacement;
            replacement->setParent(nullptr);
        }else{
            bool pside = parent->getKey() > key;
            if (pside){
                parent->setLeft(replacement);
            }else{
                parent->setRight(replacement);
            }
            replacement->setParent(parent);
        }
        startfix = parent;
    }
    else{
        bool first = true;
        NodeContestants * curSmall = rson;
        while (curSmall->getLeft()){
            first = false;
            curSmall = curSmall->getLeft();
        }
        startfix = curSmall->getParent();
        int tempKey = curSmall->getKey();
        int tempValue = curSmall->getValue();
        NodeContestants * smlParent = curSmall->getParent(), *smlrSon = curSmall->getRight();
        delete curSmall;
        toRemove->setKey(tempKey);
        toRemove->setValue(tempValue);
        if (first){
            smlParent->setRight(smlrSon);
        }else{
            smlParent->setLeft(smlrSon);
        }
        if (smlrSon) smlrSon->setParent(smlParent);
    }
    fixTree(startfix);
    size--;
    maxKey = findMax(); //TODO : find why we need this
    minKey = findMin();
    return StatusType::SUCCESS;
}

int ContestantTree::findHalf(NodeContestants *ptr) {
    if(!ptr) return -1;

}




#include "ContestantTree.h"
