# 1. Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# 2. Define the project name and language
project(CGroupChat VERSION 1.0 LANGUAGES C)

# 3. Specify the C standard to use (C11 is a safe modern choice)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 4. Include directories (where your .h files are located)
# If your headers are in the same folder, "." is fine.
include_directories(.)

# 5. List all your source files (.c files)
# This tells CMake which files need to be compiled together
set(SOURCES
        main.c
        authentication.c
        group_interaction.c
        messaging.c
        registration.c
        file_io.c
)

# 6. Define the executable name and its source files
add_executable(group_chat ${SOURCES})

# 7. (Optional) Add compiler flags for better debugging
if(MSVC)
    target_compile_options(group_chat PRIVATE /W4)
else()
    target_compile_options(group_chat PRIVATE -Wall -Wextra -g)
endif()