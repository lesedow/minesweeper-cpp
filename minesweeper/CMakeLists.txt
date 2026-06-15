# CMakeList.txt : CMake project for minesweeper, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (minesweeper "minesweeper.cpp" "minesweeper.h")

target_sources(minesweeper PRIVATE 
	src/app_state.cpp
	src/board.cpp
	src/cell.cpp
	src/sprites.cpp
)

target_include_directories (minesweeper PRIVATE include)

target_link_libraries (minesweeper PRIVATE SDL3::SDL3)
target_link_libraries (minesweeper PRIVATE SDL3_image::SDL3_image)

set_property (TARGET minesweeper PROPERTY CXX_STANDARD 23)

add_custom_command(TARGET minesweeper POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:SDL3::SDL3>
	$<TARGET_FILE:SDL3_image::SDL3_image>
	$<TARGET_FILE_DIR:minesweeper>
)

add_custom_command(TARGET minesweeper POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/assets
	$<TARGET_FILE_DIR:minesweeper>/assets
)

# TODO: Add tests and install targets if needed.
