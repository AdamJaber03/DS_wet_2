
#ifndef DS_WET_1_NODE_H
#define DS_WET_1_NODE_H

template <typename T, typename S>
class node{
public:
    virtual ~node()= default;;  //TODO - might be problem;
    node(): key(T()), value(S()), height(0), left(nullptr), right(nullptr), parent(nullptr){};
    node(T key, S value) : key(key), value(value), height(0), left(nullptr), right(nullptr), parent(nullptr) {
    };    //height should be -1
    void setParent(node * parent);
    void setRight(node * right);
    void setLeft(node * left);
    T & getKey();
    void setKey(T key);
    void setValue(S value);

    node * getLeft();
    node * getRight();
    node * getParent();
    int getHeight();
    void updateHeight();
    int getBf();
    S& getValue();

private:
    T key;
    S value;
    int height;
    node * left;
    node * right;
    node * parent;
};

template<typename T, typename S>
void node<T, S>::setValue(S value) {
    this->value = value;
}

template<typename T, typename S>
void node<T, S>::setKey(T key) {
    this->key = key;
}

template<typename T, typename S>
void node<T, S>::setParent(node *parent) {
    this->parent = parent;
}

template<typename T, typename S>
void node<T, S>::setRight(node *right) {
    this->right = right;
}

template<typename T, typename S>
void node<T, S>::setLeft(node *left) {
    this->left = left;
}

template<typename T, typename S>
void node<T, S>::updateHeight() {
    int lheight = left ? left->height: -1;
    int rheight = right? right->height: -1;
    height = (lheight > rheight ? lheight: rheight) + 1;
}

template<typename T, typename S>
T &node<T, S>::getKey() {
    return key;
}

template<typename T, typename S>
node<T, S> *node<T, S>::getLeft() {
    return left;
}

template<typename T, typename S>
node<T, S> *node<T, S>::getRight() {
    return right;
}

template<typename T, typename S>
node<T, S> *node<T, S>::getParent() {
    return parent;
}

template<typename T, typename S>
int node<T, S>::getHeight() {
    return height;
}

template<typename T, typename S>
int node<T, S>::getBf() {
    int lheight = left ? left->height: -1;
    int rheight = right? right->height: -1;
    return lheight - rheight;
}

template<typename T, typename S>
S& node<T, S>::getValue() {
    return value;
}



#endif //DS_WET_1_NODE_H
