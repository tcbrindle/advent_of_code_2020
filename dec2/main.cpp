
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../common.hpp"

namespace {

struct pw_entry {
    int lo;
    int hi;
    char c;
    std::string pw;
};

constexpr auto parse_pw_list = [](char const* path)
{
    std::ifstream infile(path);

    return aoc::lines(infile)
        .map([](auto const& line) {
            pw_entry entry;
            char buf[32];

            std::sscanf(line.c_str(), "%d-%d %c: %32s",
                    &entry.lo, &entry.hi, &entry.c, buf);
            entry.pw = buf;
            return entry;
        })
        .to_vector();
};

}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Error, no input";
        return 0;
    }

    const auto entries = parse_pw_list(argv[1]);

    const auto part1 = flow::count_if(entries, [](const auto& e) {
        const auto cnt = flow::count(e.pw, e.c);
        return cnt >= e.lo && cnt <= e.hi;
    });

    fmt::print("Part 1: found {} valid passwords\n", part1);

    const auto part2 = flow::count_if(entries, [](const auto& e) {
        return (e.pw[e.lo - 1] == e.c) ^ (e.pw[e.hi - 1] == e.c);
    });

    fmt::print("Part 2: found {} valid passwords\n", part2);
}