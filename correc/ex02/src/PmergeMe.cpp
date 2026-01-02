#include "PmergeMe.hpp"

std::vector<int> makePending(const std::vector<std::pair<int, int> >& pairs)
{
    std::vector<int> pending;

    pending.reserve(pairs.size());
    for (std::size_t i = 0; i < pairs.size(); ++i)
        pending.push_back(pairs[i].first);
    return pending;
}

std::vector<std::size_t> computeJacobsthal(std::size_t n)
{
    std::vector<std::size_t> jacob;

    jacob.push_back(0);
    jacob.push_back(1);
    while (jacob.back() < n)
    {
        std::size_t sz = jacob.size();
        jacob.push_back(jacob[sz - 1] + 2 * jacob[sz - 2]);
    }
    return (jacob);
}
