
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
    CharT delim_;

public:
    explicit lines_flow(istream_type& is, CharT delim)
        : is_(std::addressof(is)),
          delim_(delim)
    {}

    auto next() -> flow::maybe<string_type const&>
    {
        if (std::getline(*is_, buf_, delim_)) {
            return {std::as_const(buf_)};
        }
        return {};
    }
};

}

template <typename CharT, typename Traits>
detail::lines_flow<CharT, Traits>
lines(std::basic_istream<CharT, Traits>& is, CharT delim = CharT{'\n'})
{
    return detail::lines_flow<CharT, Traits>(is, delim);
}

// This function is not great, but nor are the alternatives:
//  * std::from_chars - not constexpr, requires contiguous input
//  * std::atoi - same
//  * std::stoi - needs a std::string, throws on failure
//  * istreams - urgh
// So, we're gonna roll our own
// No, I'm not going to try and do this for floating point...
template <typename I>
const auto try_parse = [](auto&& f) -> flow::optional<I> {

    static_assert(std::is_integral_v<I>);
    static_assert(std::is_same_v<flow::flow_value_t<decltype(f)>, char>);

    constexpr auto is_space = flow::pred::in(' ', '\f', '\n', '\r', '\t', '\v');
    constexpr auto is_digit = flow::pred::geq('0') && flow::pred::leq('9');

    auto f2 = flow::drop_while(FLOW_FWD(f), is_space);

    I mult = 1;
    I first = 0;

    // Deal with the first character
    {
        auto m = f2.next();
        if (!m) {
            return {};
        }

        char c = *m;

        if (c == '-') {
            mult = -1;
        } else if (c == '+') {
            // pass
        } else if (is_digit(c)) {
            first = c - '0';
        } else {
            return {};
        }
    }

    // Deal with the rest
    return std::move(f2)
             .take_while(is_digit)
             .fold([](auto acc, char c) -> flow::optional<I> {
                    return 10 * acc.value_or(0) + (c - '0');
             }, flow::optional<I>{first})
             .map([mult](I i) { return i * mult; });
};

} // namespace aoc

#endif
