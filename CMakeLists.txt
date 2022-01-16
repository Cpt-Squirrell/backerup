cmake_minimum_required(VERSION 3.22.1)  # CMake version check
project(Backerup)               		# Create project "Backerup"
set(CMAKE_CXX_STANDARD 17)            	# Enable c++17 standard

set(includes includes/inputmanager.h includes/vaultmanager.h includes/configmanager.h)
set(sources Backerup.cpp InputManager.cpp VaultManager.cpp ConfigManager.cpp)
add_library(tinyxml2 STATIC dependencies/tinyxml2/tinyxml2.cpp dependencies/tinyxml2/tinyxml2.h)

add_executable(Backerup ${sources} ${includes})
target_link_libraries(Backerup tinyxml2)