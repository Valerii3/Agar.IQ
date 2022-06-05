QT += core
QT -= gui
QT += network
QT += multimedia

TARGET = Agario_server
CONFIG += c++11 console
CONFIG -= app_bundle

TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        answer.cpp \
        bot.cpp \
        bot.cpp \
        entity.cpp \
        food.cpp \
        main.cpp \
        player.cpp \
        question.cpp \
        scene.cpp \
        server.cpp

HEADERS += \
    answer.h \
    bot.h \
    entity.h \
    food.h \
    player.h \
    question.h \
    scene.h \
    server.h \
    json.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
