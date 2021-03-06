// RUN: %cxxamp %s -o %t.out && %t.out
#include <hc.hpp>
#include <hc_math.hpp>

#include <iostream>
#include <random>
#include <cmath>
#include <cassert>

using namespace hc;

#define ERROR_THRESHOLD (1e-4)

template<typename _Tp>
bool test() {
  const int vecSize = 1024;

  // Alloc & init input data
  extent<1> e(vecSize);
  array<_Tp, 1> a(vecSize);

  array<_Tp, 1> b(vecSize);
  array<_Tp, 1> c(vecSize);
  array<_Tp, 1> d(vecSize);

  // setup RNG
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_real_distribution<_Tp> dis(1, 10);
  array_view<_Tp> ga(a);
  array_view<_Tp> gb(b);
  array_view<_Tp> gc(c);
  array_view<_Tp> gd(d);

  for (index<1> i(0); i[0] < vecSize; i++) {
    ga[i] = dis(gen);
    gb[i] = dis(gen);
  }

  parallel_for_each(
    e,
    [=](index<1> idx) [[hc]] {
    gc[idx] = precise_math::pow(ga[idx], gb[idx]);
  });

  for(unsigned i = 0; i < vecSize; i++) {
    gd[i] = precise_math::pow(ga[i], gb[i]);
  }

  _Tp sum = 0;
  for(unsigned i = 0; i < vecSize; i++) {
    if (std::isnan(gc[i])) {
      printf("gc[%d] is NaN!\n", i);
      assert(false);
    }
    _Tp diff = precise_math::fabs(gc[i] - gd[i]);
    sum += diff;
  }
  return (sum < ERROR_THRESHOLD);
}

int main(void) {
  bool ret = true;

  //ret &= test<float>();
  ret &= test<double>();

  return !(ret == true);
}

