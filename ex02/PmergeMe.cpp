#include "PmergeMe.hpp"
#include <algorithm>

void PmergeMe::insertPendVector(std::vector<int>& dst, const std::vector<int>& pend)
{
    for (size_t i = 0; i < pend.size(); ++i)
        dst.insert(std::lower_bound(dst.begin(), dst.end(), pend[i]), pend[i]);
}

void PmergeMe::pairSortVector(std::vector<int>& vec)
{
    std::vector<int> leaders;
    std::vector<int> followers;
    leaders.reserve((vec.size() + 1) / 2);
    followers.reserve(vec.size() / 2);
    bool odd = vec.size() & 1;
    int  leftover = odd ? vec.back() : 0;

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
    for (size_t i = 0; i < pend.size(); i++)
        dst.insert(std::lower_bound(dst.begin(), dst.end(), pend[i]), pend[i]);
}

void PmergeMe::pairSortDeque(std::deque<int>& deq)
{
    std::deque<int> leaders;
    std::deque<int> followers;
    bool odd = deq.size() & 1;
    int  leftover = odd ? deq.back() : 0;

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
