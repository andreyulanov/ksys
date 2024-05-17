linux-buildroot-g++: QT += core gui widgets
else:QT += core gui widgets positioning sensors network
CONFIG += c++2a
QMAKE_CXXFLAGS += -Wno-deprecated-enum-enum-conversion

!linux-buildroot-g++: DEFINES += BUILD_WITH_SENSORS
!android: DEFINES += BUILD_WITH_XMPP

SOURCES += \
 ../lib/kdatetime.cpp \
 ../lib/kfreeobject.cpp \
../lib/klocker.cpp \
 ../lib/kpack.cpp \
../lib/krender.cpp \
../lib/kshape.cpp \
 kautoscroll.cpp \
 kcontrols.cpp \
 keditwidget.cpp \
    kmucmanager.cpp \
    kmucroombackend.cpp \
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
 ../lib/kdatetime.h \
 ../lib/kfreeobject.h \
../lib/klocker.h \
 ../lib/kpack.h \
../lib/krender.h \
../lib/kshape.h \
 kautoscroll.h \
 kcontrols.h \
 keditwidget.h \
    kmucmanager.h \
    kmucroombackend.h \
 knewobjectwidget.h \
 kpackfetcher.h \
 kposgenerator.h \
 krenderwidget.h \
 kscalelabel.h \
 ksettings.h \
    kstoragemanager.h \
 ktrackmanager.h

!android: SOURCES += \
kloginwidget.cpp \
kportableobjectsender.cpp \
krosterwidget.cpp \
kxmppclient.cpp

!android: HEADERS += \
kloginwidget.h \
kportableobjectsender.h \
krosterwidget.h \
kxmppclient.h

!linux-buildroot-g++: SOURCES += \
    kheading.cpp \
    kpositionlabel.cpp \

!linux-buildroot-g++: HEADERS += \
    kheading.h \
    kpositionlabel.h \

INCLUDEPATH += ../lib

!android: LIBS += -lQXmppQt5

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
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
 images.qrc
