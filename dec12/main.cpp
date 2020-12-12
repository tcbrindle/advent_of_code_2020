
#include "../common.hpp"

struct vec2 {
    int x, y;

    constexpr vec2& operator+=(vec2 o) { x += o.x; y += o.y; return *this; }

    friend constexpr vec2 operator+(vec2 lhs, vec2 rhs) { return lhs += rhs; }

    friend constexpr vec2 operator*(int k, vec2 v) { return {k * v.x, k * v.y}; }
};

const auto rotatel = [](vec2 v, int deg) -> vec2
{
    // This might be taking "No Raw Loops" a little too far...
    return flow::ints(0, deg, 90).fold([](vec2 v, int /*unused*/) {
        return vec2{-v.y, v.x};
    }, v);
};

const auto rotater = [](vec2 v, int deg)
{
    return flow::ints(0, deg, 90).fold([](vec2 v, int /*unused*/) {
       return vec2{v.y, -v.x};
    }, v);
};

struct ferry {
    vec2 pos;
    vec2 heading;
};

const auto process_instruction_pt1 = [](ferry f, const auto& pair) -> ferry
{
    const auto [c, arg] = pair;

    switch (c) {
    case 'N':
        f.pos.y += arg;
        break;
    case 'S':
        f.pos.y -= arg;
        break;
    case 'E':
        f.pos.x += arg;
        break;
    case 'W':
        f.pos.x -= arg;
        break;
    case 'L':
        f.heading = rotatel(f.heading, arg);
        break;
    case 'R':
        f.heading = rotater(f.heading, arg);
        break;
    case 'F':
        f.pos += arg * f.heading;
        break;
    default:
        throw std::runtime_error(fmt::format("Unknown instruction {} {}", c, arg));
    }

    return f;
};

const auto part1 = [](auto const& input) -> int
{
    ferry const f = flow::fold(input, process_instruction_pt1,
                               ferry{.pos = {}, .heading= {1, 0}});
    return std::abs(f.pos.x) + std::abs(f.pos.y);
};

const auto process_instruction_pt2 = [](ferry f, const auto& pair) -> ferry
{
    const auto [c, arg] = pair;

    switch (c) {
    case 'N':
        f.heading.y += arg;
        break;
    case 'S':
        f.heading.y -= arg;
        break;
    case 'E':
        f.heading.x += arg;
        break;
    case 'W':
        f.heading.x -= arg;
        break;
    case 'L':
        f.heading = rotatel(f.heading, arg);
        break;
    case 'R':
        f.heading = rotater(f.heading, arg);
        break;
    case 'F':
        f.pos += arg * f.heading;
        break;
    default:
        throw std::runtime_error(fmt::format("Unknown instruction {} {}", c, arg));
    }
    return f;
};

const auto part2 = [](auto const& input) -> int
{
    const ferry f = flow::fold(input, process_instruction_pt2,
                               ferry{.pos = {}, .heading = {10, 1}});

    return std::abs(f.pos.x) + std::abs(f.pos.y);
};

const auto parse_input = [](auto&& istream)
{
    return aoc::lines(istream)
            .map([](const auto& line) {
                char c = line.at(0);
                int arg = std::atoi(line.c_str() + 1);
                return std::pair{c, arg};
            })
            .to_vector();
};

constexpr auto test_data =
R"(F10
N3
F7
R90
F11)";

int main(int argc, char** argv)
{
    {
        const auto test_vec = parse_input(std::istringstream(test_data));
        assert(part1(test_vec) == 25);
        assert(part2(test_vec) == 286);
    }

    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    const auto vec = parse_input(std::ifstream(argv[1]));

    fmt::print("Part 1: {}\n", part1(vec));
    fmt::print("Part 2: {}\n", part2(vec));
}