// RUN: %clangxx -fsycl -fsycl-targets=%sycl_triple %s -o %t.out
// RUN: sycl-trace --sycl %CPU_RUN_PLACEHOLDER %t.out %CPU_CHECK_PLACEHOLDER

// Test tracing of the code location data for queue.submit in case of failure
// (exception generation)
//
// CHECK: code_location_queue_submit.cpp:main:ln16:col5

#include <sycl/sycl.hpp>

int main() {
  sycl::queue Q;
  bool ExceptionCaught = false;
  unsigned char *HostAllocSrc = (unsigned char *)sycl::malloc_host(1, Q);
  unsigned char *HostAllocDst = NULL;
  try {
    Q.copy(HostAllocDst, HostAllocSrc, 1);
  } catch (sycl::exception &e) {
    ExceptionCaught = true;
  }
  Q.wait();
  sycl::free(HostAllocSrc, Q);

  return !ExceptionCaught;
}
