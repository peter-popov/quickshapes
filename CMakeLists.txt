cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

add_definitions(-std=c++1y)

project(qshapeview)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Qml REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${GLUT_INCLUDE_DIRS})

add_executable(qshapeview 
			   main.cpp 
			   ShapefileView.cpp 
			   WktShapesModel.cpp)

target_link_libraries(qshapeview Qt5::Widgets)
target_link_libraries(qshapeview Qt5::Qml)
target_link_libraries(qshapeview Qt5::Quick)
target_link_libraries(qshapeview ${GLUT_LIBRARY})
target_link_libraries(qshapeview ${OPENGL_LIBRARY})

#file(COPY mainwindow.qml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


add_custom_command(TARGET qshapeview
	POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.qml ${CMAKE_CURRENT_BINARY_DIR}/mainwindow.qml
    COMMENT "Copying QML files")