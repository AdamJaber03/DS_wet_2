
#ifndef DS_WET_1_AVL_H
#define DS_WET_1_AVL_H

#include "node.h"
#include "wet2util.h"
#include <cmath>
#include <iostream>
#include "pair.h"

template <typename T, typename S>
class avl{
public:
    avl(): root(nullptr), size(0), maxKey(T()), minKey(T()), unique(true) {};
    virtual ~avl();
    StatusType insert(T key, S value);
    StatusType remove(T & key);
    S * find(T & key);
    void changeUnique();
    T getMax();
    T getMin();
    int getSize();
    pair<T, S> * getInorder();
    void createEmpty(int size);
    void fillEmpty(pair<T, S> *list);
    void fillByValue(pair<T, S> *list, const S& min, const S& max);
    node<T, S>* getRoot();

private:
    node<T, S> * root;
    int size;
    bool unique;
    T maxKey;
    T minKey;
    T findMax();
    T findMin();
    node<T, S> * findAux(T & key);
    void fixTree(node<T, S> * start);
    void rotateRR(node<T, S> * toFix);
    void rotateLL(node<T, S> * toFix);
    void rotateRL(node<T, S> * toFix);
    void rotateLR(node<T, S> * toFix);
    void destructorAux(node<T, S> * cur);
//    void printTree();
//    void printTreeAux(node<T, S>* cur);
    void getInorderAux(pair<T, S> *list, int &i, node<T, S> *cur);
    void createEmptyAux(node<T, S> *parent, int height);
    void removeTail(int i);
    void removeTailAux(int &i, node<T, S> * cur);

    void fillEmptyAux(node<T, S> *cur, pair<T, S> *list, int &i);
    void fillByValueAux(pair<T, S>* list, const S &min, const S &max, node<T, S>* cur, int &i);
};

template<typename T, typename S>
node<T, S> *avl<T, S>::getRoot() {
    return root;
}

