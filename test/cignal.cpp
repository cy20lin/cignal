#include <cignal.hpp>
#include <iostream>

using cignal::n_vector;

int main() {
    n_vector<int,2> image({720,1080});
    std::cout << image.size() << std::endl;

    return 0;
}

