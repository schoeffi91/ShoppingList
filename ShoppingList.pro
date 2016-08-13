QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    src/shoppinglistmodel.cpp

RESOURCES += ressources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=

HEADERS += \
    src/shoppinglistmodel.h
