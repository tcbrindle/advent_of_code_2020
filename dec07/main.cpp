
#include "../common.hpp"
#include "../extern/ctre.hpp"

struct content {
    int num;
    std::string style;
};

using luggage = std::map<std::string, std::vector<content>>;

constexpr auto parse_content = [](auto f) -> content
{
    const auto [match, num, style] = ctre::match<" ?(\\d+) (.*) .*$">(std::move(f).to_string());
    assert((bool) match);
    return {std::stoi(num.to_string()), style.to_string()};
};

constexpr auto parse_rule = [](const auto& str)
    -> std::pair<std::string, std::vector<content>>
{
    auto const [match, style, contents] = ctre::match<"^(.*) bags contain (.*).$">(str);
    assert((bool) match);

    if (contents.to_view() == "no other bags") {
        return {style.to_string(), {}};
    }

    auto vec = flow::split(contents.to_view(), ',')
        .map(parse_content)
        .to_vector();

    return {style.to_string(), std::move(vec)};
};

constexpr auto parse_input = [](auto& istream)
{
    return aoc::lines(istream)
        .map(parse_rule)
        .template to<luggage>();
};

auto find_shiny_gold(const std::string& style, const luggage& cases) -> bool
{
    if (!cases.contains(style)) {
        return false;
    }

    auto const& vec = cases.at(style);

    if (flow::map(vec, &content::style).any(flow::pred::eq("shiny gold"))) {
        return true;
    }

    return flow::map(vec, &content::style).any([&](const auto& s) {
        return find_shiny_gold(s, cases);
    });
}

constexpr auto part1 = [](luggage const& cases)
{
    return flow::keys(cases)
            .count_if([&cases](const auto& s) { return find_shiny_gold(s, cases); });
};

auto sum_contents(const std::string& style, const luggage& cases) -> int
{
    if (!cases.contains(style)) {
        return 0;
    }

    return flow::from(cases.at(style))
                .map([&cases](const content& c) { return  c.num * (sum_contents(c.style, cases) + 1); })
                .sum();
}

constexpr auto part2 = [](luggage const& cases)
{
    return sum_contents("shiny gold", cases);
};

constexpr auto test_data =
R"(light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.)";

constexpr auto test_data2 =
R"(shiny gold bags contain 2 dark red bags.
dark red bags contain 2 dark orange bags.
dark orange bags contain 2 dark yellow bags.
dark yellow bags contain 2 dark green bags.
dark green bags contain 2 dark blue bags.
dark blue bags contain 2 dark violet bags.
dark violet bags contain no other bags.)";

int main(int argc, char** argv)
{
    if (argc < 2) {
        fmt::print(stderr, "No input\n");
        return -1;
    };

    {
        std::istringstream iss1(test_data);
        auto const cases1 = parse_input(iss1);

        assert(part1(cases1) == 4);
        assert(part2(cases1) == 32);

        std::istringstream iss2(test_data2);
        auto const cases2 = parse_input(iss2);

        assert(part2(cases2) == 126);
    }

    std::ifstream file(argv[1]);
    const auto cases = parse_input(file);

    fmt::print("Part 1: {}\n", part1(cases));

    fmt::print("Part 2: {}\n", part2(cases));
}
