
#include "../common.hpp"

namespace {

constexpr auto count_trees = [](auto&& forest, const int xinc, const int yinc) {
    return flow::stride(forest, yinc)
        .zip_with([xinc](auto&& row, auto idx) {
            return flow::cycle(FLOW_FWD(row)).advance(1 + xinc * idx).value();
        }, flow::ints())
        .count('#');
};

constexpr auto part1 = [](auto&& forest) { return count_trees(forest, 3, 1); };

constexpr auto part2 = [](auto&& forest) {
    return count_trees(forest, 1, 1)
        * count_trees(forest, 3, 1)
        * count_trees(forest, 5, 1)
        * count_trees(forest, 7, 1)
        * count_trees(forest, 1, 2);
};

constexpr auto test_data =
R"(..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#)";

static_assert(part1(flow::c_str(test_data).split('\n')) == 7);
static_assert(part2(flow::c_str(test_data).split('\n')) == 336);

}

int main(int argc, char** argv) {

    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    const auto forest = aoc::vector_from_file<std::string>(argv[1]);

    fmt::print("Part one: counted {} trees\n", part1(forest));

    fmt::print("Part two: counted {} trees\n", part2(forest));
}