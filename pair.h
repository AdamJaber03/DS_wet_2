

#ifndef DS_WET_1_PAIR_H
#define DS_WET_1_PAIR_H
template <typename T, typename S>
class pair{
public:
    pair()=default;
    pair(T p1, S p2): p1(p1), p2(p2) {};
    ~pair()=default;
    bool operator<(const pair &toCompare) const;
    bool operator>(const pair &toCompare) const;
    bool operator<=(const pair &toCompare) const;
    bool operator==(const pair &toCompare) const;
    pair(const pair & toCopy): p1(toCopy.p1), p2(toCopy.p2) {};
    pair& operator=(const pair & toCopy);
    T getP1() const;
    S getP2() const;
    void setP1(T p1);
    void setP2(S p2);
private:
    T p1;
    S p2;
};

template<typename T, typename S>
void pair<T, S>::setP2(S p2) {
    this->p2 = p2;
}

template<typename T, typename S>
void pair<T, S>::setP1(T p1) {
    this->p1 = p1;
}

template<typename T, typename S>
bool pair<T, S>::operator<=(const pair &toCompare) const{
    return operator<(toCompare) || operator==(toCompare);
}

template<typename T, typename S>
S pair<T, S>::getP2() const{
    return p2;
}

template <typename T, typename S>
bool pair<T, S>::operator<(const pair<T, S> &toCompare) const{
    if (p1 < toCompare.p1) return true;
    return p1 == toCompare.p1 && p2 > toCompare.p2;
}

template <typename T, typename S>
bool pair<T, S>::operator>(const pair<T, S> &toCompare) const {
    return toCompare < *this;
}

template <typename T, typename S>
bool pair<T, S>::operator==(const pair<T, S> &toCompare) const{
    return p1 == toCompare.p1 && p2 == toCompare.p2;
}

template<typename T, typename S>
pair<T, S>& pair<T, S>::operator=(const pair<T, S> &toCopy) {
    p1 = toCopy.p1;
    p2 = toCopy.p2;
    return *this;
}

template <typename T, typename S>
T pair<T, S>::getP1() const {
    return p1;
}


#endif //DS_WET_1_PAIR_H
