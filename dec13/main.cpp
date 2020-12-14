
#include "../common.hpp"

const auto parse_input = [](auto&& istream)
{
    int target;
    istream >> target;

    auto ids = aoc::lines(istream, ',').map(aoc::try_parse<int64_t>).to_vector();

    return std::pair{target, std::move(ids)};
};

const auto part1 = [](auto const& pair)
{
    auto const& [target, ids] = pair;

    return flow::from(ids)
            .deref()
            .map([t = target](int id) { return std::pair(id, id - t % id); })
            .min([](auto p1, auto p2) { return p1.second < p2.second; })
            .map([](auto p) { return p.first * p.second; })
            .value();
};

// Nicked wholesale from Wikipedia. I'm not proud.
const auto mod_inverse = [](int64_t a, int64_t n)  -> int64_t {

    int64_t t = 0, r = n, newt = 1, newr = a;

    while (newr != 0) {
        int64_t q = r / newr;

        int64_t temp = t;
        t = newt;
        newt = temp - (q * newt);

        temp = r;
        r = newr;
        newr = temp - (q * newr);
    }

    assert(r == 1);
    if (t < 0) {
        t = t + n;
    }

    return t;
};

const auto part2 = [](auto const& input) -> int64_t
{
    // Do a first pass through the input to find the product of all
    // the (valid) items
    const int64_t product = flow::deref(input).product();

    // Calculate the result using the Chinese Remainder Theorem
    auto result = flow::enumerate(input)
                    .filter([](auto p) { return (bool) p.second; })
                    .map([&] (auto p) {
                        const auto [idx, n] = p;
                        const auto y = product/(*n);
                        return -idx * y * mod_inverse(y, *n);
                    })
                    .sum();

    // Make sure the result is the smallest positive integer satisfying the condition
    result %= product;
    return result < 0 ? result + product : result;
};

constexpr auto test_data =
R"(939
7,13,x,x,59,x,31,19)";

int main(int argc, char** argv)
{
    {
        const auto test_input = parse_input(std::istringstream(test_data));
        assert(part1(test_input) == 295);
        assert(part2(test_input.second) == 1068781);

        // Additional tests for part2
        // (Are there any systems on which int64_t is not long long?)
        constexpr flow::optional<long long> ex1[] = {17LL, {}, 13LL, 19LL};
        static_assert(part2(ex1) == 3417);

        constexpr flow::optional<long long> ex2[] = {67LL, 7LL, 59LL, 61LL};
        static_assert(part2(ex2) == 754018);

        constexpr flow::optional<long long> ex3[] = {67LL, {}, 7LL, 59LL, 61LL};
        static_assert(part2(ex3) == 779210);

        constexpr flow::optional<long long> ex4[] = {67LL, 7LL, {}, 59LL, 61LL};
        static_assert(part2(ex4) == 1261476);

        constexpr flow::optional<long long> ex5[] = {1789LL, 37LL, 47LL, 1889LL};
        static_assert(part2(ex5) == 1202161486);
    }

    if (argc < 2) {
        fmt::print(stderr, "Error, no input");
        return -1;
    }

    const auto input = parse_input(std::ifstream(argv[1]));

    fmt::print("Part 1: {}\n", part1(input));
    fmt::print("Part 2: {}\n", part2(input.second));
}