cmake_minimum_required(VERSION 3.8)
project(Lox)

set(CMAKE_CXX_STANDARD 20)

#add_subdirectory(src)
add_executable(lox)
#target_link_libraries(lox quickjs)

##
##  List all C/C++ Source files in the directory passed in
##
function(get_files_list source_dir output_variable)
    file(GLOB_RECURSE SRC_H     ${source_dir}/*.h)
    file(GLOB_RECURSE SRC_HPP   ${source_dir}/*.hpp)
    file(GLOB_RECURSE SRC_HH    ${source_dir}/*.hh)

    list(APPEND FILE_LISTS ${SRC_H} ${SRC_HPP}  ${SRC_HH})

    list(LENGTH ARGN num_extra_args)

    if (num_extra_args EQUAL 0)
        file(GLOB_RECURSE SRC_C     ${source_dir}/*.c)
        file(GLOB_RECURSE SRC_CPP   ${source_dir}/*.cpp)
        file(GLOB_RECURSE SRC_CC    ${source_dir}/*.cc)
        
        if(APPLE)
            file(GLOB_RECURSE SRC_MM    ${source_dir}/*.mm)
            list(APPEND FILE_LISTS ${SRC_MM})
        endif()

        list(APPEND FILE_LISTS ${SRC_C} ${SRC_CPP} ${SRC_CC})
    endif()

    set(${output_variable} ${FILE_LISTS} PARENT_SCOPE)

endfunction()


##
##  Add source files to target in a same structure as they are in on the file system
##
function(add_source_files target source_dir base_dir)
    get_files_list(${source_dir} file_list)

    foreach (ignore_dir ${ARGN})
        list(FILTER file_list EXCLUDE REGEX ^${ignore_dir}*)
    endforeach()

    target_sources(${target} PRIVATE
        ${file_list}
    )
    add_source_files_list(${base_dir} ${file_list})

endfunction()

##
##  Arrange a list of source files to match the structure of the file system
##
function(add_source_files_list base_dir)
    if(${ARGC} LESS 2)
        return()
    endif()

    list(APPEND files ${ARGV})
    list(REMOVE_ITEM files ${base_dir})
    # Group source files in the same structure as the file system
    foreach (FILE_LIST ${files})
        foreach(FILE_PATH ${FILE_LIST})
            # Always start with absolute path
            get_filename_component(ABSOLUTE_FILE_PATH ${FILE_PATH} ABSOLUTE)
            # Get relative path (to base dir)
            file(RELATIVE_PATH FILE_PATH ${base_dir} ${ABSOLUTE_FILE_PATH})
            # Extract file name/directory
            get_filename_component(FILE_NAME ${FILE_PATH} NAME)
            get_filename_component(FILE_DIRECTORY ${FILE_PATH} DIRECTORY)

            if("${FILE_NAME}" STREQUAL "")
                message(FATAL_ERROR "Bad file format")
            endif()

            # Get hierarchical structure to build source_groups
            set(FULL_GROUP_NAME "${SOURCE_GROUP_BASENAME}")
            if(NOT "${FILE_DIRECTORY}" STREQUAL "")
                string(REPLACE "/" "\\" GROUP_NAME ${FILE_DIRECTORY})
                set(FULL_GROUP_NAME "${SOURCE_GROUP_BASENAME}\\\\${GROUP_NAME}")
            endif()
            source_group("${FULL_GROUP_NAME}" FILES ${ABSOLUTE_FILE_PATH})
        endforeach()
    endforeach()
endfunction()

add_source_files(lox ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src)