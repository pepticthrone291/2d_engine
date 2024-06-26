cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME 2d_engine)
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "2D Engine")
set(WINDOW_WIDTH 800)
set(WINDOW_HEIGHT 600)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/common.cpp src/common.h
  src/shader.cpp src/shader.h
  src/program.cpp src/program.h
  src/buffer.cpp src/buffer.h
  src/vertex_layout.cpp src/vertex_layout.h
  src/context.cpp src/context.h
)

include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
  WINDOW_NAME="${WINDOW_NAME}"
  WINDOW_WIDTH=${WINDOW_WIDTH}
  WINDOW_HEIGHT=${WINDOW_HEIGHT}
  )

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})