TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        arraysum.cpp \
        main.cpp \
        matmul.cpp

HEADERS += \
    arraysum.h \
    matmul.h

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
