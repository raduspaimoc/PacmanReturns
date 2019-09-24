find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable(cmake_hello main.cpp graphics.cpp map.cpp cell.cpp Utils.cpp )
target_link_libraries(cmake_hello ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
