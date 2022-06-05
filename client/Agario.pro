QT       += core gui network sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/bot.cpp \
    src/login.cpp \
    src/answers.cpp \
    src/entity.cpp \
    src/food.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/player.cpp \
    src/question.cpp \
    src/scene.cpp \
    src/settingswindow.cpp \
    src/worker.cpp

HEADERS += \
    include/bot.h \
    include/login.h \
    include/answers.h \
    include/entity.h \
    include/food.h \
    include/mainwindow.h \
    include/player.h \
    include/question.h \
    include/scene.h \
    include/settingswindow.h \
    include/worker.h \
    json.hpp \



FORMS += \
    ui/login.ui \
    ui/mainwindow.ui \
    ui/scene.ui \
    ui/settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




##### OPENSSL #####


INCLUDEPATH += C:/ProjectDB/Agar.IQ/client/openssl

LIBS += C:/ProjectDB/Agar.IQ/client/openssl/libcrypto.lib
LIBS += C:/ProjectDB/Agar.IQ/client/openssl/libssl.lib
