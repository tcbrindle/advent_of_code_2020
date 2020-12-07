
#include "../common.hpp"

constexpr auto sort = [](auto cont)
{
    std::sort(cont.begin(), cont.end());
    return cont;
};

constexpr auto set_union = [](auto set1, auto set2)
{
    decltype(set1) out;
    std::set_union(set1.begin(), set1.end(),
                   set2.begin(), set2.end(),
                   std::inserter(out, out.end()));
    return out;
};

constexpr auto set_intersect = [](auto set1, auto set2)
{
    decltype(set1) out;
    std::set_intersection(set1.begin(), set1.end(),
                          set2.begin(), set2.end(),
                          std::inserter(out, out.end()));
    return out;
};

constexpr auto part1 = [](auto&& input) {
    return flow::split(FLOW_FWD(input), "")
        .map([](auto lines) {
            return std::move(lines)
                .map(sort)
                .fold_first(set_union)
                .value()
                .size(); })
        .sum();
};

constexpr auto part2 = [](auto&& input) {
    return flow::split(FLOW_FWD(input), "")
        .map([](auto lines) {
            return std::move(lines)
                .map(sort)
                .fold_first(set_intersect)
                .value()
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

    fmt::print("Part 2: sum of counts was {}\n", part2(input));
}