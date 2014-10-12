cmake_minimum_required(VERSION 2.8.11)

add_definitions(-std=c++11)

project(quickshapes)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets	REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${GLUT_INCLUDE_DIRS})

add_library(shapesview 
			SHARED
			ShapefileView.cpp 
			WktShapesModel.cpp
			ShapesViewPlugin.cpp)

target_link_libraries(shapesview Qt5::Widgets)
target_link_libraries(shapesview Qt5::Qml)
target_link_libraries(shapesview Qt5::Quick)
target_link_libraries(shapesview ${GLUT_LIBRARY})
target_link_libraries(shapesview ${OPENGL_LIBRARY})

add_custom_command(TARGET shapesview
	POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying QML files")