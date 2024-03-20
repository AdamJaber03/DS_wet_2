#ifndef DS_WET_2_TEAMSTREE_H
#define DS_WET_2_TEAMSTREE_H
#include "pair.h"
#include "NodeTeams.h"
#include "wet2util.h"
#include <cmath>

class TeamsTree {
public:
    TeamsTree(): root(nullptr), size(0), maxKey(pair<int, int>()), minKey(pair<int, int>()), unique(true) {};
    virtual ~TeamsTree();
    StatusType insert(pair<int, int> key, Team* value, int teamMedals=0);      //pair of power and id, value is team object
    StatusType remove(pair<int, int> & key);       //pair of power and id
    Team * find(pair<int, int> & key);
    StatusType playTournament(int lowPower, int highPower);
    output_t<int> play_match(pair<int, int> team1, pair<int, int> team2);
    void changeUnique();
    pair<int, int> getMax();
    pair<int, int> getMin();
    int getSize();
    pair<pair<int, int>, Team*> * getInorder();
    void createEmpty(int size);
    void fillEmpty(pair<pair<int, int>, Team*> *list);
    //void fillByValue(pair<pair<int, int>, Team*> *list, Team* min, Team* max);
    NodeTeams* getRoot();
    int NumWins(const pair<int, int>& key);
    int getIndex(const pair<int, int>& key);
    void printTree();
    int getMaxRank();


private:
    NodeTeams * root;
    int size;
    bool unique;
    pair<int, int> maxKey;
    pair<int, int> minKey;
    pair<int, int> findMax();
    pair<int, int> findMin();
    NodeTeams * findAux(pair<int, int> & key);
    void fixTree(NodeTeams * start);
    void rotateRR(NodeTeams * toFix);
    void rotateLL(NodeTeams * toFix);
    void rotateRL(NodeTeams * toFix);
    void rotateLR(NodeTeams * toFix);
    void destructorAux(NodeTeams * cur);
    void printTreeAux(NodeTeams* cur);
    void getInorderAux(pair<pair<int, int>, Team*> *list, int &i, NodeTeams *cur);
    void createEmptyAux(NodeTeams *parent, int height);
    void removeTail(int i);
    void removeTailAux(int &i, NodeTeams * cur);

    void fillEmptyAux(NodeTeams *cur, pair<pair<int, int>, Team*> *list, int &i);
    //void fillByValueAux(pair<pair<int, int>, Team*>* list, Team* min, Team* max, NodeTeams* cur, int &i);
    void findIndexUpdateMedals(int index, int numMedals);
    static void fixMaxSubTree(NodeTeams *start);
};


#endif //DS_WET_2_TEAMSTREE_H
