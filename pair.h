

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
    T getP1();
    S getP2();
private:
    T p1;
    S p2;
};

template<typename T, typename S>
bool pair<T, S>::operator<=(const pair &toCompare) const{
    return operator<(toCompare) || operator==(toCompare);
}

template<typename T, typename S>
S pair<T, S>::getP2() {
    return p2;
}

template <typename T, typename S>
bool pair<T, S>::operator<(const pair<T, S> &toCompare) const{
    if (p1 < toCompare.p1) return true;
    return p1 == toCompare.p1 && p2 < toCompare.p2;
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
T pair<T, S>::getP1() {
    return p1;
}


#endif //DS_WET_1_PAIR_H
