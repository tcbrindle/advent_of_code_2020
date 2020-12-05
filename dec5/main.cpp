
#include "../common.hpp"

struct bounds { int lo, hi; };

constexpr auto get_row = [](std::string_view str) -> int
{
    return flow::take(str, 7).fold([](bounds b,  char c) {
        switch (c) {
            case 'F': b.hi = (b.lo + b.hi)/2; break;
            case 'B': b.lo = 1 + (b.lo + b.hi)/2; break;
        }
        return b;
    }, bounds{0, 127}).lo;
};

constexpr auto get_column = [](std::string_view str) -> int
{
    return flow::drop(str, 7).fold([](bounds b,  char c) {
        switch (c) {
            case 'L': b.hi = (b.lo + b.hi)/2; break;
            case 'R': b.lo = 1 + (b.lo + b.hi)/2; break;
        }
        return b;
    }, bounds{0, 7}).lo;
};

constexpr auto get_seat_id = [](std::string_view str) -> int
{
    return 8 * get_row(str) + get_column(str);
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

    const auto vec = aoc::vector_from_file<std::string>(argv[1]);

    // To calculate part 2 we need the min, the max and the sum.
    // We could easily do this in two passes with minmax() and sum(),
    // but instead lets collect them all in one go using another fold.
    struct stats { int min = INT_MAX, max, sum; };

    const auto [min, max, sum] = flow::map(vec, get_seat_id).fold(
        [](stats s, int item) {
            return stats {
                .min = std::min(item, s.min),
                .max = std::max(item, s.max),
                .sum = s.sum + item
            };
        }, stats{});

    fmt::print("Part 1: max seat id is {}\n", max);

    const auto missing =  (max * (max + 1) - (min - 1) * min)/2 - sum;
    fmt::print("Part 2: missing seat id is {}\n", missing);
}