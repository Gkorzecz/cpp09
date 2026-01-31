#include "PmergeMe.hpp"
#include <algorithm>
#include <cassert>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
{
    (void)other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    (void)other;
    return (*this);
}

PmergeMe::~PmergeMe() {}

template <typename Container>
static bool isSizeOdd(Container &C)
{
    if ((C.size() % 2) == 0)
        return (false);
    return (true);
}

static void buildInsertionOrder(size_t n, std::vector<size_t> &out)
{
    out.clear();
    if (n == 0) return;

    // Jacobsthal numbers: J0=0, J1=1, Jk = J_{k-1} + 2*J_{k-2}
    std::vector<size_t> J;
    J.push_back(0);
    J.push_back(1);
    while (true)
    {
        size_t next = J[J.size() - 1] + 2 * J[J.size() - 2];
        J.push_back(next);
        if (next >= n) break;              // stop after we pass or hit n
    }

    // Emit descending blocks: [Jk-1 ... (Jk-1_prev)] for k=2.., capped at n
    size_t prev = 0;
    for (size_t k = 2; k < J.size(); ++k)
    {
        size_t jk = J[k];
        if (jk > n) jk = n;

        // push indices (jk-1 ... prev) descending
        for (size_t t = jk; t > prev; --t)
            out.push_back(t - 1);

        prev = jk;
        if (jk == n) break;
    }

    // for very small n (e.g. n==1) the loop above may not run; handle it
    if (out.empty())
        out.push_back(0);

    assert(out.size() == n);
}

template <class Seq>
static void insertPendSeqBounded(Seq &dst, const Seq &pend, const Seq &leadersOfPend)
{
    std::vector<size_t> order;
    buildInsertionOrder(pend.size(), order);

    for (size_t k = 0; k < order.size(); ++k)
    {
        const size_t idx = order[k];
        const typename Seq::value_type val = pend[idx];
        const typename Seq::value_type leader = leadersOfPend[idx];

        // Find the current position of the leader in the (partially built) base chain.
        // Leaders are unique and are already present in 'dst'.
        typename Seq::iterator leaderIt = std::lower_bound(dst.begin(), dst.end(), leader);

        // Binary-search ONLY up to and including the leader position.
        typename Seq::iterator pos = std::lower_bound(dst.begin(), leaderIt + 1, val);

        dst.insert(pos, val);
    }
}

void PmergeMe::insertPendVector(std::vector<int> &dst, const std::vector<int> &pend)
{
    // This overload is not used directly anymore by pairSortVector,
    // but keep it to match your header if other code calls it.
    // Fall back to unbounded insertion for compatibility.
    // (You can delete this and change the header if you prefer.)
    std::vector<int> dummy_leaders(pend.size(), dst.empty() ? 0 : dst[0]);
    insertPendSeqBounded(dst, pend, dummy_leaders);
}

void PmergeMe::insertPendDeque(std::deque<int> &dst, const std::deque<int> &pend)
{
    std::deque<int> dummy_leaders(pend.size(), dst.empty() ? 0 : dst.front());
    insertPendSeqBounded(dst, pend, dummy_leaders);
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

    // 1) Build pairs (leader, loser) — keep ORIGINAL leaders to align with losers
    std::vector<int> leaders;    // original unsorted leaders
    std::vector<int> losers;
    leaders.reserve(vec.size() / 2);
    losers.reserve(vec.size() / 2);

    for (size_t i = 0; i < vec.size(); i += 2)
    {
        int a = vec[i], b = vec[i + 1];
        if (a < b)
            std::swap(a, b);
        leaders.push_back(a);
        losers.push_back(b);
    }

    // 2) Base chain = SORTED copy of leaders
    std::vector<int> base = leaders;
    if (base.size() > 1)
        pairSortVector(base);   // recursion on the copy

    vec.swap(base);             // vec == sorted winners (base chain)

    // 3) Insert losers, bounded by *their own* (original) leader
    insertPendSeqBounded(vec, losers, leaders);

    // 4) Stray element if odd
    if (odd)
        vec.insert(std::lower_bound(vec.begin(), vec.end(), leftover), leftover);
}

void PmergeMe::sortVector(std::vector<int> &vec)
{
    if (vec.size() < 2)
        return;
    pairSortVector(vec);
}


void PmergeMe::pairSortDeque(std::deque<int> &deq)
{
    bool odd = isSizeOdd(deq);
    int leftover = 0;
    if (odd)
    {
        leftover = deq.back();
        deq.pop_back();
    }

    // 1) Build pairs (leader, loser) — keep ORIGINAL leaders
    std::deque<int> leaders;    // original unsorted leaders
    std::deque<int> losers;
    leaders.resize(deq.size() / 2);
    losers.resize(deq.size() / 2);

    for (size_t i = 0, p = 0; i < deq.size(); i += 2, ++p)
    {
        int a = deq[i], b = deq[i + 1];
        if (a < b)
            std::swap(a, b);
        leaders[p] = a;
        losers[p]  = b;
    }

    // 2) Base chain = SORTED copy of leaders
    std::deque<int> base = leaders;
    if (base.size() > 1)
        pairSortDeque(base);    // recursion on the copy

    deq.swap(base);             // deq == sorted winners (base chain)

    // 3) Insert losers, bounded by *their own* leader
    insertPendSeqBounded(deq, losers, leaders);

    // 4) Stray element if odd
    if (odd)
        deq.insert(std::lower_bound(deq.begin(), deq.end(), leftover), leftover);
}

void PmergeMe::sortDeque(std::deque<int> &deq)
{
    if (deq.size() < 2)
        return;
    pairSortDeque(deq);
}