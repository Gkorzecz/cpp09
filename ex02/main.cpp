#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <string>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <sys/time.h>   // gettimeofday

static double elapsedUs(const timeval& start, const timeval& end)
{
    return (1e6 * static_cast<double>(end.tv_sec  - start.tv_sec)  + static_cast<double>(end.tv_usec - start.tv_usec));
}

static bool validPositive(const char* s, int& out)
{
    if (!s || !*s)
        return (false);
    char* end = 0;
    long v = std::strtol(s, &end, 10);
    if (*end != '\0' || v <= 0 || v > INT_MAX)
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
        std::cout << *it;
        if (++it != c.end())
            std::cout << ' ';
    }
}

int main(int ac, char** av)
{
    if (ac < 2)
    {
        std::cout << "Error" << std::endl;
        return 1;
    }

    std::vector<int> original;
    original.reserve(ac - 1);
    for (int i = 1, v; i < ac; ++i)
    {
        if (!validPositive(av[i], v))
        {
            std::cout << "Error" << std::endl;
            return 1;
        }
        original.push_back(v);
    }
    std::cout << "Before: ";
    printSeq(original);
    std::cout << std::endl;
    std::vector<int> vec = original;
    timeval vBeg, vEnd;
    gettimeofday(&vBeg, 0);
    PmergeMe::sortVector(vec);
    gettimeofday(&vEnd, 0);
    double vecUs = elapsedUs(vBeg, vEnd);

    std::deque<int> deq(original.begin(), original.end());
    timeval dBeg, dEnd;
    gettimeofday(&dBeg, 0);
    PmergeMe::sortDeque(deq);
    gettimeofday(&dEnd, 0);
    double deqUs = elapsedUs(dBeg, dEnd);

    std::cout << "After:  ";
    printSeq(vec);
    std::cout << std::endl;

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << original.size() << " elements with std::vector : " 
              << vecUs << " us" << std::endl;

    std::cout << "Time to process a range of " << original.size() << " elements with std::deque  : "
              << deqUs << " us" << std::endl;

    return 0;
}