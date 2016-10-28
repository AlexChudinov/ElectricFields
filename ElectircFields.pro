QT += core gui widgets

CONFIG += c++14

TARGET = ElectircFields
TEMPLATE = app

SOURCES += main.cpp \
    main_window.cpp

HEADERS += main_window.h \
    app_data.h

include(display_mesh/display_mesh.pri)
include(mesh_math/mesh_math.pri)
include(linear_algebra/linear_algebra.pri)

RESOURCES += resources.qrc
