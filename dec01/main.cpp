
#include "../common.hpp"

namespace {

constexpr auto tuple_sum = [](auto&& tuple) {
    return std::apply([](auto&&... args) { return (FLOW_FWD(args) + ...); }, FLOW_FWD(tuple));
};

constexpr auto tuple_product = [](auto&& tuple) {
    return std::apply([](auto&&... args) { return (FLOW_FWD(args) * ...); }, FLOW_FWD(tuple));
};

constexpr auto find_value = [](auto&&... args) {
    return flow::cartesian_product(FLOW_FWD(args)...)
            .filter([](auto const& t) { return tuple_sum(t) == 2020; })
            .map(tuple_product)
            .next().value();
};

constexpr auto test_data = std::array{1721, 979, 366, 299, 675, 1456};

static_assert(find_value(test_data, test_data)== 514579);

static_assert(find_value(test_data, test_data, test_data) == 241861950);

}

int main(int argc, char** argv)
{
    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    const auto input = aoc::vector_from_file<int>(argv[1]);

    fmt::print("Part one: product is {}\n", find_value(input, input));

    fmt::print("Part two: product is {}\n", find_value(input, input, input));
}
