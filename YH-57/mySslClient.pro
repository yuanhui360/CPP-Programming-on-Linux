TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        ssl_client.cpp

HEADERS += \
    ssl_client.h

LIBS += -lboost_iostreams -lpthread -lboost_system -lcrypto -lssl
