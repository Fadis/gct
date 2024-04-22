#include <cstdlib>
#include <iostream>
#include <gct/kdtree.hpp>

int main() {

  if( gct::test_kdtree() ) {
    std::cout << "ok" << std::endl;
  }
  else {
    std::cout << "failed" << std::endl;
    std::abort();
  }

}

