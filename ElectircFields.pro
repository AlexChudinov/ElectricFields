QT += core
QT -= gui

CONFIG += c++11

TARGET = ElectircFields
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += C:\Users\Luba\Desktop\BarnesHut

HEADERS += \
    quad_3d_fun.h \
    graph.h \
    ansys_mesh_load.h
