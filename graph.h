#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <algorithm>

/**
 * Graph to keep mesh connectivity information
 */

namespace data_structs {

using std::vector;

template<typename label>
struct graph
{
    using label_vector_type = std::vector<label>;
    using connectivity_type = std::vector<label_vector_type>;

    connectivity_type connectivity_;

public:

    /**
     * Creates empty graph
     */
    graph(){}

    /**
     * Adds new connection into graph, node labels go in ascending order
     */
    void add_connection(label i, label j)
    {
        label max = std::max(i, j);
        label min = std::min(i, j);

        if(max >= connectivity_.size())
        {
            connectivity_.resize(max + 1);
            connectivity_[max].push_back(min);
            connectivity_[min].push_back(max);
        }
        else
        {
            typename label_vector_type::const_iterator cit1 = connectivity_[max].cbegin();
            typename label_vector_type::const_iterator cit2 = connectivity_[max].cend();
            typename label_vector_type::const_iterator cit;
            if((cit = std::lower_bound(cit1, cit2, min)) != cit2)
            {
                if(*cit != min)
                {
                    connectivity_[max].insert(cit, min);
                    cit1 = connectivity_[min].cbegin();
                    cit2 = connectivity_[min].cend();
                    connectivity_[min].insert(std::lower_bound(cit1, cit2, max), max);
                }
            }
            else
            {
                connectivity_[max].push_back(min);
                cit1 = connectivity_[min].cbegin();
                cit2 = connectivity_[min].cend();
                connectivity_[min].insert(std::lower_bound(cit1, cit2, max), max);
            }
        }
    }

    /**
     * Get neighbour of the node
     */
    const label_vector_type& get_node_neighbour(label node_label) const
    {
        return connectivity_[node_label];
    }

    /**
     * Get number of nodes
     */
    size_t size() const { return connectivity_.size(); }
};

}

#endif // GRAPH_H
