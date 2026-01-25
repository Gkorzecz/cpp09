#pragma once
#include <vector>
#include <deque>

class PmergeMe
{
public:
    /* public API expected by the subject */
    static void sortVector(std::vector<int>& vec);
    static void sortDeque (std::deque<int>&  deq);

private:                    /* internal helpers */
    /* Jacobsthal + insertion-order generator (header-only for reuse) */
    static void buildInsertionOrder(std::size_t n, std::vector<std::size_t>& out);

    /* vector specialisation */
    static void pairSortVector  (std::vector<int>& vec);
    static void insertPendVector(std::vector<int>& dst,
                                 const std::vector<int>& pend);

    /* deque  specialisation */
    static void pairSortDeque  (std::deque<int>& deq);
    static void insertPendDeque(std::deque<int>& dst,
                                const std::deque<int>& pend);
};