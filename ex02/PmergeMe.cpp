#include "PmergeMe.hpp"
#include <algorithm>
#include <cassert>

PmergeMe::PmergeMe()
{}

PmergeMe::PmergeMe(const PmergeMe &other)
{
    (void)other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    (void)other;
    return (*this);
}

PmergeMe::~PmergeMe()
{}

template <typename Container>
static bool isSizeOdd(Container &C)
{
    if ((C.size() % 2) == 0)
        return (false);
    return (true);
}

/* example for n = 27 :
out = 0
      2 1
      4 3
      10 9 8 7 6 5
      20 19 18 17 16 15 14 13 12 11
      26 25 24 23 22 21*/
static void buildInsertionOrder(size_t n, std::vector<size_t> &out)
{
    out.clear();
    if (n == 0)
        return;

    /* Build Jacob and stop when we depassed size of container */
    std::vector<size_t> Jacob;
    Jacob.push_back(0);
    Jacob.push_back(1);
    while (true)
    {
        size_t next = Jacob[Jacob.size() - 1] + 2 * Jacob[Jacob.size() - 2];
        Jacob.push_back(next);
        if (next >= n)
            break;
    }
    
    /* Build the actual order up to n*/
    size_t prev = 0;
    for (size_t k = 2; k < Jacob.size(); k++)
    {
        size_t jk = Jacob[k];
        if (jk > n)
            jk = n;
        for (size_t t = jk; t > prev; t--)
            out.push_back(t - 1);

        prev = jk;
        if (jk == n)
            break;
    }
}

template <typename Container>
static void insertLoosers(Container &base, const Container &pend, const Container &leaders)
{
    std::vector<size_t> order;
    buildInsertionOrder(pend.size(), order);

    for (size_t k = 0; k < order.size(); k++)
    {
        const size_t idx = order[k];

        int valueToInsert = pend[idx];
        int partnerOfValue = leaders[idx];

        typename Container::iterator leaderIter = std::lower_bound(base.begin(), base.end(), partnerOfValue);
        typename Container::iterator pos = std::lower_bound(base.begin(), leaderIter + 1, valueToInsert);

        base.insert(pos, valueToInsert);
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

    std::vector<int> leaders;
    std::vector<int> losers;
    leaders.reserve(vec.size() / 2);
    losers.reserve(vec.size() / 2);

    for (size_t i = 0; i < vec.size(); i += 2)
    {
        int a = vec[i];
        int b = vec[i + 1];
        if (a < b)
            std::swap(a, b);
        leaders.push_back(a);
        losers.push_back(b);
    }

    std::vector<int> base = leaders;
    if (base.size() > 1)
        pairSortVector(base);

    vec.swap(base);

    insertLoosers(vec, losers, leaders);
    if (odd)
        vec.insert(std::lower_bound(vec.begin(), vec.end(), leftover), leftover);
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

    std::deque<int> leaders;
    std::deque<int> losers;
    leaders.resize(deq.size() / 2);
    losers.resize(deq.size() / 2);

    for (size_t i = 0, p = 0; i < deq.size(); i += 2, p++)
    {
        int a = deq[i];
        int b = deq[i + 1];
        if (a < b)
            std::swap(a, b);
        leaders[p] = a;
        losers[p]  = b;
    }

    std::deque<int> base = leaders;
    if (base.size() > 1)
        pairSortDeque(base);

    deq.swap(base);

    insertLoosers(deq, losers, leaders);
    if (odd)
        deq.insert(std::lower_bound(deq.begin(), deq.end(), leftover), leftover);
}

void PmergeMe::sortDeque(std::deque<int> &deq)
{
    pairSortDeque(deq);
}

void PmergeMe::sortVector(std::vector<int> &vec)
{
    pairSortVector(vec);
}