QT += core gui sql multimedia multimediawidgets network charts
#QT += texttospeech
QT += quickcontrols2 uitools

QT += widgets texttospeech
 requires(qtHaveModule(gui))
 qtHaveModule(gui): requires(qtConfig(combobox))

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET += Pharmacy_2

SOURCES += \
    main.cpp \
    home.cpp \
    my_database.cpp \
    part_1.cpp \
    stock.cpp

HEADERS += \
    home.h \
    my_database.h \
    part_1.h \
    stock.h

FORMS += \
    home.ui \
    part_1.ui \
    stock.ui

#QMAKE_CXXFLAGS += -std=gnu++14

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    StyleSheet.css \
    filedialog.qml
