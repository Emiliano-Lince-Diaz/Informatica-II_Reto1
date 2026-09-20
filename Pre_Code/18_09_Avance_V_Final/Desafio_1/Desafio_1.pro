TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        interfazUsuario.cpp \
        logicaJuego.cpp \
        main.cpp \
        manejoMemoria.cpp \
        motorDeBits.cpp

HEADERS += \
    interfazUsuario.h \
    logicaJuego.h \
    manejoMemoria.h \
    motorDeBits.h
