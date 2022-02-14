cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME cha_engine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(WINDOW_NAME )
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

if (APPLE)
	set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
endif()

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} src/main.cpp src/common.cpp src/Shader.cpp src/Shader.h src/Program.cpp src/Program.h src/Context.cpp src/Context.h src/Vertex_layout.cpp src/Vertex_layout.h src/Buffer.cpp src/Buffer.h src/Image.cpp src/Image.h src/Texture.cpp src/Texture.h)

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
