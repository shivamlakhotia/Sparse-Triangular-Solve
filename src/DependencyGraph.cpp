#include "DependencyGraph.h"

DependencyGraph::DependencyGraph()
{
    this->graph = map<int, unordered_set<int>>();
}

void DependencyGraph::insertEdge(int i, int j)
{
    map<int, unordered_set<int>>::iterator it;
    it = this->graph.find(j);
    if (it != this->graph.end())
    {
        (it->second).insert(i);
    }
    else
    {
        unordered_set<int> dependencySet = unordered_set<int>();
        dependencySet.insert(i);
        this->graph.insert(pair<int, unordered_set<int>>(j, dependencySet));
    }
    return;
}

void DependencyGraph::traverseDG()
{
    cout << "Dependency Graph:\n";
    map<int, unordered_set<int>>::iterator it;
    for (it = this->graph.begin(); it != this->graph.end(); it++)
    {
        // cout << it->first << "\n";
        unordered_set<int>::iterator it2;

        for (it2 = (it->second).begin(); it2 != (it->second).end(); it2++)
        {
            cout << it->first << "-->" << *it2 << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void DependencyGraph::findReachable(unordered_set<int> seed, unordered_set<int> &visited)
{
    cout << "Finding reachability set now\n";
    int head = -1;
    unordered_set<int>::iterator seedIt;
    while (!seed.empty())
    {
        seedIt = seed.begin();
        head = *seedIt;
        seed.erase(seedIt);

        // cout << head << " is reachable\n";
        visited.insert(head);

        map<int, unordered_set<int>>::iterator it;
        unordered_set<int>::iterator it2;

        it = (this->graph).find(head);

        if (it != (this->graph).end())
        {
            for (it2 = (it->second).begin(); it2 != (it->second).end(); it2++)
            {
                if (visited.find(*it2) == visited.end())
                {
                    seed.insert(*it2);
                }
            }
        }
    }

    cout << "Reachable set size: " << visited.size() << "\n";
    // unordered_set<int>::iterator it;
    // for (it = visited.begin(); it != visited.end(); it++)
    //     cout << *it << " ";
    cout << "\n\n";
}