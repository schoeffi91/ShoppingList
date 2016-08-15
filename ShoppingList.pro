QT += qml quick sql

CONFIG += c++11

SOURCES += main.cpp \
    src/shoppinglistmodel.cpp

RESOURCES += ressources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

HEADERS += \
    src/shoppinglistmodel.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
