
#include "../common.hpp"

struct offset {
    int x, y;
};

struct coord {
    int x, y;

    constexpr coord& operator+=(offset o) { x += o.x; y += o.y; return *this; }
    friend constexpr coord operator+(coord lhs, offset rhs) { return lhs += rhs; }
};

const auto count_occupied = [](auto const& input, const coord pos)
{
    const int x_sz = input.size();
    const int y_sz = input[0].size();

    const auto in_bounds = [&](coord c) {
        return c.x >= 0 && c.x < x_sz &&
               c.y >= 0 && c.y < y_sz;
    };

    constexpr auto neighbours = std::array<offset, 8>{{
        {-1, -1}, {-1, 0}, {-1, +1},
        { 0, -1},          { 0, +1},
        {+1, -1}, {+1, 0}, {+1, +1}
    }};

    return flow::map(neighbours, [&](offset o) { return pos + o; })
                .filter(in_bounds)
                .map([&](coord c) { return input[c.x][c.y]; })
                .count('#');
};

const auto part1 = [](auto input)
{
    auto next = input;

    do {
        input.swap(next);

        for (int i = 0; i < input.size(); i++) {
            for (int j = 0; j < input[i].size(); j++) {
                const auto n = count_occupied(input, {i, j});
                switch (input[i][j]) {
                case 'L': next[i][j] = (n == 0 ? '#' : 'L'); break;
                case '#': next[i][j] = (n >= 4 ? 'L' : '#'); break;
                default: break;
                }
            }
        }
    } while(input != next);

    return flow::flatten(input).count('#');
};

const auto count_occupied2 = [](const auto& input, const coord pos)
{
    const int x_sz = input.size();
    const int y_sz = input[0].size();

    const auto in_bounds = [&](coord c) {
        return c.x >= 0 && c.x < x_sz &&
               c.y >= 0 && c.y < y_sz;
    };

    constexpr auto directions = std::array<offset, 8>{{
        {-1, -1}, {-1, 0}, {-1, +1},
        { 0, -1},          { 0, +1},
        {+1, -1}, {+1, 0}, {+1, +1}
    }};

    return flow::map(directions, [&](offset o) {
        return flow::generate([p = pos, o] () mutable { return p += o; })
            .take_while(in_bounds)
            .map([&](coord c) { return input[c.x][c.y]; })
            .drop_while(flow::pred::eq('.'))
            .take(1)
            .count('#');
    }).sum();
};

const auto part2 = [](auto input)
{
    auto next = input;

    do {
        input.swap(next);

        for (int i = 0; i < input.size(); i++) {
            for (int j = 0; j < input[i].size(); j++) {
                const auto n = count_occupied2(input, {i, j});
                switch (input[i][j]) {
                case 'L': next[i][j] = (n == 0 ? '#' : 'L'); break;
                case '#': next[i][j] = (n >= 5 ? 'L' : '#'); break;
                default: break;
                }
            }
        }

    } while (next != input);

    return flow::flatten(input).count('#');
};

constexpr auto test_data =
R"(L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL)";

int main(int argc, char** argv)
{
    std::istringstream iss(test_data);
    const auto test_vec = aoc::lines(iss).to_vector();

    assert(part1(test_vec) == 37);
    assert(part2(test_vec) == 26);

    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    std::ifstream file(argv[1]);
    const auto data = aoc::lines(file).to_vector();

    fmt::print("Part 1: {}\n", part1(data));

    fmt::print("Part 2: {}\n", part2(data));
}