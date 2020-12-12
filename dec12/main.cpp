
#include "../common.hpp"

#include <complex>

using icomplex = std::complex<int>;

constexpr auto rotate_table = std::array<icomplex, 4>{{
    {1, 0}, {0, 1}, {-1, 0}, {0, -1}
}};

struct ferry {
    icomplex pos;
    icomplex heading;
};

const auto part1 = [](auto const& input) -> int
{
    const ferry f = flow::fold(input, [](ferry f, const auto& pair) {
        const auto [c, arg] = pair;

        switch (c) {
        case 'N': f.pos += icomplex{0, arg}; break;
        case 'S': f.pos -= icomplex{0, arg}; break;
        case 'E': f.pos += icomplex{arg, 0}; break;
        case 'W': f.pos -= icomplex{arg, 0}; break;
        case 'L': f.heading *= rotate_table.at(arg/90); break;
        case 'R': f.heading *= rotate_table.at(4 - arg/90); break;
        case 'F': f.pos += arg * f.heading; break;
        default:
            throw std::runtime_error(fmt::format("Unknown instruction {} {}", c, arg));
        }

        return f;
    }, ferry{.heading= {1, 0}});

    return std::abs(f.pos.real()) + std::abs(f.pos.imag());
};

const auto part2 = [](auto const& input) -> int
{
    const ferry f = flow::fold(input, [](ferry f, const auto& pair) {
        const auto [c, arg] = pair;

        switch (c) {
        case 'N': f.heading += icomplex{0, arg}; break;
        case 'S': f.heading -= icomplex{0, arg}; break;
        case 'E': f.heading += icomplex{arg, 0}; break;
        case 'W': f.heading -= icomplex{arg, 0}; break;
        case 'L': f.heading *= rotate_table.at(arg/90); break;
        case 'R': f.heading *= rotate_table.at(4 - arg/90); break;
        case 'F': f.pos += arg * f.heading; break;
        default:
            throw std::runtime_error(fmt::format("Unknown instruction {} {}", c, arg));
        }

        return f;
    }, ferry{.heading = {10, 1}});

    return std::abs(f.pos.real()) + std::abs(f.pos.imag());
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