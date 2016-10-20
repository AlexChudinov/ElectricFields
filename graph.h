#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

/**
 * Graph to keep mesh connectivity information
 */

namespace data_structs {

using std::vector;

template<typename label>
struct graph
{
    using mesh_connectivity_type = vector<vector<label>>;
    mesh_connectivity_type mesh_connectivity;

    graph(){}

    void estable_connection(label i, label j)
    {

    }
};

}

#endif // GRAPH_H
