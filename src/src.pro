TEMPLATE = app
TARGET = cookie-test

CONFIG += \
    qt

QT += \
    gui \
    qml \
    quick

SOURCES += \
    main.cpp

QML_SOURCES = \
    qml/cookie-test.qml \
    qml/WebPage.qml

RESOURCES += \
    ui.qrc

OTHER_FILES += \
    $${QML_SOURCES} \
    $${RESOURCES}

target.path = /usr/bin
INSTALLS += target
