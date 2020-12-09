
#include "../common.hpp"

enum class opcode {
    nop, acc, jmp
};

struct instruction {
    opcode op;
    int arg;
};

enum class exit_status {
    success, loop
};

struct result {
    int acc;
    exit_status status;
};

const auto run_program = [](const auto& program) -> result
{
    int acc = 0;
    size_t iptr = 0;
    std::vector<bool> seen(program.size(), false);

    while (iptr < program.size()) {
        if (seen[iptr]) {
            return {acc, exit_status::loop};
        }

        auto& inst = program[iptr];
        seen[iptr] = true;

        switch (inst.op) {
        case opcode::nop:
            iptr += 1;
            break;
        case opcode::acc:
            acc += inst.arg;
            iptr += 1;
            break;
        case opcode::jmp:
            iptr += inst.arg;
            break;
        }
    }

    return {acc, exit_status::success};
};

const auto swap_opcodes = [](opcode o) {
    if (o == opcode::nop) {
        return opcode::jmp;
    } else if (o == opcode::jmp) {
        return opcode::nop;
    } else {
        return o;
    }
};

const auto part1 = [](const auto& input)
{
    return run_program(input).acc;
};

const auto part2 = [](const auto& input)
{
    // This is a bit "impure", but it saves us having to take a copy
    // each time we go through the loop, or adding special "if swapped"
    // logic to the executor
    auto cache = input;

    return flow::ints(0, input.size())
        .filter([&input](auto idx) { return input[idx].op != opcode::acc; })
        .map([&cache](auto idx) {
            cache[idx].op = swap_opcodes(cache[idx].op);
            auto res = run_program(cache);
            cache[idx].op = swap_opcodes(cache[idx].op);
            return res;
        })
        .filter([](auto res) { return res.status == exit_status::success; })
        .map(&result::acc)
        .next().value();
};

const auto parse_opcode = [](const std::string_view str) -> opcode
{
    if (str.starts_with("nop")) return opcode::nop;
    if (str.starts_with("acc")) return opcode::acc;
    if (str.starts_with("jmp")) return opcode::jmp;
    throw std::runtime_error(fmt::format("Unknown opcode {}", str));
};

const auto parse_input = [](auto&& istream)
{
    return aoc::lines(istream)
            .map([](std::string const& str) -> instruction {
                return {parse_opcode(str), std::atoi(str.c_str() + 3)};
            })
            .to_vector();
};

constexpr auto test_data =
R"(nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6)";

int main(int argc, char** argv)
{
    if (argc < 2) {
        fmt::print(stderr, "Error, no input");
        return -1;
    }

    {
        const auto test = parse_input(std::istringstream(test_data));
        assert(part1(test) == 5);
        assert(part2(test) == 8);
    }

    const auto program = parse_input(std::ifstream(argv[1]));

    fmt::print("Part 1: accumulator was {}\n", part1(program));

    fmt::print("Part 2: accumulator was {}\n", part2(program));
}