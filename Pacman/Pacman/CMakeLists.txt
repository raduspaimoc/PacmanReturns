# set(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS} -std=c++11 -lGLU -lGL -lm") # compiler version and compiler flags.
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable(cmake_hello main.cpp graphics.cpp map.cpp cell.cpp Utils.cpp )
target_link_libraries(cmake_hello ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )