QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = safe-game
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    main_window.cpp \
    safe_widget.cpp \
    switch_widget.cpp \
    hall_of_fame_dialog.cpp \
    utils.cpp

HEADERS += \
    main_window.h \
    safe_widget.h \
    switch_widget.h \
    orientation.h \
    hall_of_fame_dialog.h \
    utils.h

RESOURCES += \
    resources/resources.qrc
