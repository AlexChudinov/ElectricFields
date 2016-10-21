QT += core
QT -= gui

CONFIG += c++11

TARGET = ElectircFields
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ansys_mesh_export.cpp

INCLUDEPATH += C:\Proj_22022016\BarnesHut

HEADERS += \
    graph.h \
    ansys_mesh_export.h \
    mesh_geometry.h
