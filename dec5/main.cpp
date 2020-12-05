
#include "../common.hpp"

constexpr auto get_seat_id = [](std::string_view str) -> int
{
    return flow::fold(str, [](unsigned val,  char c) {
        return (val << 1) + (c == 'B' || c == 'R');
    }, 0u);
};

// To calculate part 2 we need the min, the max and the sum.
// We could easily do this in two passes with minmax() and sum(),
// but instead lets collect them all in one go using another fold.
constexpr auto get_min_max_sum = [](auto&& input)
{
    struct stats {
        int min = INT_MAX, max, sum;
    };

    return flow::fold(FLOW_FWD(input), [](stats s, int item) {
        return stats {
            .min = std::min(item, s.min),
            .max = std::max(item, s.max),
            .sum = s.sum + item
        };
    }, stats{});
};

constexpr std::pair<std::string_view, int> test_data[] = {
    { "BFFFBBFRRR", 567 },
    { "FFFBBBFRRR", 119 },
    { "BBFFBBFRLL", 820 }
};

static_assert(flow::all(test_data, [](auto p) {
    return get_seat_id(p.first) == p.second;
}));

int main(int argc, char** argv)
{
    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    std::ifstream input(argv[1]);

    const auto [min, max, sum] =
        aoc::lines(input).map(get_seat_id).apply(get_min_max_sum);

    fmt::print("Part 1: max seat id is {}\n", max);

    const auto missing =  (max * (max + 1) - (min - 1) * min)/2 - sum;
    fmt::print("Part 2: missing seat id is {}\n", missing);
}