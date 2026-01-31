#pragma once
#include <vector>
#include <deque>

class PmergeMe
{
public:
    static void sortVector(std::vector<int> &vec);
    static void sortDeque(std::deque<int> &deq);

private:
    static void buildInsertionOrder(std::size_t n, std::vector<std::size_t> &out);

    static void pairSortVector(std::vector<int>& vec);
    static void insertPendVector(std::vector<int>& dst, const std::vector<int>& pend);

    static void pairSortDeque(std::deque<int>& deq);
    static void insertPendDeque(std::deque<int>& dst, const std::deque<int>& pend);
};