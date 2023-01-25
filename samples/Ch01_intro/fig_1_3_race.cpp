// Copyright (C) 2020 Intel Corporation

// SPDX-License-Identifier: MIT

#include <sycl/sycl.hpp>
#include <iostream>
using namespace sycl;

const std::string secret {
  "Ifmmp-!xpsme\"\012J(n!tpssz-!Ebwf/!"
  "J(n!bgsbje!J!dbo(u!ep!uibu/!.!IBM\01" };

const auto sz = secret.size();

int main() {
  queue Q;

  char *result = malloc_shared<char>(sz, Q);

  // Introduce potential data race!  We don't define a dependence
  // to ensure correct ordering with later operations.
  Q.memcpy(result,secret.data(),sz);

  Q.parallel_for(sz,[=](auto& i) {
      result[i] -= 1;
      }).wait();

  std::cout << result << "\n";
  free(result, Q);
  return 0;
}


