QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

LIBS   += -L/usr/lib -lpoppler-qt5

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/pdfarea/pagecontroller/pagecontroller.cpp \
    src/pdfarea/pagecontroller/pageslider.cpp \
    src/pdfarea/pagecontroller/pagespinbox.cpp \
    src/pdfarea/pagecontroller/scalebox.cpp \
    src/pdfarea/pdfview/pdfview.cpp \
    src/pdfarea/pdfview/selectrect.cpp \
    src/sidebar/toc.cpp

HEADERS += \
        include/mainwindow.h \
    include/pdfarea/pagecontroller/pagecontroller.h \
    include/pdfarea/pagecontroller/pageslider.h \
    include/pdfarea/pagecontroller/pagespinbox.h \
    include/pdfarea/pagecontroller/scalebox.h \
    include/pdfarea/pdfview/pdfview.h \
    include/pdfarea/pdfview/selectrect.h \
    include/sidebar/toc.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
