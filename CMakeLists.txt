cmake_minimum_required(VERSION 3.16.3)  # CMake version check
project(Backerup)               		# Create project "Backerup"
set(CMAKE_CXX_STANDARD 17)            	# Enable c++17 standard

set(includes includes/inputmanager.h includes/vaultmanager.h includes/configmanager.h includes/backfile.h)
set(sources Backerup.cpp InputManager.cpp VaultManager.cpp ConfigManager.cpp backFile.cpp)
add_library(tinyxml2 STATIC dependencies/tinyxml2/tinyxml2.cpp dependencies/tinyxml2/tinyxml2.h)
include_directories(includes)

add_executable(Backerup ${sources} ${includes})
target_link_libraries(Backerup tinyxml2)