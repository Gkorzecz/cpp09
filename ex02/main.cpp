#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <set>
#include <cstdlib>
#include <climits>
#include <ctime>

static double timeElapsed(const timespec &a, const timespec &b)
{
    long sec  = b.tv_sec  - a.tv_sec;
    long nsec = b.tv_nsec - a.tv_nsec;
    return (static_cast<double>(sec) * 1e6 + static_cast<double>(nsec) / 1e3);
}


static void hasDuplicates(const std::vector<int> &vec)
{
    std::set<int> dupliTest;

    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (!dupliTest.insert(vec[i]).second)
            throw std::invalid_argument("No duplicates allowed");
    }
}

static void validateArgs(char *argv[], int argc, std::vector<int> &original)
{
    std::string tested;
    for (int i = 1; i < argc; i++)
    {
        tested = argv[i];
        if (tested.empty())
            throw std::invalid_argument("Null or empty argument");
        for (unsigned long j = 0; j < tested.size(); j++)
        {   
            if (!isdigit(argv[i][j]))
                throw std::invalid_argument("Only positive integer accepted");
        }
        char* end = 0;
        long v = std::strtol(argv[i], &end, 10);
        if (v < 0 || v > INT_MAX)
            throw std::out_of_range("Out-of-bound argument");
        original.push_back(static_cast<int>(v));
    }
}

template <typename Container>
static void isContainerSorted(Container &C)
{
    size_t i = 1;
    while (i < C.size())
    {
        if (C[i] < C[i - 1])
            return;
        i++;
    }
    throw std::logic_error("Container is sorted");
}


template <typename T>
void printContainer(const T &c)
{
    for (typename T::const_iterator it = c.begin(); it != c.end(); it++)
        std::cout << *it << ' ';
    std::cout << "\n";
}

int main(int argc, char* argv[])
{
    if (argc <= 2)
    {
        std::cout << "Not enough arguments (min 2)" << std::endl;
        return (1);
    }

    timespec ta, tb;
    clock_gettime(CLOCK_MONOTONIC, &ta);
    std::vector<int> original;
    original.reserve(argc - 1);
    try
    {
        validateArgs(argv, argc, original);
        hasDuplicates(original);
        isContainerSorted(original);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return (1);
    }
    clock_gettime(CLOCK_MONOTONIC, &tb);
    double allUs = timeElapsed(ta, tb);
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process data : " << allUs << " us\n";
    

    std::cout << "Before: ";
    printContainer(original);

    /* SORTED WITH VECTOR */
    std::vector<int> vec = original;
    timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    PmergeMe::sortVector(vec);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double vecUs = timeElapsed(t0, t1);

    /* SORTED WITH DEQUE */
    std::deque<int> deq(original.begin(), original.end());
    timespec t2, t3;
    clock_gettime(CLOCK_MONOTONIC, &t2);
    PmergeMe::sortDeque(deq);
    clock_gettime(CLOCK_MONOTONIC, &t3);
    double deqUs = timeElapsed(t2, t3);

    std::cout << "After:  ";
    printContainer(vec);

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << original.size() << " elements with std::vector : " << vecUs << " us\n";
    std::cout << "Time to process a range of " << original.size()   << " elements with std::deque  : " << deqUs << " us\n";

    /* FINAL TEST*/
    try
    {
        isContainerSorted(vec);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    try
    {
        isContainerSorted(deq);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return (0);
}