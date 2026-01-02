

#include "PmergeMe.hpp"
#include <algorithm>
#include <vector>
#include <deque>

static void jacobsthalOrder(size_t n, std::vector<size_t>& out)
{
    if (!n) return;
    std::vector<size_t> j;
    for (size_t a = 1, b = 1; b - 1 < n;)
    {
        size_t k = b - 1;
        if (k && k < n)
            j.push_back(k);
        size_t c = b + 2 * a;
        a = b;
        b = c;
    }
    size_t prev = 0;
    for (size_t t = 0; t < j.size(); t++)
    {
        for (size_t i = prev; i < j[t]; ++i)
            out.push_back(i);
        out.push_back(j[t]);
        prev = j[t] + 1;
    }
    for (size_t i = prev; i < n; ++i)
        out.push_back(i);
}

void PmergeMe::insertPendVector(std::vector<int>& dst, const std::vector<int>& pend)
{
    std::vector<size_t> idx;
    jacobsthalOrder(pend.size(), idx);
    for (size_t k = 0; k < idx.size(); ++k)
    {
        size_t i = idx[k];
        if (i < pend.size())
            dst.insert(std::lower_bound(dst.begin(), dst.end(), pend[i]), pend[i]);
    }
}

void PmergeMe::pairSortVector(std::vector<int>& vec)
{
    std::vector<int> leaders;
    std::vector<int> followers;
    leaders.reserve((vec.size() + 1) / 2);
    followers.reserve(vec.size() / 2);
    bool odd = vec.size() & 1;
    int leftover = odd ? vec.back() : 0;

    for (size_t i = 0; i + 1 < vec.size(); i += 2)
    {
        int a = vec[i], b = vec[i + 1];
        if (a < b)
            std::swap(a, b);
        leaders.push_back(a);
        followers.push_back(b);
    }
    if (leaders.size() > 1)
        pairSortVector(leaders);

    vec = leaders;
    insertPendVector(vec, followers);
    if (odd)
        vec.insert(std::lower_bound(vec.begin(), vec.end(), leftover), leftover);
}

void PmergeMe::sortVector(std::vector<int>& vec)
{
    if (vec.size() < 2)
        return;
    pairSortVector(vec);
}

void PmergeMe::insertPendDeque(std::deque<int>& dst, const std::deque<int>& pend)
{
    std::vector<size_t> idx;
    jacobsthalOrder(pend.size(), idx);
    for (size_t k = 0; k < idx.size(); ++k)
    {
        size_t i = idx[k];
        if (i < pend.size())
            dst.insert(std::lower_bound(dst.begin(), dst.end(), pend[i]), pend[i]);
    }
}

void PmergeMe::pairSortDeque(std::deque<int>& deq)
{
    std::deque<int> leaders;
    std::deque<int> followers;
    bool odd = deq.size() & 1;
    int leftover = odd ? deq.back() : 0;

    for (size_t i = 0; i + 1 < deq.size(); i += 2)
    {
        int a = deq[i];
        int b = deq[i + 1];
        if (a < b)
            std::swap(a, b);
        leaders.push_back(a);
        followers.push_back(b);
    }
    if (leaders.size() > 1)
        pairSortDeque(leaders);

    deq = leaders;
    insertPendDeque(deq, followers);
    if (odd)
        deq.insert(std::lower_bound(deq.begin(), deq.end(), leftover), leftover);
}

void PmergeMe::sortDeque(std::deque<int>& deq)
{
    if (deq.size() < 2)
        return;
    pairSortDeque(deq);
}