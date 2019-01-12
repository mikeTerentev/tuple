#include <iostream>
#include "tuple.h"
#include "assert.h"
struct A{
    int b;
    A()=default;
    A(const A& a) : b(a.b){}
    A(A &&a) = delete;
};

int main(){
    tuple<std::string,int,bool> t("abc",5,true);
    assert(get<0>(t) == "abc");
    assert(get<1>(t) == 5);
    assert(get<2>(t) == true);
    assert(get<int>(t)== 5);
    const tuple<std::string,int,bool> tc("abc",5,true);
    const std::string res = "abc";
    assert(get<0>(tc) == res);
    assert(get<std::string>(tc) == res);
   // assert(get<long>(t)== 5);
    std::unique_ptr<int> up(new int(10));
   tuple<std::unique_ptr<int>> tp(std::move(up));
   std::cerr<<*get<0>(tp);
   A a;
   tuple<A> tr(a);
    return 0;
}
