//
// Created by Михаил Терентьев on 2019-01-12.
//

#ifndef TUPLE_TUPLE_H
#define TUPLE_TUPLE_H

#include <cstddef>

template<typename... Args>
struct tuple;

template<size_t Index, typename Ttuple>
struct Element;

template<typename T>
struct tuple<T> {
    explicit tuple(T x) : val(x) {};
public:
    T val;
};


template<typename T, typename... Args>
struct tuple<T, Args...> : public tuple<Args...> {

    explicit tuple(T x, Args ... args) : tuple<Args...>(args...), val(x) {}

public:
    T val;
};

///////////////////////////////////////
//SAVE TYPE
template<typename Head, typename... Tail>
struct Element<0, tuple<Head, Tail...>> {
    using ResType = Head;
    using ResTuple = tuple<Head, Tail...>;
};
//GO DEEPER
template<size_t index, typename Head, typename... Tail>
struct Element<index, tuple<Head, Tail...>> : public Element<index - 1, tuple<Tail...>> {
};
//////////////////////////////////////////

template<size_t index, typename... Args>
typename Element<index, tuple<Args...>>::ResType &get(tuple<Args...> &t) {
    using Truple_f = typename Element<index, tuple<Args...>>::ResTuple;
    return static_cast<Truple_f & > (t).val;
}

template<size_t index, typename... Args>
typename Element<index, tuple<Args...>>::ResType const &get(tuple<Args...> const &t) {
    using Truple_f = typename Element<index, tuple<Args...>>::ResTuple;
    return static_cast<Truple_f const &>(t).val;
}


template<typename ExpType, typename ...Args>
struct Frequency;

template<typename ExpType>
struct Frequency<ExpType> {
    constexpr static std::size_t freq = 0;
};

template<typename T, typename First, typename ...Args>
struct Frequency<T, First, Args...> {
    constexpr static std::size_t freq = std::is_same<T, First>::value + Frequency<T, Args...>::freq;
};

//bad
template<typename T, typename Head, typename ...Tail>
struct getter {
    static T const &getVal(tuple<Head, Tail...> const &t) {
        return getter<T, Tail...>::getVal(t);
    }

    static T &getVal(tuple<Head, Tail...> &t) {
        return getter<T, Tail...>::getVal(t);
    }
};
//good
template<typename T, typename ...Args>
struct getter<T, T, Args...> {
    static T const &getVal(tuple<T, Args...> const &t) {
        return t.val;
    }

    static T &getVal(tuple<T, Args...> &t) {
        return t.val;
    }
};

template<typename ExpectedType, typename ...Args>
ExpectedType const &get(tuple<Args...> const &t) {
    static_assert(Frequency<ExpectedType, Args...>::freq == 1, "Fail");
    return getter<ExpectedType, Args...>::getVal(t);
}

template<typename ExpectedType, typename ...Args>
ExpectedType &get(tuple<Args...> &t) {
    static_assert(Frequency<ExpectedType, Args...>::freq == 1, "Fail");
    return getter<ExpectedType, Args...>::getVal(t);
}


#endif //TUPLE_TUPLE_H
