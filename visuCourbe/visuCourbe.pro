#R. Raffin
#M1 Informatique, Aix-Marseille Université
#.pro de l'application de départ du TP1

QT	+= core gui
CONFIG	+= c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visuCourbe
TEMPLATE = app

LIBS += -lGLU



# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myopenglwidget.cpp \
    point.cpp \
    discretisation.cpp \
    surfacesbezier.cpp \
    surface.cpp \
    Bernstein.cpp

HEADERS += \
        mainwindow.h \
    myopenglwidget.h \
    point.h \
    discretisation.h \
    surfacesbezier.h \
    surface.h \
    Bernstein.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    shaders.qrc

