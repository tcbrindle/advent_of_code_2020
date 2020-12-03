
#ifndef ADVENT_OF_CODE_2020_COMMON_HPP
#define ADVENT_OF_CODE_2020_COMMON_HPP

#include <array>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "extern/flow.hpp"

#define FMT_HEADER_ONLY
#include "extern/fmt/format.h"

namespace aoc {

template <typename T>
std::vector<T> vector_from_file(const char* path)
{
    std::ifstream str(path);
    return flow::from_istream<T>(str).to_vector();
}

} // namespace aoc

#endif
