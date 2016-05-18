#Specify minimum cmake requirements
cmake_minimum_required(VERSION 2.8.12)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0008 NEW)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


project(exampleGuiPlugin)

# Pull in standard configuration.
include(../CytonBuildDefaults.cmake)

set(ENERGID_EXTERNAL_QT_PATH "${qt_moc_path}/../")

# Supply CMake with required Qt CMake files.
set(CMAKE_PREFIX_PATH ${ENERGID_EXTERNAL_QT_PATH})

set(CMAKE_MODULE_PATH CMAKE_PREFIX_PATH)

# The linker does not use library rpaths, so we must tell it where to find the icu libs refrenced by the Qt libs.

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,'${ENERGID_EXTERNAL_QT_PATH}/../icu49/lib'")


# Copy the platform plugins into the bin directory.

if (EXISTS "${ENERGID_EXTERNAL_QT_PATH}/plugins/platforms/")

    file (COPY "${ENERGID_EXTERNAL_QT_PATH}/plugins/platforms" DESTINATION ".")

endif()

# Find includes in corresponding build directories.

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library.
find_package(Qt5Widgets REQUIRED)

# Pull in rendering requirements
include_directories(${render_inc})

link_directories(${render_lib} ${toolkits}/../lib ${toolkits}/../bin)

set(QT_MOC_EXECUTABLE ${qt_moc_path})

set (MOC_HEADERS exampleWidget.h
    cytonExampleGUIPlugin.h
)


# Set the contents of our plugin
add_library(cytonExampleGUIPlugin SHARED
    cytonExampleGUIPlugin.cpp 
    exampleWidget.cpp 
    jointControl.cpp
    ${MOC_HEADERS}
)

# Provide a different naming convention than standard
set_target_properties(cytonExampleGUIPlugin PROPERTIES PREFIX "" SUFFIX ".ecp")


target_link_libraries(
    cytonExampleGUIPlugin
    ecPlugins 
    ecFoundCore 
    ecViewerCore 
    ecCytonBase
    ecHardwarePlugin
    ${qt_lib}
    ecboost_program_options-mt
    ecboost_system-mt
    ecFoundCommon
    ecControl
    ecManipulation
    ecManipulator
    ecSlotManager
    ecXmlReaderWriter
    ecXml
    ecManipulationDirector
    Qt5::Widgets
)

if(UNIX)
target_link_libraries(
    cytonExampleGUIPlugin
    ${toolkits}/../bin/cytonPlugin.ecp
    ${toolkits}/../bin/manipulationActionExecPlugin.ecp
)
endif(UNIX)

if(WIN32)
target_link_libraries(
    cytonExampleGUIPlugin
    ${toolkits}/../lib/cytonPlugin.ecp.lib
    ${toolkits}/../lib/manipulationActionExecPlugin.ecp.lib
)
endif(WIN32)

set(CMAKE_BUILD_TYPE Release)
