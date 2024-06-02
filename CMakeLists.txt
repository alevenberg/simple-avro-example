cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 14)

# Define the project name and where to report bugs.
set(PACKAGE_BUGREPORT
    "https://github.com/alevenberg/simple-avro-example")
project(simple-avro-example CXX)

find_package(unofficial-avro-cpp CONFIG REQUIRED)

# Generate the avro C++ files using the avro compiler.
find_program(AVROGENCPP NAMES avrogencpp)
macro (run_avro_compiler file namespace)
    add_custom_command(
        OUTPUT ${file}.h
        COMMAND
            ${AVROGENCPP} ARGS --input
            "${CMAKE_CURRENT_SOURCE_DIR}/${file}.avro" --output ${file}.h
            --namespace ${namespace}
        DEPENDS ${AVROGENCPP} "${CMAKE_CURRENT_SOURCE_DIR}/${file}.avro"
        COMMENT "Executing Avro compiler")
    set_source_files_properties(${file}.h PROPERTIES GENERATED TRUE)
endmacro (run_avro_compiler)

run_avro_compiler(schema v1)

add_executable(quickstart quickstart.cc schema.h)
target_include_directories(quickstart PRIVATE SYSTEM
                                              "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(quickstart PRIVATE unofficial::avro-cpp::avrocpp)

add_executable(generic generic.cc schema.h)
target_include_directories(generic PRIVATE SYSTEM
                                              "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(generic PRIVATE unofficial::avro-cpp::avrocpp)

add_executable(generic_multiple generic_multiple.cc schema.h)
target_include_directories(generic_multiple PRIVATE SYSTEM
                                              "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(generic_multiple PRIVATE unofficial::avro-cpp::avrocpp)

add_executable(generic_test generic_test.cc schema.h)
target_include_directories(generic_test PRIVATE SYSTEM
                                              "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(generic_test PRIVATE unofficial::avro-cpp::avrocpp)