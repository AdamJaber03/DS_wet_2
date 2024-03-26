#include "TeamsTree.h"
#include "iostream"

NodeTeams *TeamsTree::getRoot() {
    return root;
}

//void TeamsTree::fillByValueAux(pair<pair<int, int>, Team*> *list, Team* min, Team *max, NodeTeams *cur, int &i) {
//    if (!cur) return;
//    fillByValueAux(list, min, max, cur->getLeft(), i);
//    while (!(min <= list[i].getP2() && list[i].getP2() <= max)){
//        i++;
//    }
//    cur->setKey(list[i].getP1());
//    cur->setValue(list[i].getP2());
//    i++;
//    fillByValueAux(list, min, max, cur->getRight(), i);
//
//}

//void TeamsTree::fillByValue(pair<pair<int, int>, Team*> *list, Team* min, Team* max) {
//    int i = 0;
//    fillByValueAux(list, min, max, root, i);
//    minKey = findMin();
//    maxKey = findMax();
//}

void TeamsTree::removeTailAux(int &i, NodeTeams *cur) {
    if (!cur || !i) return;
    NodeTeams * left = cur->getLeft(), *right = cur->getRight();
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

void TeamsTree::removeTail(int i) {
    removeTailAux(i, root);
}

void TeamsTree::createEmpty(int treeSize) {
    if (root) return;
    if (!treeSize) return;
    int height = ceil(log2(treeSize+1)) - 1;
    root = new NodeTeams();
    createEmptyAux(root, height);
    removeTail((1<<(height + 1)) - 1 - treeSize);
    size = treeSize;
}

void TeamsTree::fillEmpty(pair<pair<int, int>, Team*> * list){
    int i = 0;
    fillEmptyAux(root, list, i);
    minKey = findMin();
    maxKey = findMax();
}

void TeamsTree::fillEmptyAux(NodeTeams * cur, pair<pair<int, int>, Team*> * list, int &i){
    if (!cur) return;
    fillEmptyAux(cur->getLeft(), list, i);
    cur->setKey(list[i].getP1());
    cur->setValue(list[i].getP2());
    i++;
    fillEmptyAux(cur->getRight(), list, i);
}

void TeamsTree::createEmptyAux(NodeTeams* parent, int height){
    if (!height) return;
    NodeTeams *left = new NodeTeams();
    NodeTeams *right = new NodeTeams();
    left->setParent(parent);
    right->setParent(parent);
    parent->setLeft(left);
    parent->setRight(right);
    parent->setHeight(height);
    createEmptyAux(parent->getLeft(), height - 1);
    createEmptyAux(parent->getRight(), height - 1);
}

pair<pair<int, int>, Team*> * TeamsTree::getInorder() {
    pair<pair<int, int>, Team*>* list = new pair<pair<int, int>, Team*>[size];      //TODO - need to delete list
    int i = 0;
    getInorderAux(list, i, root);
    return list;
}

void TeamsTree::getInorderAux(pair<pair<int, int>, Team*>* list, int &i, NodeTeams* cur) {
    if (!cur) return;
    getInorderAux(list, i, cur->getLeft());
    list[i++] = pair<pair<int, int>,Team*>(cur->getKey(), cur->getValue());
    getInorderAux(list, i, cur->getRight());
}

int TeamsTree::getSize() {
    return size;
}

void TeamsTree::printTreeAux(NodeTeams* cur) {
    if (!cur) return;
    printTreeAux(cur->getLeft());
    std::cout << "  (" << cur->getKey().getP1() << ", " <<cur->getKey().getP2() << ") ";
    printTreeAux(cur->getRight());
}

void TeamsTree::printTree() {
    NodeTeams * cur = root;
    printTreeAux(cur);
    std::cout << std::endl;
}


StatusType TeamsTree::insert(pair<int, int> key, Team* value, int teamMedals) {
    NodeTeams * newNode;
    try {
        newNode = new NodeTeams(key, value);
    }catch (std::bad_alloc& err){
        return StatusType::ALLOCATION_ERROR;
    }
    if (!root){
        root = newNode;
        size++;
        newNode->updateMedals(teamMedals);
        newNode->updateSubTreeSize();
        newNode->updateHeight();
        newNode->updateMaxPowerSubTree();
        maxKey = root->getKey();
        minKey = root->getKey();
        return StatusType::SUCCESS;
    }
    if(unique && find(key)){
        delete newNode;
        return StatusType::FAILURE;
    }
    NodeTeams * parent = findAux(key);
    if (key < parent->getKey()){
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }
    newNode->setParent(parent);
    while(parent){
        parent->updateHeight();
        parent->updateSubTreeSize();
        parent = parent->getParent();
    }
    int curMedals = NumWins(key);
    newNode->updateMedals(teamMedals-curMedals);
    newNode->updateMaxPowerSubTree();

    fixTree(newNode);

    size++;
    maxKey = findMax();
    minKey = findMin();
    return StatusType::SUCCESS;
}

NodeTeams *TeamsTree::findAux(pair<int, int> &key) {
    NodeTeams * cur = root, *parent = nullptr;
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

Team *TeamsTree::find(pair<int, int> &key) {   //TODO - this was changed from Team** to Team* make sure this doesnt cause any issues
    NodeTeams * parent = findAux(key);
    if (!parent && root) return root->getValue();
    if (!parent) return nullptr;
    if (parent->getLeft() && parent->getLeft()->getKey() == key){
        return parent->getLeft()->getValue();
    } else if(parent->getRight() && parent->getRight()->getKey() == key){
        return parent->getRight()->getValue();
    }
    return nullptr;
}

void TeamsTree::rotateLL(NodeTeams * toFix) {
    NodeTeams * lSon = toFix->getLeft(), *parent = toFix->getParent();
    NodeTeams * lrson = lSon->getRight();

    // rotation medals correction
    int toFixMedals = toFix->getMedals(), lSonMedals = lSon->getMedals();
    toFix->updateMedals(-(toFixMedals + lSonMedals));
    lSon->updateMedals(toFixMedals);
    if (lrson) lrson->updateMedals(lSonMedals);

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

    if (lrson) lrson->updateMaxPowerSubTree();
    toFix->updateHeight();
    toFix->updateSubTreeSize();
    toFix->updateMaxPowerSubTree();
    lSon->updateHeight();
    lSon->updateSubTreeSize();
    lSon->updateMaxPowerSubTree();
}

void TeamsTree::rotateRR(NodeTeams * toFix) {
    NodeTeams * rSon = toFix->getRight(), *parent = toFix->getParent();
    NodeTeams * rlson = rSon->getLeft();

    // rotation medals correction
    int toFixMedals = toFix->getMedals(), rSonMedals = rSon->getMedals();
    toFix->updateMedals(-(toFixMedals + rSonMedals));
    rSon->updateMedals(toFixMedals);
    if (rlson) rlson->updateMedals(rSonMedals);

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
    if (rlson)rlson->setParent(toFix);

    rSon->setLeft(toFix);
    toFix->setParent(rSon);

    if (rlson) rlson->updateMaxPowerSubTree();
    toFix->updateHeight();
    toFix->updateSubTreeSize();
    toFix->updateMaxPowerSubTree();
    rSon->updateHeight();
    rSon->updateSubTreeSize();
    rSon->updateMaxPowerSubTree();
}

void TeamsTree::rotateLR(NodeTeams * toFix) {
    rotateRR(toFix->getLeft());
    rotateLL(toFix);
}

void TeamsTree::rotateRL(NodeTeams * toFix) {
    rotateLL(toFix->getRight());
    rotateRR(toFix);
}

void TeamsTree::fixTree(NodeTeams *start) {
    NodeTeams * cur = start;
    while (cur){
        cur->updateHeight();
        cur->updateSubTreeSize();
        cur->updateMaxPowerSubTree();
        int bf = cur->getBf();

        if (abs(bf) < 2){
            cur = cur->getParent();
            continue;
        }
        if (bf == 2){
            if (cur->getLeft()->getBf() >= 0){
                rotateLL(cur);
            } else{
                rotateLR(cur);
            }
        } else {
            if (cur->getRight()->getBf() <= 0) {
                rotateRR(cur);
            } else {
                rotateRL(cur);
            }
        }
        cur = cur->getParent();
    }
}


StatusType TeamsTree::remove(pair<int, int> &key) {
    NodeTeams * parent = findAux(key);
    NodeTeams * toRemove = nullptr;
    NodeTeams * startfix = nullptr;
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
    NodeTeams * lson = toRemove->getLeft(), * rson = toRemove->getRight();
    //if toRemove is a leaf
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
    //if toRemove only has one son
    else if ((rson && !lson) || (!rson && lson)){
        NodeTeams * replacement = lson? lson: rson;
        replacement->updateMedals(toRemove->getMedals());
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
        startfix = replacement;
        delete toRemove;
    } // toRemove has two sons
    else{
        bool first = true;
        NodeTeams * curSmall = rson;
        while (curSmall->getLeft()){
            first = false;
            curSmall = curSmall->getLeft();
        }
        startfix = curSmall->getParent();
        pair<int, int> tempKey = curSmall->getKey();
        Team* tempValue = curSmall->getValue();
        NodeTeams * smlParent = curSmall->getParent(), *smlrSon = curSmall->getRight();

        // This is Medals correctness algorithm in removal
        int toRemoveWins = NumWins(key), curSmallWins = NumWins(curSmall->getKey());
        toRemove->updateMedals(curSmallWins - toRemoveWins);
        rson->updateMedals(toRemoveWins - curSmallWins);
        rson->updateMaxPowerSubTree();
        lson->updateMedals(toRemoveWins - curSmallWins);
        lson->updateMaxPowerSubTree();
        if (smlrSon){
            smlrSon->updateMedals(curSmall->getMedals());
            smlrSon->updateMaxPowerSubTree();
        }


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

void TeamsTree::changeUnique() {
    unique = !unique;
}

TeamsTree::~TeamsTree() {
    destructorAux(root);
}

void TeamsTree::destructorAux(NodeTeams *cur) {
    if (!cur) return;
    destructorAux(cur->getLeft());
    destructorAux(cur->getRight());
    delete cur;
}

pair<int, int> TeamsTree::findMax() {
    if (!root) return pair<int, int>();
    NodeTeams *cur = root;
    while (cur->getRight()){
        cur = cur->getRight();
    }
    return cur->getKey();
}

pair<int, int> TeamsTree::findMin() {
    if (!root) return pair<int, int>();
    NodeTeams *cur = root;
    while (cur->getLeft()){
        cur = cur->getLeft();
    }
    return cur->getKey();
}

pair<int, int> TeamsTree::getMax() {
    return maxKey;
}

pair<int, int> TeamsTree::getMin() {
    return minKey;
}

output_t<int> TeamsTree::playTournament(int lowPower, int highPower) {
    //TODO - check this algorithem i think there are small mistakes here
    NodeTeams *lowest = nullptr;
    NodeTeams *current = root;
    while (current){
        int cur_power = current->getKey().getP1();
        if (lowPower <= cur_power){
            lowest = current;
            current = current->getLeft();
        } else{
            current = current->getRight();
        }
    }
    NodeTeams *highest = nullptr;
    current = root;
    while (current){
        int cur_power = current->getKey().getP1();
        if (highPower >= cur_power){
            highest = current;
            current = current->getRight();
        } else{
            current = current->getLeft();
        }
    }
    if (!highest || !lowest) return StatusType::FAILURE;
    //at this point highest should contain the greatest node with power up to highPower

    int numTeams = getIndex(highest->getKey()) - getIndex(lowest->getKey()) + 1;
    if (numTeams < 1) return StatusType::FAILURE;  // if no tems in tournment or only 1 team
    if (numTeams == 1) return lowest->getKey().getP2();
    if ((numTeams & (numTeams - 1)) != 0) return StatusType::FAILURE;       //check if power of 2 with bitwise and. powers of two are 1000...
    int startInd = getIndex(lowest->getKey());
    int tot_medals = 0;
    while (numTeams/2){
        tot_medals += 1;
        numTeams /= 2;
        findIndexUpdateMedals(startInd + numTeams - 1, 1);
        startInd += numTeams;
    }
    findIndexUpdateMedals(startInd, -tot_medals);    //fix medals for everyone greater then highest
    return highest->getKey().getP2();
}

void TeamsTree::findIndexUpdateMedals(int index, int numMedals) {
    NodeTeams* current = root;
    int tempInd = 0;
    while (current){
        int currentInd = tempInd + (current->getLeft()? current->getLeft()->getSubTreeSize(): 0) + 1;     //maybe not +1, currently counting index from 1 to n. without +1 counting from 0 to n-1
        if (currentInd == index){
            if (current->getRight()) current->getRight()->updateMedals(numMedals);
            break;
        }else if (currentInd < index){
            current = current->getRight();
            tempInd = currentInd;
        } else{
            current->updateMedals(numMedals);
            if (current->getLeft()) current->getLeft()->updateMedals(-numMedals);
            current = current->getLeft();
        }
    }
    //fix max on route from index to root, only for those nodes there was a change in maxPower in their subtree
    fixMaxSubTree(current->getRight()? current->getRight(): current);
}

int TeamsTree::NumWins(const pair<int, int>& key) {
    NodeTeams * current = root;
    int wins = 0;
    while (!(current->getKey() == key)){
        wins += current->getMedals();
        if (current->getKey() > key){
            current = current->getLeft();
        }else{
            current = current->getRight();
        }
    }
    wins += current->getMedals();
    return wins;
}

int TeamsTree::getIndex(const pair<int, int> & key) {
    NodeTeams *current = root;
    int index = 0;
    while (current){
        if (key == current->getKey()){
            index += 1 + (current->getLeft()? current->getLeft()->getSubTreeSize(): 0);
            break;
        }
        if (key > current->getKey()){
            index += 1 + (current->getLeft()? current->getLeft()->getSubTreeSize(): 0);
            current = current->getRight();
        } else{
            current = current->getLeft();
        }
    }
    return index;
}

int TeamsTree::getMaxRank() {
    return root? root->getMaxPowerSubTree(): 0;
}

void TeamsTree::fixMaxSubTree(NodeTeams *start) {
    NodeTeams* curr = start;
    while (curr){
        curr->updateMaxPowerSubTree();
        curr = curr->getParent();
    }
}

output_t<int> TeamsTree::play_match(pair<int, int> team1, pair<int, int> team2) {
    int index1 = getIndex(team1), index2 = getIndex(team2);
    if (index1 > index2){
        findIndexUpdateMedals(index1-1, 1);
        findIndexUpdateMedals(index1, -1);
        return output_t<int>(team1.getP2());
    }else{
        findIndexUpdateMedals(index2-1, 1);
        findIndexUpdateMedals(index2, -1);
        return output_t<int>(team2.getP2());
    }
}
