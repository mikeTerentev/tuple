//
// Created by Михаил Терентьев on 2019-01-12.
//

#ifndef TUPLE_TUPLE_H
#define TUPLE_TUPLE_H

#include <cstddef>

template<typename... Args>
struct tuple;

template<>
struct tuple<> {
};

template<typename T>
struct tuple<T> {
    template<typename F, typename = typename std::enable_if<!std::is_same<typename std::remove_reference<F>::type, tuple<T> >::value>::type>
    explicit tuple(F&&  x) :  val(std::forward<F>(x)) {};
public:
    T val;
};

template<size_t Index, typename Ttuple>
struct Element;

template<typename T, typename... Args>
struct tuple<T, Args...> : public tuple<Args...> {
    tuple()= default;
    template<typename F, typename ...As>
    constexpr explicit  tuple(F &&f, As &&...args): tuple<Args...>(std::forward<As>(args)...), val(std::forward<F>(f)) {}

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
    static constexpr T const &getVal(tuple<Head, Tail...> const &t) {
        return getter<T, Tail...>::getVal(t);
    }

    static constexpr T &getVal(tuple<Head, Tail...> &t) {
        return getter<T, Tail...>::getVal(t);
    }
};
//good
template<typename T, typename ...Args>
struct getter<T, T, Args...> {
    static constexpr T const &getVal(tuple<T, Args...> const &t) {
        return t.val;
    }

    static constexpr T &getVal(tuple<T, Args...> &t) {
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
