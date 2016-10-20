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
    using mesh_connectivity_type = std::vector<std::vector<label>>;
    mesh_connectivity_type mesh_connectivity_;

    graph(){}

    void add_connection(label i, label j)
    {
        label max = std::max(i, j);
        label min = std::min(i, j);
        mesh_connectivity_type::const_iterator cit1 = mesh_connectivity_.begin();
        mesh_connectivity_type::const_iterator cit2 = mesh_connectivity_.end();

        if(max >= mesh_connectivity_.size())
        {
            mesh_connectivity_.resize(max + 1);
            mesh_connectivity_[max].push_back(min);
            mesh_connectivity_[min].push_back(max);
        }
        else
        {
            std::vector<label>::const_iterator cit1 = mesh_connectivity_[max].begin();
            std::vector<label>::const_iterator cit2 = mesh_connectivity_[max].end();
            std::vector<label>::const_iterator cit;
            if((cit = std::lower_bound(cit1, cit2, min)) != cit2)
            {
                if(*cit != min) mesh_connectivity_[max].insert(cit, min);
            }
            else
            {
                mesh_connectivity_[max].push_back(min);
            }

            cit1 = mesh_connectivity_[min].begin();
            cit2 = mesh_connectivity_[min].end();
            if((cit = std::lower_bound(cit1, cit2, max)) != cit2)
            {
                if(*cit != max) mesh_connectivity_[min].insert(cit, max);
            }
            else
            {
                mesh_connectivity_[min].push_back(max);
            }
        }
    }
};

}

#endif // GRAPH_H
