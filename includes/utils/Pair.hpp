#ifndef PAIR
#define PAIR

template <typename T, typename U> struct Pair
{
    T first;
    U second;
    Pair(T first, U second) : first(first), second(second) {};
};

#endif // PAIR
