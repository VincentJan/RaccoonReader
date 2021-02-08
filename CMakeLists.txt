cmake_minimum_required(VERSION 3.17)
project(RaccoonReader)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)

include_directories(include)
include_directories(include/pdfarea/pagecontroller)
include_directories(include/pdfarea/pdfview)
include_directories(include/sidebar)

set(INCLUDES
        include/mainwindow.h
        include/pdfarea/pagecontroller/pagecontroller.h
        include/pdfarea/pagecontroller/pageslider.h
        include/pdfarea/pagecontroller/pagespinbox.h
        include/pdfarea/pagecontroller/scalebox.h
        include/pdfarea/pdfview/highlightarea.h
        include/pdfarea/pdfview/marktable.h
        include/pdfarea/pdfview/pdfmarkstate.h
        include/pdfarea/pdfview/pdfstate.h
        include/pdfarea/pdfview/pdfview.h
        include/pdfarea/pdfview/pdfviewstate.h
        include/pdfarea/pdfview/pdfzoomstate.h
        include/sidebar/toc.h
        )

set (SOURCES
        src/pdfarea/pagecontroller/pagecontroller.cpp
        src/pdfarea/pagecontroller/pageslider.cpp
        src/pdfarea/pagecontroller/pagespinbox.cpp
        src/pdfarea/pagecontroller/scalebox.cpp
        src/pdfarea/pdfview/highlightarea.cpp
        src/pdfarea/pdfview/pdfmarkstate.cpp
        src/pdfarea/pdfview/pdfview.cpp
        src/pdfarea/pdfview/pdfviewstate.cpp
        src/pdfarea/pdfview/pdfzoomstate.cpp
        src/sidebar/toc.cpp
        src/mainwindow.cpp
        src/main.cpp
        )

add_executable(${PROJECT_NAME} ${INCLUDES} ${SOURCES} Resource.qrc)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} poppler-qt5)
