#include <iostream>
#include <x86intrin.h>
#include <benchmark/benchmark.h>

// we implement an identity monad, does function application

template<typename C>
struct Monad {
  using contained = C;
};

// Monad<A> -> (A -> B) -> Monad<B>
template<typename MA, template <typename> typename F>
struct mmap {
  using type = Monad<typename F<typename MA::contained>::type>;
};

template<typename A, template <typename> typename F>
using mmap_t = typename mmap<A,F>::type;


// Monad<Monad<A>> -> Monad<A>
template<typename Arg1>
using join_t = typename Arg1::contained;

// (A -> Monad<B>) -> (B -> Monad<C>) -> ... -> (A -> Monad<Z>)
template<template <typename> typename F1, 
  template <typename> typename F2, 
  template <typename> typename... Fs>
struct compose {
  template<typename A>
  struct ttype {
    using type = join_t<mmap_t<typename F1<A>::type, compose<F2,Fs...>::template ttype > >;
  };
};

template<template <typename> typename F1, 
  template <typename> typename F2>
struct compose<F1,F2> {
  template<typename A>
  struct ttype {
    using type = join_t<mmap_t<typename F1<A>::type,F2>>;
  };
};

template<int Val>
struct V {
  static const int value = Val;
};

using V1 = V<1>;

template<typename Inp>
struct Plus5M {
  using type = Monad<V<Inp::value + 5>>;
};
template<typename Inp>
struct Plus5 {
  using type = V<Inp::value + 5>;
};

auto result = compose<Plus5M,Plus5M,Plus5M>::template ttype<V1>::type::contained::value;

int main() {
  std::cout << result << "\n";
  return 0;
}