template<typename T, typename S>
void avl<T, S>::fillByValueAux(pair<T, S> *list, const S &min, const S &max, node<T, S> *cur, int &i) {
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

template<typename T, typename S>
void avl<T, S>::fillByValue(pair<T, S> *list, const S& min, const S& max) {
    int i = 0;
    fillByValueAux(list, min, max, root, i);
    minKey = findMin();
    maxKey = findMax();
}

template<typename T, typename S>
void avl<T, S>::removeTailAux(int &i, node<T, S> *cur) {
    if (!cur || !i) return;
    node<T, S> * left = cur->getLeft(), *right = cur->getRight();
    removeTailAux(i, right);

    if (!cur->getLeft() && !cur->getRight()){
        if (cur->getParent()->getRight()){
            cur->getParent()->setRight(nullptr);
        }else{
            cur->getParent()->setLeft(nullptr);
        }
        delete cur;
        cur = nullptr;
        i--;
    }
    removeTailAux(i, left);
    if (cur) cur->updateHeight();
}

template<typename T, typename S>
void avl<T, S>::removeTail(int i) {
    removeTailAux(i, root);
}

template<typename T, typename S>
void avl<T, S>::createEmpty(int treeSize) {
    if (root) return;
    if (!treeSize) return;
    int height = ceil(log2(treeSize+1)) - 1;
    root = new node<T, S>();
    createEmptyAux(root, height);
    removeTail((1<<(height + 1)) - 1 - treeSize);
    size = treeSize;
}

template<typename T, typename S>
void avl<T, S>::fillEmpty(pair<T, S> * list){
    int i = 0;
    fillEmptyAux(root, list, i);
    minKey = findMin();
    maxKey = findMax();
}

template<typename T, typename S>
void avl<T, S>::fillEmptyAux(node<T, S> * cur, pair<T, S> * list, int &i){
    if (!cur) return;
    fillEmptyAux(cur->getLeft(), list, i);
    cur->setKey(list[i].getP1());
    cur->setValue(list[i].getP2());
    i++;
    fillEmptyAux(cur->getRight(), list, i);
}

template<typename T, typename S>
void avl<T, S>::createEmptyAux(node<T, S>* parent, int height){
    if (!height) return;
    node<T, S> *left = new node<T, S>();
    node<T, S> *right = new node<T, S>();
    left->setParent(parent);
    right->setParent(parent);
    parent->setLeft(left);
    parent->setRight(right);
    parent->setHeight(height);
    createEmptyAux(parent->getLeft(), height - 1);
    createEmptyAux(parent->getRight(), height - 1);
}

template<typename T, typename S>
pair<T, S> * avl<T, S>::getInorder() {
    pair<T, S>* list = new pair<T, S>[size];      //TODO - need to delete list
    int i = 0;
    getInorderAux(list, i, root);
    return list;
}

template<typename T, typename S>
void avl<T, S>::getInorderAux(pair<T, S>* list, int &i, node<T, S>* cur) {
    if (!cur) return;
    getInorderAux(list, i, cur->getLeft());
    list[i++] = pair<T,S>(cur->getKey(), cur->getValue());
    getInorderAux(list, i, cur->getRight());
}

template<typename T, typename S>
int avl<T, S>::getSize() {
    return size;
}

//template<typename T, typename S>
//void avl<T, S>::printTreeAux(node<T, S>* cur) {
//    if (!cur) return;
//    printTreeAux(cur->getLeft());
//    std::cout << " " << cur->getKey() << " ";
//    printTreeAux(cur->getRight());
//}

//template<typename T, typename S>
//void avl<T, S>::printTree() {
//    node<T, S> * cur = root;
//    printTreeAux(cur);
//    std::cout << std::endl;
//}


template<typename T, typename S>
StatusType avl<T, S>::insert(T key, S value) {
    node<T, S> * newNode;
    try {
        newNode = new node<T, S>(key, value);
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
    if(unique && find(key)){
        delete newNode;
        return StatusType::FAILURE;
    }
    node<T, S> * parent = findAux(key);
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

template<typename T, typename S>
node<T, S> *avl<T, S>::findAux(T &key) {
    node<T, S> * cur = root, *parent = nullptr;
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

template<typename T, typename S>
S *avl<T, S>::find(T &key) {
    node<T, S> * parent = findAux(key);
    if (!parent && root) return &root->getValue();
    if (!parent) return nullptr;
    if (parent->getLeft() && parent->getLeft()->getKey() == key){
        return &parent->getLeft()->getValue();
    } else if(parent->getRight() && parent->getRight()->getKey() == key){
        return &parent->getRight()->getValue();
    }
    return nullptr;
}

template<typename T, typename S>
void avl<T, S>::rotateLL(node<T, S> * toFix) {
    node<T, S> * lSon = toFix->getLeft(), *parent = toFix->getParent();
    node<T, S> * lrson = lSon->getRight();
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

template<typename T, typename S>
void avl<T, S>::rotateRR(node<T, S> * toFix) {
    node<T, S> * rSon = toFix->getRight(), *parent = toFix->getParent();
    node<T, S> * rlson = rSon->getLeft();
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

template<typename T, typename S>
void avl<T, S>::rotateLR(node<T, S> * toFix) {
    rotateRR(toFix->getLeft());
    rotateLL(toFix);
}

template<typename T, typename S>
void avl<T, S>::rotateRL(node<T, S> * toFix) {
    rotateLL(toFix->getRight());
    rotateRR(toFix);
}

template<typename T, typename S>
void avl<T, S>::fixTree(node<T, S> *start) {
    node<T, S> * cur = start;
    while (cur){
        cur->updateHeight();
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


template<typename T, typename S>
StatusType avl<T, S>::remove(T &key) {
    node<T, S> * parent = findAux(key);
    node<T, S> * toRemove = nullptr;
    node<T, S> * startfix = nullptr;
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
    node<T, S> * lson = toRemove->getLeft(), * rson = toRemove->getRight();
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
        node<T, S> * replacement = lson? lson: rson;
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
        node<T, S> * curSmall = rson;
        while (curSmall->getLeft()){
            first = false;
            curSmall = curSmall->getLeft();
        }
        startfix = curSmall->getParent();
        T tempKey = curSmall->getKey();
        S tempValue = curSmall->getValue();
        node<T, S> * smlParent = curSmall->getParent(), *smlrSon = curSmall->getRight();
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
    maxKey = findMax();
    minKey = findMin();
    return StatusType::SUCCESS;
}

template<typename T, typename S>
void avl<T, S>::changeUnique() {
    unique = !unique;
}

template<typename T, typename S>
avl<T, S>::~avl() {
    destructorAux(root);
}

template<typename T, typename S>
void avl<T, S>::destructorAux(node<T, S> *cur) {
    if (!cur) return;
    destructorAux(cur->getLeft());
    destructorAux(cur->getRight());
    delete cur;
}

template<typename T, typename S>
T avl<T, S>::findMax() {
    if (!root) return T();
    node<T, S> *cur = root;
    while (cur->getRight()){
        cur = cur->getRight();
    }
    return cur->getKey();
}

template<typename T, typename S>
T avl<T, S>::findMin() {
    if (!root) return T();
    node<T, S> *cur = root;
    while (cur->getLeft()){
        cur = cur->getLeft();
    }
    return cur->getKey();
}

template<typename T, typename S>
T avl<T, S>::getMax() {
    return maxKey;
}

template<typename T, typename S>
T avl<T, S>::getMin() {
    return minKey;
}


#endif //DS_WET_1_AVL_H
