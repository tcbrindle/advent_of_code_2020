
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

namespace detail {

template <typename CharT, typename Traits>
struct lines_flow: flow::flow_base<lines_flow<CharT, Traits>> {
private:
    using istream_type = std::basic_istream<CharT, Traits>;
    using string_type = std::basic_string<CharT, Traits>;
    istream_type* is_;
    string_type buf_;

public:
    explicit lines_flow(istream_type& is)
        : is_(std::addressof(is))
    {}

    auto next() -> flow::maybe<string_type const&>
    {
        if (std::getline(*is_, buf_)) {
            return {std::as_const(buf_)};
        }
        return {};
    }
};

}

template <typename CharT, typename Traits>
detail::lines_flow<CharT, Traits>
lines(std::basic_istream<CharT, Traits>& is)
{
    return detail::lines_flow<CharT, Traits>(is);
}

} // namespace aoc

#endif
