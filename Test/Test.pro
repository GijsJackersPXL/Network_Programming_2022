TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/../lib -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../include

SOURCES += \
        main.cpp
        grasslands.cpp
        ironhouse.cpp
        ironhouse2.cpp
        simple_client.cpp
        simple_server.cpp
        stonehouse.cpp
        strawhouse.cpp
        woodhouse.cpp

