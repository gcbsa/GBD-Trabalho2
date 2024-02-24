cmake_minimum_required(VERSION 3.20)
project(MyProject
  LANGUAGES C
)
set(EXECUTABLE_NAME ${PROJECT_NAME})

#### ADD EXECUTABLE ####

file(GLOB SRCS CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/*.c")
add_executable(${EXECUTABLE_NAME} ${SRCS})
