
#include "../common.hpp"

#include "../extern/ctre.hpp"

#include <bitset>
#include <variant>

template <typename... Ls>
struct visitor : Ls... { using Ls::operator()...; };

template <typename... Ls>
visitor(Ls...) -> visitor<std::remove_cvref_t<Ls>...>;

struct masks {
    std::bitset<36> on, off = UINT64_MAX, dummy;
};

struct write {
    uint64_t addr;
    uint64_t value;
};

using instruction = std::variant<masks, write>;

const auto parse_input = [](auto&& input) -> std::vector<instruction>
{
    return aoc::lines(input)
        .map([](const auto& line) -> instruction {
            if (auto [match, bits] = ctre::match<"^mask = ([X01]{36})$">(line); match) {
                return flow::reverse(bits).enumerate().fold([](masks m, auto p) {
                    auto [idx, c] = p;
                    switch (c) {
                        case '0': m.off.flip(idx); break;
                        case '1': m.on.flip(idx); break;
                        case 'X': m.dummy.flip(idx); break;
                    }
                    return m;
                }, masks{});
            } else if (auto [match, s_addr, s_val] = ctre::match<"^mem\\[(\\d+)\\] = (\\d+)$">(line); match) {
                uint64_t addr = aoc::try_parse<uint64_t>(s_addr).value();
                uint64_t value = aoc::try_parse<uint64_t>(s_val).value();
                return write{addr, value};
            } else {
                throw std::runtime_error(fmt::format("Unrecognised instruction \"{}\"", line));
            }
        })
        .to_vector();
};

struct machine_state {
    std::unordered_map<uint64_t, uint64_t> memory;
    masks masks_;
};

const auto part1 = [](const auto& instructions) -> uint64_t
{
    auto mem = flow::fold(instructions,
                                [](machine_state state, instruction instr) {
        return std::visit(visitor{
            [&state](masks m) { state.masks_ = m; return std::move(state); },
            [&state](write w) {
                w.value |= state.masks_.on.to_ullong();
                w.value &= state.masks_.off.to_ullong();
                state.memory[w.addr] = w.value;
                return std::move(state);
            }}, std::move(instr));
        }, machine_state{}).memory;

    return flow::values(std::move(mem)).sum();
};

const auto part2 = [](const auto& instructions)
{
    auto mem = flow::fold(instructions,
                          [](machine_state state, instruction instr) {
        return std::visit(visitor{
            [&state](masks m) { state.masks_ = m; return std::move(state); },
            [&state](write w) {

                // First, set all the 1s
                w.addr |= state.masks_.on.to_ullong();

                // For each bit...
                auto stack = flow::ints(0, 36)
                    //...that's set in the "dummy" bitset
                    .filter([&](auto bit_idx) { return state.masks_.dummy.test(bit_idx); })
                    // ...set all bitsets in the stack to have that bit set to zero,
                    // then add a copy of each bitset with that bit set to 1
                    // (We need to iterate by index here as we're modifying the vector as we go)
                    .fold([&](auto stack, auto bit_idx) {
                        const auto sz = stack.size();
                        flow::ints(0, sz)
                            .map([&](auto stack_idx) -> auto& { return stack[stack_idx]; })
                            .for_each([&](auto& b) {
                                auto cpy = b;
                                b.set(bit_idx, true);
                                cpy.set(bit_idx, false);
                                stack.push_back(cpy);
                            });
                            return stack;
                    }, std::vector<std::bitset<36>>{w.addr});

                flow::for_each(std::move(stack), [&](auto b) {
                    state.memory[b.to_ullong()] = w.value;
                });

                return std::move(state);
            }
        }, std::move(instr));
    }, machine_state{}).memory;

    return flow::values(std::move(mem)).sum();
};

constexpr auto test_data1 =
R"(mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
mem[8] = 11
mem[7] = 101
mem[8] = 0)";

constexpr auto test_data2 =
R"(mask = 000000000000000000000000000000X1001X
mem[42] = 100
mask = 00000000000000000000000000000000X0XX
mem[26] = 1)";

int main(int argc, char** argv)
{
    {
        const auto test_vec1 = parse_input(std::istringstream(test_data1));
        assert(part1(test_vec1) == 165);

        const auto test_vec2 = parse_input(std::istringstream(test_data2));
        assert(part2(test_vec2) == 208);
    }

    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    const auto vec = parse_input(std::ifstream(argv[1]));

    fmt::print("Part 1: {}\n", part1(vec));
    fmt::print("Part 2: {}\n", part2(vec));
}