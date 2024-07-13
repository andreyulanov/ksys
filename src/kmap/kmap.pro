linux-buildroot-g++: QT += core gui widgets
else:QT += core gui widgets positioning sensors network
QT += qml quick location
CONFIG += c++2a
QMAKE_CXXFLAGS += -Wno-deprecated-enum-enum-conversion

QT += qml quick sql

!linux-buildroot-g++: DEFINES += BUILD_WITH_SENSORS
!android: DEFINES += BUILD_WITH_XMPP

SOURCES += \
 ../lib/kbase.cpp \
 ../lib/kclass.cpp \
 ../lib/kclassmanager.cpp \
 ../lib/kdatetime.cpp \
 ../lib/kobject.cpp \
 ../lib/kfreeobject.cpp \
../lib/klocker.cpp \
 ../lib/kpack.cpp \
../lib/krender.cpp \
 ../lib/krenderpack.cpp \
 kautoscroll.cpp \
 kcontrols.cpp \
 keditwidget.cpp \
 knewobjectwidget.cpp \
 kpackfetcher.cpp \
 kposgenerator.cpp \
 krenderwidget.cpp \
 kscalelabel.cpp \
 ksettings.cpp \
    kstoragemanager.cpp \
 ktrackmanager.cpp \
main.cpp

HEADERS += \
 ../lib/kbase.h \
 ../lib/kclass.h \
 ../lib/kclassmanager.h \
 ../lib/kdatetime.h \
 ../lib/kfreeobject.h \
../lib/klocker.h \
 ../lib/kobject.h \
 ../lib/kpack.h \
../lib/krender.h \
 ../lib/krenderpack.h \
 kautoscroll.h \
 kcontrols.h \
 keditwidget.h \
 knewobjectwidget.h \
 kpackfetcher.h \
 kposgenerator.h \
 krenderwidget.h \
 kscalelabel.h \
 ksettings.h \
    kstoragemanager.h \
 ktrackmanager.h

!android: SOURCES += \
kportableobjectsender.cpp \
krosterwidget.cpp \
kxmppclient.cpp \
kmucmanager.cpp \
kmucroombackend.cpp

!android: HEADERS += \
kportableobjectsender.h \
krosterwidget.h \
kxmppclient.h \
kmucmanager.h \
kmucroombackend.h

!linux-buildroot-g++: SOURCES += \
    kheading.cpp \
    kpositionlabel.cpp \

!linux-buildroot-g++: HEADERS += \
    kheading.h \
    kpositionlabel.h \

INCLUDEPATH += ../lib

# find QXmpp lib
exists( /usr/lib/libQXmppQt5.so ) {
      message( "Found QXmpp lib: /usr/lib/libQXmppQt5.so" )
      LIBS += /usr/lib/libQXmppQt5.so
} else {
    message( "There is no file /usr/lib/libQXmppQt5.so" )
    message( "I'll link with -lqxmpp arg" )
    unix|win32: LIBS += -lqxmpp
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
 KMuc.qml \
 android/AndroidManifest.xml \
 android/build.gradle \
 android/gradle.properties \
 android/gradle/wrapper/gradle-wrapper.jar \
 android/gradle/wrapper/gradle-wrapper.properties \
 android/gradlew \
 android/gradlew.bat \
 android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES += \
 images.qrc \
 qml.qrc

CONFIG += qmltypes
QML_IMPORT_NAME = ksys.kmap.backend
QML_IMPORT_MAJOR_VERSION = 1
