// RUN: %clangxx -fsycl -fsycl-targets=%sycl_triple %s -o %t.out
// RUN: %HOST_RUN_PLACEHOLDER %t.out
// RUN: %CPU_RUN_PLACEHOLDER %t.out
// RUN: %GPU_RUN_PLACEHOLDER %t.out
// RUN: %ACC_RUN_PLACEHOLDER %t.out

#include "load.h"
#include <iostream>
using namespace sycl;

int main() {
  queue q;

  if (!q.get_device().has(aspect::atomic64)) {
    std::cout << "Skipping test\n";
    return 0;
  }

  constexpr int N = 32;
  load_test<double>(q, N);

  // Include long tests if they are 64 bits wide
  if constexpr (sizeof(long) == 8) {
    load_test<long>(q, N);
    load_test<unsigned long>(q, N);
  }

  // Include long long tests if they are 64 bits wide
  if constexpr (sizeof(long long) == 8) {
    load_test<long long>(q, N);
    load_test<unsigned long long>(q, N);
  }

  // Include pointer tests if they are 64 bits wide
  if constexpr (sizeof(char *) == 8) {
    load_test<char *>(q, N);
  }

  std::cout << "Test passed." << std::endl;
}