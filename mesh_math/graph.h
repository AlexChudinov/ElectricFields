#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <algorithm>
#include <utility>

/**
 * Unordered graph to keep mesh connectivity information
 */

namespace data_structs {

using std::vector;

template<typename label>
struct graph
{
    using label_vector_type = std::vector<label>;
    using connectivity_type = std::vector<label_vector_type>;
    using connection_type   = std::pair<label, label>;

    connectivity_type connectivity_;

public:

    /**
     * Creates empty graph
     */
    graph(){}

    /**
     * Adds new connection into the unordered graph,
     * Note: node labels go in ascending order
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

    /**
     * Returns a total number of connections
     */
    size_t connections_number() const
    {
        size_t connestions = 0;
        for(const label_vector_type& lv : connectivity_)
            connestions += lv.size();
        return connestions/2;
    }

    /**
     * Iterates over unique connections passing corresponding labels into the observer functor
     */
    template<typename Observer>
    void iterate_over_unique_connections(Observer observer) const
    {
        using label_vec_iter = typename label_vector_type::const_iterator;
        for(size_t i = 0; i != connectivity_.size(); ++i)
        {
            label_vec_iter first = connectivity_[i].cbegin();
            label_vec_iter last = connectivity_[i].cend();
            first = std::lower_bound(first, last, i);
            for(; first != last; ++first) observer(i, *first);
        }
    }
};

}

#endif // GRAPH_H
