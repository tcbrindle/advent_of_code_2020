
#include "../common.hpp"


struct node {
    int value;
    node* next;

    // A constructor to keep make_unique happy
    node(int v, node* n = nullptr)
        : value(v), next(n)
    {}

    constexpr node* extract(int size)
    {
        assert(this->next);
        auto ret = this->next;
        auto temp = ret;

        while (size-- > 1) {
            assert(temp->next);
            temp = temp->next;
        }

        // Stitch things back together
        this->next = temp->next;
        temp->next = nullptr;

        return ret;
    }

    constexpr void insert(node* other)
    {
        auto temp = this->next;
        this->next = other;
        while (other->next) {
            other = other->next;
        }
        other->next = temp;
    }

    // Make our nodes iterable with libflow
    auto to_flow() -> flow::async<int&> {
        node* cur = this;
        while (cur) {
            co_yield cur->value;
            cur = cur->next;
        }
    }
};

const auto play_game = [](auto const& input, const int moves)
{
    const auto size = input.size();

    std::vector<std::unique_ptr<node>> cache{};
    cache.push_back(std::make_unique<node>(input.front(), nullptr));

    flow::from(input).drop(1).for_each([&](int i) {
        auto n = std::make_unique<node>(i, nullptr);
        cache.back()->next = n.get();
        cache.push_back(std::move(n));
    });

    // make the list circular
    node* head = cache.front().get();
    cache.back()->next = head;

    std::sort(cache.begin(), cache.end(),
              [](const auto& p1, const auto& p2) {
                  return p1->value < p2->value;
              });

    flow::ints(0, moves).fold([&](node* cur, int) {
        node* next3 = cur->extract(3);

        int dest = cur->value - 1;
        dest = dest < 1 ? size : dest;

        while (flow::contains(*next3, dest)) {
            --dest;
            dest = dest < 1 ? size : dest;
        }

        // Lookup the pointer with value dest and insert after it
        cache[dest - 1]->insert(next3);

        return cur->next;
    }, head);

    return cache[0]->to_flow().take(9).to_vector();
};

const auto part1 = [](std::array<int, 9> array, int moves)
{
    auto output = play_game(array, moves);

    return flow::drop(output, 1)
            .fold_first([](int acc, int val) {
                return 10 * acc + val;
            }).value();
};

const auto part2 = [](const std::array<int, 9>& input)
{
    // Prepare the input
    auto vec = flow::copy(input)
                    .chain(flow::iota(10, 1'000'001)) // half-open...
                    .to_vector();

    // This might take a while...
    auto output = play_game(vec, 10'000'000);

    return flow::drop(output, 1).take(2).as<int64_t>().product();

};

#ifndef NDEBUG
constexpr auto test_input = std::array{3, 8, 9, 1, 2, 5, 4, 6, 7};
#endif

// My personal input value
constexpr auto my_input = std::array{2, 1, 9, 7, 4, 8, 3, 6, 5};

int main()
{
    assert(part1(test_input, 10) == 92658374);
    assert(part1(test_input, 100) == 67384529);
    assert(part2(test_input) == 149245887792);

    fmt::print("Part 1: {}\n", part1(my_input, 100));
    fmt::print("Part 2: {}\n", part2(my_input));
}