project(gelf-getnote)
cmake_minimum_required(VERSION 3.16)

set(SOURCE_ROOT_DIR "${PROJECT_SOURCE_DIR}/../../..")
add_executable(gelf-getnote gelf-getnote.c)
target_link_directories(gelf-getnote PRIVATE
	"${SOURCE_ROOT_DIR}/libelf")
find_library(LIBELF elf PATHS "${SOURCE_ROOT_DIR}/libelf" REQUIRED NO_DEFAULT_PATH)
message(status "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
message(status "SOURCE_ROOT_DIR=${SOURCE_ROOT_DIR}")
message(status "libelf=${LIBELF}")

target_include_directories(gelf-getnote PRIVATE 
	"${SOURCE_ROOT_DIR}/common"
	"${SOURCE_ROOT_DIR}/libelf")
target_link_libraries(gelf-getnote ${LIBELF})
