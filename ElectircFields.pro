QT += core gui widgets

CONFIG += c++14

TARGET = ElectircFields
TEMPLATE = app

SOURCES += main.cpp \
    main_window.cpp

INCLUDEPATH += C:\Proj_22022016\BarnesHut #Path to matrix and vector headers

HEADERS += main_window.h \
    app_data.h

include(display_mesh/display_mesh.pri)
include(mesh_math/mesh_math.pri)

RESOURCES += resources.qrc
