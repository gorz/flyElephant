TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    Controller.cpp \
    Task.cpp \
    TaskListModel.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    Controller.hpp \
    Task.hpp \
    TaskListModel.hpp

