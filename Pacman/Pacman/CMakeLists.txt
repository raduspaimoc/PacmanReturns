find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD_LIBRARIES -ljpeg)
file(GLOB pacman_SRC "*.h" "*.cpp")


add_executable(cmake_hello ${pacman_SRC})
# set_property(TARGET cmake_hello PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
target_link_libraries(cmake_hello ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
