#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <set>
#include <cstdlib>
#include <climits>
#include <ctime>

static double timeElapsed(const timespec& a, const timespec& b)
{
    double timElaps = (b.tv_nsec - a.tv_nsec) / 1e3 + (b.tv_sec - a.tv_sec) / 1e6;
    return (timElaps);
}

static bool toPositive(const char* s, int &out)
{
    if (!s || !*s)
        return (false);
    char* end = 0;
    long v = std::strtol(s, &end, 10);
    if (*end || v <= 0 || v > INT_MAX)
        return (false);
    out = static_cast<int>(v);
    return (true);
}

template <class C>
void printSeq(const C& c)
{
    for (typename C::const_iterator it = c.begin(); it != c.end(); ++it)
        std::cout << *it << ' ';
}

int main(int ac, char** av)
{
    if (ac < 2)
        return (std::cerr << "Error" << std::endl, 1);

    std::vector<int> original;
    original.reserve(ac - 1);
    // std::set<int> seen;                // reject duplicates
    for (int i = 1, v; i < ac; ++i)
    {
        // if (!toPositive(av[i], v) || !seen.insert(v).second)
        //     return (std::cerr << "Error" << std::endl, 1);
        if (!toPositive(av[i], v))
            return (std::cerr << "Error" << std::endl, 1);
        original.push_back(v);
    }

    std::cout << "Before: ";
    printSeq(original);
    std::cout << "\n";

    /* ---------- std::vector ---------- */
    std::vector<int> vec = original;
    timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    PmergeMe::sortVector(vec);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double vecUs = timeElapsed(t0, t1);

    /* ---------- std::deque  ---------- */
    std::deque<int> deq(original.begin(), original.end());
    timespec t2, t3;
    clock_gettime(CLOCK_MONOTONIC, &t2);
    PmergeMe::sortDeque(deq);
    clock_gettime(CLOCK_MONOTONIC, &t3);
    double deqUs = timeElapsed(t2, t3);

    std::cout << "After:  ";
    printSeq(vec);
    std::cout << "\n";

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << original.size()
              << " elements with std::vector : " << vecUs << " us\n";
    std::cout << "Time to process a range of " << original.size()
              << " elements with std::deque  : " << deqUs << " us\n";
    return 0;
}