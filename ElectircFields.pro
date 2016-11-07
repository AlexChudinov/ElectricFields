QT += core gui widgets

CONFIG += c++14

TARGET = ElectircFields
TEMPLATE = app

SOURCES += main.cpp \
    main_window.cpp

HEADERS += main_window.h

include(display_mesh/display_mesh.pri)
include(mesh_math/mesh_math.pri)
include(linear_algebra/linear_algebra.pri)
include(data_export/data_export.pri)
include(application_data/application_data.pri)

RESOURCES += resources.qrc
