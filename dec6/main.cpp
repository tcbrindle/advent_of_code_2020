
#include "../common.hpp"
#include <set>

constexpr auto str_to_set = [](auto&& str)
{
    return std::set(str.begin(), str.end());
};

constexpr auto set_intersect =
[]<typename T>(std::set<T> const& set1, std::set<T> const& set2)
{
    std::set<T> out;
    std::set_intersection(set1.begin(), set1.end(),
                          set2.begin(), set2.end(),
                          std::inserter(out, out.begin()));
    return out;
};

constexpr auto part1 = [](auto&& input) {
    return flow::split(FLOW_FWD(input), "")
        .map([](auto lines) {
            return std::move(lines)
                .flatten()
                .template to<std::set>()
                .size(); })
        .sum();
};

constexpr auto part2 = [](auto&& input) {
    return flow::split(FLOW_FWD(input), "")
        .map([](auto lines) {
            return std::move(lines)
                .map(str_to_set)
                .fold(set_intersect,
                      lines.next().map(str_to_set).value())
                .size(); })
        .sum();
};


constexpr auto test_data =
R"(abc

a
b
c

ab
ac

a
a
a
a

b)";

int main(int argc, char** argv)
{
    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    const auto input = [&]{
        std::ifstream is(argv[1]);
        return aoc::lines(is).to_vector();
    }();

    fmt::print("Part 1: sum of counts was {}\n", part1(input));

    fmt::print("Part 1: sum of counts was {}\n", part2(input));

}