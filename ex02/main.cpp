#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <string>
#include <cstdlib>
#include <climits>
#include <sys/time.h>
#include <time.h>


static double timeElapsed(const timespec& a, const timespec& b)
{
    double timElaps = (b.tv_nsec - a.tv_nsec) / 1e3;
    return (timElaps);
}

static bool validPositive(const char* s, int& out)
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


template <typename T>
void printSeq(const T& c)
{
    typename T::const_iterator it = c.begin();
    while (it != c.end())
    {
        std::cout << *it << ' ';
        it++;
    }
}


int main(int ac, char** av)
{
    if (ac < 2)
    {
        std::cout << "Error" << std::endl;
        return (1);
    }

    std::vector<int> original;
    original.reserve(ac - 1);
    for (int i = 1, v; i < ac; ++i)
    {
        if (!validPositive(av[i], v))
        {
            std::cout << "Error" << std::endl;
            return (1);
        }
        original.push_back(v);
    }

    std::cout << "Before: ";
    printSeq(original);
    std::cout << std::endl;

    std::vector<int> vec = original;
    timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    PmergeMe::sortVector(vec);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double vecUs = timeElapsed(t0, t1);

    std::deque<int> deq(original.begin(), original.end());
    timespec t2, t3;
    clock_gettime(CLOCK_MONOTONIC, &t2);
    PmergeMe::sortDeque(deq);
    clock_gettime(CLOCK_MONOTONIC, &t3);
    double deqUs  = timeElapsed(t2, t3);

    std::cout << "After:  ";
    printSeq(vec);
    std::cout << std::endl;

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << original.size() << " elements with std::vector : " << vecUs << " us" << std::endl;
    std::cout << "Time to process a range of " << original.size() << " elements with std::deque  : " << deqUs << " us" << std::endl;

    return (0);
}
