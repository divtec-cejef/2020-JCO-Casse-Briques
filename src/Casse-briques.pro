#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T13:38:06
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CasseBrique
TEMPLATE = app

#DEFINES += DEBUG_SPRITE_COUNT
#DEFINES += DEBUG_BBOX
#DEFINES += DEBUG_SHAPE
#DEFINES += DEPLOY # Pour une compilation dans un but de déploiement

SOURCES += main.cpp\
    keytickhandler.cpp \
        mainfrm.cpp \
    gamescene.cpp \
    player.cpp \
    playertickhandler.cpp \
    sprite.cpp \
    gamecore.cpp \
    resources.cpp \
    gameview.cpp \
    gamecanvas.cpp \
    spritetickhandler.cpp \
    bouncingspritehandler.cpp \
    utilities.cpp

HEADERS  += mainfrm.h \
    gamescene.h \
    keytickhandler.h \
    player.h \
    playertickhandler.h \
    sprite.h \
    gamecore.h \
    resources.h \
    gameview.h \
    gamecanvas.h \
    spritetickhandler.h \
    bouncingspritehandler.h \
    utilities.h

FORMS    += mainfrm.ui
