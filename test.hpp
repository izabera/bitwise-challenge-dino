#ifndef TEST_HPP
#define TEST_HPP

#include <fnmatch.h>
#include <stdio.h>
#include <vector>

using result = bool;
static constexpr result FAIL = false;
static constexpr result PASS = true;
static constexpr result OK = true;

struct test {
    const char *name;
    result (*run)();
};
static inline std::vector<test> tests;

struct test_helper {
    const char *name;
    result (*run)();
    test_helper(const char *name) : name(name) {}

    template <typename F>
    test_helper& operator+(F f) { run = f; return *this; }
};

struct test_adder {
    test_adder(test_helper th) { tests.push_back({th.name, +th.run}); }
};

#define cat_(a,b) a##b
#define cat(a,b) cat_(a,b)
#define TEST(name) \
    static inline test_adder cat(ta,__COUNTER__) [[maybe_unused]] = \
    test_helper(name) + []() -> result

static inline bool run_tests(int argc, char **argv) {
    int pass = 0, fail = 0;

    for (const auto& test : tests) {
        bool run = false, skip = false, all_negative = true;

        for (auto i = 1; i < argc; i++) {
            if (argv[i][0] == '!' || argv[i][0] == '-') {
                if (fnmatch(argv[i]+1, test.name, 0) == 0) {
                    skip = true;
                    break;
                }
            }
            else {
                all_negative = false;
                if (fnmatch(argv[i], test.name, 0) == 0) {
                    run = true;
                    break;
                }
            }
        }

        if (skip || (!all_negative && !run))
            continue;

        printf("test: %s\n", test.name);
        if (test.run() == FAIL) {
            fail++;
            printf("test %s failed\n", test.name);
        } else
            pass++;
    }

    int ran = pass + fail;
    printf("pass=%d/%d\n", pass, ran);
    printf("fail=%d/%d\n", fail, ran);
    printf("skipped=%d\n", int(tests.size() - ran));
    return fail == 0;
}

#endif
