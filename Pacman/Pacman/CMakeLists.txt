find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

file(GLOB pacman_SRC "*.h" "*.cpp")

add_executable(cmake_hello ${pacman_SRC})
target_link_libraries(cmake_hello ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
