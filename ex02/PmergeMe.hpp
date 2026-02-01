#pragma once
#include <vector>
#include <deque>

class PmergeMe
{
public:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    static void sortVector(std::vector<int> &vec);
    static void sortDeque(std::deque<int> &deq);

private:
    static void pairSortVector(std::vector<int> &vec);
    static void pairSortDeque(std::deque<int>& deq);
};