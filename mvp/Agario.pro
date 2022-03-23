QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    answers.cpp \
    entity.cpp \
    food.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    question.cpp \
    scene.cpp \
    settingswindow.cpp \
    worker.cpp

HEADERS += \
    answers.h \
    entity.h \
    food.h \
    mainwindow.h \
    player.h \
    question.h \
    scene.h \
    settingswindow.h \
    worker.h

FORMS += \
    mainwindow.ui \
    scene.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
