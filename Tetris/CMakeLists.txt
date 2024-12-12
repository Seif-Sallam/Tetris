cmake_minimum_required(VERSION 3.16)

set(SOURCES
	./src/${PROJECT_NAME}/main.cpp)

add_executable(
	${PROJECT_NAME}
	${SOURCES}
	${HEADERS}
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		./include/
		./include/${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		raylib
		opengl32
		gdi32
		winmm
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
		RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources"
)

# Remove the console window in release mode
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()
