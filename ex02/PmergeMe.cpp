#include "PmergeMe.hpp"
#include <algorithm>   // lower_bound, swap
#include <cassert>

/* =============================================================
 * Jacobsthal-based insertion order (Ford-Johnson, A-'63, TAOCP 3 §5.3.1)
 * ============================================================= */
/* ------------------------------------------------------------------ */
/*  Ford-Johnson insertion-order generator – no duplicates, O(n)      */
void PmergeMe::buildInsertionOrder(std::size_t n, std::vector<std::size_t>& out)
{
    out.clear();
    if (n == 0)
        return;

    /* ---- Jacobsthal milestones < n (1, 3, 5, 11, …) ---- */
    std::vector<std::size_t> jac;
    for (std::size_t a = 1, b = 1; b - 1 < n; )
    {
        std::size_t k = b - 1;           // Jk – 1   (Knuth’s notation)
        if (k && k < n)
            jac.push_back(k);
        std::size_t c = b + 2 * a;       // next Jacobsthal
        a = b;
        b = c;
    }

    /* ---- walk left-to-right, injecting each milestone when reached ---- */
    std::size_t nextMilestone = jac.empty() ? n : jac.front();
    std::size_t mIndex = 0;

    for (std::size_t i = 0; i < n; ++i)
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

    /* sanity-check – *will* hold now */
    assert(out.size() == n);
}


/* =============================================================
 * std::vector specialisation
 * ============================================================= */
void PmergeMe::insertPendVector(std::vector<int>& dst,
                                const std::vector<int>& pend)
{
    std::vector<std::size_t> order;
    buildInsertionOrder(pend.size(), order);

    std::size_t sortedPrefix = 1;   // first leader already sorted
    for (std::size_t k = 0; k < order.size(); ++k)
    {
        std::size_t idx = order[k];
        const int   val = pend[idx];
        /* binary-search only the sorted part accumulated so far */
        dst.insert(std::lower_bound(dst.begin(),
                                    dst.begin() + sortedPrefix,
                                    val),
                   val);
        ++sortedPrefix;
    }
}

void PmergeMe::pairSortVector(std::vector<int>& vec)
{
    const bool odd = vec.size() & 1;
    int leftover  = odd ? vec.back() : 0;

    std::vector<int> leaders;
    std::vector<int> followers;
    leaders.reserve((vec.size() + 1) / 2);
    followers.reserve(vec.size() / 2);

    for (std::size_t i = 0; i + 1 < vec.size(); i += 2)
    {
        int a = vec[i], b = vec[i + 1];
        if (a < b) std::swap(a, b);
        leaders.push_back(a);
        followers.push_back(b);
    }

    if (leaders.size() > 1)
        pairSortVector(leaders);          // recursion on leaders

    vec.swap(leaders);                    // leaders become the base chain
    insertPendVector(vec, followers);     // Ford-Johnson insertions

    if (odd)                              // append the last stray elem.
        vec.insert(std::lower_bound(vec.begin(), vec.end(), leftover),
                   leftover);
}

void PmergeMe::sortVector(std::vector<int>& vec)
{
    if (vec.size() < 2) return;
    pairSortVector(vec);
}

/* =============================================================
 * std::deque specialisation
 * (same logic but on a random-access deque)
 * ============================================================= */
void PmergeMe::insertPendDeque(std::deque<int>& dst,
                               const std::deque<int>& pend)
{
    std::vector<std::size_t> order;
    buildInsertionOrder(pend.size(), order);

    std::size_t sortedPrefix = 1;
    for (std::size_t k = 0; k < order.size(); ++k)
    {
        std::size_t idx = order[k];
        const int   val = pend[idx];
        dst.insert(std::lower_bound(dst.begin(),
                                    dst.begin() + sortedPrefix,
                                    val),
                   val);
        ++sortedPrefix;
    }
}

void PmergeMe::pairSortDeque(std::deque<int>& deq)
{
    const bool odd = deq.size() & 1;
    int leftover  = odd ? deq.back() : 0;

    std::deque<int> leaders;
    std::deque<int> followers;
    leaders.resize((deq.size() + 1) / 2);
    followers.resize(deq.size() / 2);

    for (std::size_t i = 0, l = 0, f = 0; i + 1 < deq.size(); i += 2)
    {
        int a = deq[i], b = deq[i + 1];
        if (a < b) std::swap(a, b);
        leaders[l++]   = a;
        followers[f++] = b;
    }

    if (leaders.size() > 1)
        pairSortDeque(leaders);

    deq.swap(leaders);
    insertPendDeque(deq, followers);

    if (odd)
        deq.insert(std::lower_bound(deq.begin(), deq.end(), leftover),
                   leftover);
}

void PmergeMe::sortDeque(std::deque<int>& deq)
{
    if (deq.size() < 2) return;
    pairSortDeque(deq);
}