cmake_minimum_required(VERSION 3.16)

project(data-collector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Sensors Positioning Network)

qt_standard_project_setup()

qt_add_executable(appdata-collector
    main.cpp
)

qt_add_qml_module(appdata-collector
    URI data-collector
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES mobilitydata.h mobilitydata.cpp
    QML_FILES components/DataField.qml
    RESOURCES qml.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appdata-collector PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appdata-collector
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appdata-collector
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Sensors
    PRIVATE Qt6::Positioning
    PRIVATE Qt6::Network
)

include(GNUInstallDirs)
install(TARGETS appdata-collector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
