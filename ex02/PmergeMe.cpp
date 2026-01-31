#include "PmergeMe.hpp"
#include <algorithm>
#include <cassert>

template <typename Container>
static bool isSizeOdd(Container &C)
{
    if ((C.size() % 2) == 0)
        return (false);
    return (true);
}

/*  Ford-Johnson insertion-order generator – no duplicates, O(n)      */
void PmergeMe::buildInsertionOrder(std::size_t n, std::vector<std::size_t> &out)
{
    out.clear();
    if (n == 0)
        return;

    /* ---- Jacobsthal milestones < n (1, 3, 5, 11, …) ---- */
    std::vector<size_t> jac;
    for (size_t a = 1, b = 1; b - 1 < n; )
    {
        size_t k = b - 1;           // Jk – 1   (Knuth’s notation)
        if (k && k < n)
            jac.push_back(k);
        size_t c = b + 2 * a;       // next Jacobsthal
        a = b;
        b = c;
    }

    /* ---- walk left-to-right, injecting each milestone when reached ---- */
    size_t nextMilestone = jac.empty() ? n : jac.front();
    size_t mIndex = 0;

    for (size_t i = 0; i < n; i++)
    {
        if (i == nextMilestone)
        {
            out.push_back(nextMilestone);            // Jk-1 itself
            if (++mIndex < jac.size())
                nextMilestone = jac[mIndex];         // advance to next J
            else
                nextMilestone = n;                   // no more milestones
        }
        else
            out.push_back(i);                        // plain sequential index
    }

    assert(out.size() == n);
}


void PmergeMe::insertPendVector(std::vector<int> &dst, const std::vector<int> &pend)
{
    std::vector<std::size_t> order;
    buildInsertionOrder(pend.size(), order);

    std::size_t sortedPrefix = 1;   // first leader already sorted
    for (std::size_t k = 0; k < order.size(); k++)
    {
        std::size_t idx = order[k];
        const int   val = pend[idx];
        /* binary-search only the sorted part accumulated so far */
        dst.insert(std::lower_bound(dst.begin(), dst.begin() + sortedPrefix, val), val);
        sortedPrefix++;
    }
}


void PmergeMe::pairSortVector(std::vector<int> &vec)
{
    bool odd = isSizeOdd(vec);
    int leftover = 0;
    if (odd)
    {
        leftover = vec.back();
        vec.pop_back();
    }

    std::vector<int> winners;
    std::vector<int> loosers;
    winners.reserve(vec.size() / 2);
    loosers.reserve(vec.size() / 2);

    for (size_t i = 0; i < vec.size(); i += 2)
    {
        int a = vec[i], b = vec[i + 1];
        if (a < b)
            std::swap(a, b);
        winners.push_back(a);
        loosers.push_back(b);
    }

    if (winners.size() > 1)
        pairSortVector(winners);          // recursion on winners

    vec.swap(winners);                    // winners become the base chain
    insertPendVector(vec, loosers);     // Ford-Johnson insertions

    if (odd)                              // append the last stray elem.
        vec.insert(std::lower_bound(vec.begin(), vec.end(), leftover), leftover);
}

// void PmergeMe::pairSortVector(std::vector<int> &vec)
// {
//     const bool odd = vec.size() & 1;
//     int leftover  = odd ? vec.back() : 0;

//     std::vector<int> winners;
//     std::vector<int> loosers;
//     winners.reserve((vec.size() + 1) / 2);
//     loosers.reserve(vec.size() / 2);

//     for (std::size_t i = 0; i + 1 < vec.size(); i += 2)
//     {
//         int a = vec[i], b = vec[i + 1];
//         if (a < b)
//             std::swap(a, b);
//         winners.push_back(a);
//         loosers.push_back(b);
//     }

//     if (winners.size() > 1)
//         pairSortVector(winners);          // recursion on winners

//     vec.swap(winners);                    // winners become the base chain
//     insertPendVector(vec, loosers);     // Ford-Johnson insertions

//     if (odd)                              // append the last stray elem.
//         vec.insert(std::lower_bound(vec.begin(), vec.end(), leftover), leftover);
// }

void PmergeMe::sortVector(std::vector<int> &vec)
{
    if (vec.size() < 2)
        return;
    pairSortVector(vec);
}


void PmergeMe::insertPendDeque(std::deque<int> &dst, const std::deque<int> &pend)
{
    std::vector<std::size_t> order;
    buildInsertionOrder(pend.size(), order);

    std::size_t sortedPrefix = 1;
    for (std::size_t k = 0; k < order.size(); k++)
    {
        std::size_t idx = order[k];
        const int   val = pend[idx];
        dst.insert(std::lower_bound(dst.begin(), dst.begin() + sortedPrefix, val), val);
        sortedPrefix++;
    }
}

void PmergeMe::pairSortDeque(std::deque<int> &deq)
{
    const bool odd = deq.size() & 1;
    int leftover  = odd ? deq.back() : 0;

    std::deque<int> winners;
    std::deque<int> loosers;
    winners.resize((deq.size() + 1) / 2);
    loosers.resize(deq.size() / 2);

    for (std::size_t i = 0, l = 0, f = 0; i + 1 < deq.size(); i += 2)
    {
        int a = deq[i];
        int b = deq[i + 1];
        if (a < b)
            std::swap(a, b);
        winners[l++] = a;
        loosers[f++] = b;
    }

    if (winners.size() > 1)
        pairSortDeque(winners);

    deq.swap(winners);
    insertPendDeque(deq, loosers);

    if (odd)
        deq.insert(std::lower_bound(deq.begin(), deq.end(), leftover), leftover);
}

void PmergeMe::sortDeque(std::deque<int> &deq)
{
    if (deq.size() < 2)
        return;
    pairSortDeque(deq);
}