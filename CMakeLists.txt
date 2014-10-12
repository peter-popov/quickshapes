cmake_minimum_required(VERSION 2.8.11)

add_definitions(-std=c++11)

project(quickshapes)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_subdirectory(ShapesView)

find_package(Qt5Widgets	REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

add_executable(qshapeview_test 
			   test/main.cpp)

target_link_libraries(qshapeview_test Qt5::Widgets)
target_link_libraries(qshapeview_test Qt5::Qml)
target_link_libraries(qshapeview_test Qt5::Quick)	
target_link_libraries(qshapeview_test shapesview)

add_custom_command(TARGET qshapeview_test
	POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test/mainwindow.qml ${CMAKE_CURRENT_BINARY_DIR}/mainwindow.qml
    COMMENT "Copying QML files")