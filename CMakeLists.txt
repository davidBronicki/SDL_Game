cmake_minimum_required(VERSION 3.0)

project(First_Game CXX)


file(GLOB Game_Code_Pile
	"*/*.cpp")
add_executable(game ${Game_Code_Pile})



INCLUDE(FindPkgConfig)

find_package(SDL2 REQUIRED)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})

set_property(TARGET game PROPERTY CXX_STANDARD 11)
target_link_libraries(game
	${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})


