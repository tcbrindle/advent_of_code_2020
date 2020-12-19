
#include "../common.hpp"

#include <variant>

// Variant visitation helper
template <typename... Ls>
struct visitor : Ls... {
    using Ls::operator()...;
};

template <typename... Ls>
visitor(Ls...) -> visitor<Ls...>;



enum class rules {
    part1,
    part2
};

struct value { int64_t val; };
struct add_op {};
struct mult_op {};

using term = std::variant<value, add_op, mult_op>;

template <rules>
constexpr auto evaluate_recursive(std::string_view& expr) -> int64_t;

template <rules R>
constexpr auto next_term(std::string_view& expr) -> term
{
    while (!expr.empty()) {
        const char c = expr.front();
        expr.remove_prefix(1);

        if (c == ' ') {
            continue;
        }

        if (c >= '0' && c <= '9') {
            return value{c - '0'};
        } else if (c == '+') {
            return add_op{};
        } else if (c == '*') {
            return mult_op{};
        } else if (c == '(') {
            auto val = evaluate_recursive<R>(expr);
            expr.remove_prefix(1); // eat the ')'
            return value{val};
        } else {
            throw std::runtime_error(fmt::format("Unexpected character {} in next_term", c));
        }
    }

    __builtin_unreachable();
};

template <rules R>
constexpr auto evaluate_recursive(std::string_view& expr) -> int64_t
{
    int64_t last_val = -1;

    while (!expr.empty() && expr[0] != ')')
    {
        last_val = std::visit(visitor{
            [&](value v) { assert(last == -1); return v.val; },
            [&](add_op) {
                auto arg = next_term<R>(expr);
                assert(std::holds_alternative<value>(arg));
                return last_val + std::get<value>(arg).val;
            },
            [&](mult_op) {
                if constexpr (R == rules::part1) {
                    auto arg = next_term<R>(expr);
                    assert(std::holds_alternative<value>(arg));
                    return last_val * std::get<value>(arg).val;
                } else if constexpr (R == rules::part2) {
                    return last_val * evaluate_recursive<R>(expr);
                }
            }
        }, next_term<R>(expr));
    }

    return last_val;
}

template <rules R>
const auto evaluate_expression = [](std::string_view expr) -> int64_t
{
    return evaluate_recursive<R>(expr);
};

template <rules R>
const auto evaluate_all = [](std::string_view input)
{
    return flow::split(input, '\n')
            // HACKHACKHACK
            .map([](auto f) { size_t sz = f.size(); return std::string_view{&*f.next(), sz}; })
            .map(evaluate_expression<R>)
            .sum();
};

static_assert(evaluate_expression<rules::part1>("1 + 2 * 3 + 4 * 5 + 6") == 71);
static_assert(evaluate_expression<rules::part1>("1 + (2 * 3) + (4 * (5 + 6))") == 51);
static_assert(evaluate_expression<rules::part1>("2 * 3 + (4 * 5)") == 26);
static_assert(evaluate_expression<rules::part1>("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 437);
static_assert(evaluate_expression<rules::part1>("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 12240);
static_assert(evaluate_expression<rules::part1>("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2)") == 13632);

static_assert(evaluate_expression<rules::part2>("1 + 2 * 3 + 4 * 5 + 6") == 231);
static_assert(evaluate_expression<rules::part2>("1 + (2 * 3) + (4 * (5 + 6))") == 51);
static_assert(evaluate_expression<rules::part2>("2 * 3 + (4 * 5)") == 46);
static_assert(evaluate_expression<rules::part2>("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 1445);
static_assert(evaluate_expression<rules::part2>("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 669060);
static_assert(evaluate_expression<rules::part2>("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2)") == 23340);

int main(int argc, char** argv)
{
    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    std::ifstream file(argv[1]);
    const std::string input = flow::from_istreambuf(file).to_string();

    fmt::print("Part 1: {}\n", evaluate_all<rules::part1>(input));
    fmt::print("Part 2: {}\n", evaluate_all<rules::part2>(input));
}