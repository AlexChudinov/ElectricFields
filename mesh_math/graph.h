#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <algorithm>
#include <utility>
#include <stack>

/**
 * Unordered graph to keep mesh connectivity information
 */

namespace data_structs {

using std::vector;

template<typename label>
class graph
{
    using label_vector_type = std::vector<label>;
    using connectivity_type = std::vector<label_vector_type>;
    using connection_type   = std::pair<label, label>;
    using compacted_graph   = std::pair<graph, label_vector_type>;

    connectivity_type connectivity_;

public:

    /**
     * Creates an empty graph
     */
    graph(){}

    /**
     * Resets the connectivity array
     */
    void clear()
    {
        connectivity_.clear();
    }

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
            typename label_vector_type::const_iterator
                    first_max = connectivity_[max].cbegin(),
                    last_max  = connectivity_[max].cend(),
                    first_min = connectivity_[min].cbegin(),
                    last_min  = connectivity_[min].cend(),
                    cit = std::lower_bound(first_max, last_max, min);
            if(cit == last_max || *cit != min)
            {
                connectivity_[max].insert(cit, min);
                connectivity_[min].insert(std::lower_bound(first_min, last_min, max), max);
            }
        }
    }

    /**
     * Get neighbour of the node
     */
    inline const label_vector_type& get_node_neighbour(label node_label) const
    { return connectivity_[node_label]; }

    /**
     * Get number of nodes
     */
    inline size_t size() const { return connectivity_.size(); }

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

    /**
     * Graph deep search itterative
     */
    template<typename Observer>
    void dfs_iterative(Observer observer) const
    {
        std::vector<bool> visited(this->size(), false);
        std::stack<label> dfs_stack;
        std::vector<bool>::const_iterator
                first = visited.cbegin(),
                last  = visited.cend(),
                current = first;

        while( (current = std::find(current, last, false)) != last)
        {
            dfs_stack.push(label(std::distance(first, current)));
            while(!dfs_stack.empty())
            {
                label current_label = dfs_stack.top(); dfs_stack.pop();
                if(!visited[current_label])
                {
                    observer(current_label);
                    visited[current_label] = true;
                    for(label l : this->get_node_neighbour(current_label)) dfs_stack.push(l);
                }
            }
        }
    }

    /**
     * Removes nodes with empty connections, compacting initial graph and storing mapping to an initial graph nodes
     * into a vector of labels. So the old label can be accessed using new labels and this vector
     */
    compacted_graph remove_empty_connections() const
    {
        compacted_graph result;
        result.second.reserve(connectivity_.size());

        auto observer = [&result, this](label node_label1, label node_label2)
        {
            label new_label1, new_label2;
            typename label_vector_type::const_iterator
                    first = result.second.cbegin(),
                    last  = result.second.cend(),
                    current = std::lower_bound(first, last, node_label1);

            if( *current != node_label1 )
            {
                new_label1 = result.second.size();
                result.second.insert(current, node_label1);
            }
            else new_label1 = std::distance(first, current);

            current = std::lower_bound(current, last, node_label2);
            if( *current != node_label2 )
            {
                new_label2 = result.second.size();
                result.second.insert(current, node_label2);
            }
            else new_label2 = std::distance(first, current);

            result.first.add_connection(new_label1, new_label2);
        };

        this->iterate_over_unique_connections(observer);

        return result;
    }
};

}

#endif // GRAPH_H
