QT += core gui widgets

CONFIG += c++14

TARGET = ElectircFields
TEMPLATE = app

SOURCES += main.cpp \
    ansys_mesh_export.cpp \
    main_window.cpp \
    show_mesh_data.cpp

INCLUDEPATH += C:\Proj_22022016\BarnesHut

HEADERS += \
    graph.h \
    ansys_mesh_export.h \
    mesh_geometry.h \
    main_window.h \
    app_data.h \
    show_mesh_data.h

RESOURCES += resources.qrc

DISTFILES +=
