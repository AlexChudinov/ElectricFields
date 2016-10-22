#ifndef APP_DATA_H
#define APP_DATA_H

#include <ansys_mesh_export.h>

struct app_data
{
    using mesh_geom = mesh_geometry<float, uint32_t>;
    mesh_geom* mesh_geometry_;
    app_data():mesh_geometry_(nullptr){}
    ~app_data(){ if(mesh_geometry_) delete mesh_geometry_; }
};

#endif // APP_DATA_H
