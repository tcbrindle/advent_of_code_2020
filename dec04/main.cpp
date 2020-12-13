
#include "../common.hpp"
#include "ctre.hpp"
#include "nanorange.hpp"

// I'm not a fan of the parsing problems that AoC gives sometimes
// Perhaps they're fun in other languages, but most definitely not C++...
// There are certainly smarter ways to do all this, but my only goal here
// is to get it over with and hope tomorrow's problem is more fun.
// Beautiful code can wait for a less tedious task.

// Note: The use of CTRE in this solution ICEs both AppleClang and
// upstream LLVM Clang. Yay. GCC10 works though, and this does
// at least mean we can use the nifty C++20 CTRE syntax.

using passport = std::map<std::string, std::string>;

constexpr auto is_valid_pt1 = [](const passport& pp) -> bool {
    constexpr const char* const required_fields[] = {
        "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
    };

    return flow::all(required_fields, [&pp] (auto f) {
        return pp.find(f) != pp.end();
    });
};

constexpr auto is_valid_pt2 = [](const passport& pp) -> bool {

    auto valid_byr = [](const auto& s) {
        auto i = std::stoi(s);
        return s.size() == 4 && i >= 1920 && i <= 2002;
    };

    auto valid_iyr = [](const auto& s) {
        auto i = std::stoi(s);
        return s.size() == 4 && i >= 2010 && i <= 2020;
    };

    auto valid_eyr = [](const auto& s) {
        auto i = std::stoi(s);
        return s.size() == 4 && i >= 2020 && i <= 2030;
    };

    auto valid_hgt = [](const auto& s) {
        if (auto [m, n, u] = ctre::match<"(\\d+)(in|cm)">(s); m) {
            auto val = std::stoi(n.to_string());
            if (u == "cm") {
                return val >= 150 && val <= 193;
            } else {
                return val >= 59 && val <= 76;
            }
        }
        return false;
    };

    auto valid_hcl = [](const auto s) {
        return ctre::match<"#[0-9a-f]{6}">(s);
    };

    auto valid_ecl = [](const auto s) {
        return ctre::match<"amb|blu|brn|gry|grn|hzl|oth">(s);
    };

    auto valid_pid = [](const auto s) {
        return ctre::match<"\\d{9}">(s);
    };

    return is_valid_pt1(pp) &&
           valid_byr(pp.at("byr")) &&
           valid_iyr(pp.at("iyr")) &&
           valid_eyr(pp.at("eyr")) &&
           valid_hgt(pp.at("hgt")) &&
           valid_hcl(pp.at("hcl")) &&
           valid_ecl(pp.at("ecl")) &&
           valid_pid(pp.at("pid"));
};

constexpr auto parse_field = [](std::string_view str) -> std::pair<std::string, std::string>
{
    auto [_, left, right] = ctre::match<"([a-z]+):(\\S+)">(str);
    return {left.to_string(), right.to_string()};

};

constexpr auto parse_passport = [](std::string input) -> passport
{
    // C++ iostreams very handily strip whitespace for us, so let's make use of that
    std::istringstream iss(std::move(input));
    return flow::from_istream<std::string>(iss)
                .map(parse_field)
                .to<passport>();
};

constexpr auto parse_input = [](std::string str) -> std::vector<passport> {

    // Sadly, libflow's version of split only accepts a single element as
    // a delimiter at the moment, and so can't search for "\n\n".
    // Equally sadly, GCC10's version of std::views::split crashes when
    // trying to do the same thing, so here we need to break out
    // trusty old NanoRange, which of course works perfectly ;)

    using namespace std::string_view_literals;

    auto view = str
        | nano::views::split("\n\n"sv)
        | nano::views::transform([](auto r) {
            auto v = nano::views::common(r);
            return std::string(v.begin(), v.end()); })
        | nano::views::transform(parse_passport)
        | nano::views::common;

    return std::vector(view.begin(), view.end());
};

int main(int argc, char** argv)
{
    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    std::ifstream file(argv[1]);

    auto in_str = flow::from_istreambuf(file).to_string();

    const auto passports = parse_input(in_str);

    fmt::print("Part 1: found {} valid passports\n",
        flow::count_if(passports, is_valid_pt1));

    fmt::print("Part 2: found {} valid passports\n",
        flow::count_if(passports, is_valid_pt2));
}