
#include "../common.hpp"

int main(int argc, char** argv)
{
    // Read the input into a vector
    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    const auto input = aoc::vector_from_file<int>(argv[1]);

    // Just brute force things
    const auto part1 = [&] {
        for (int i : input) {
            for (int j : input) {
                if (i + j == 2020) {
                    return i * j;
                }
            }
        }
        return -1;
    }();

    fmt::print("Part one: product is {}\n", part1);

    const auto part2 = [&] {
        for (int i : input) {
            for (int j : input) {
                for (int k : input) {
                    if (i + j + k == 2020) {
                        return i * j * k;
                    }
                }
            }
        }
        return -1;
    }();

    fmt::print("Part two: product is {}\n", part2);
}
