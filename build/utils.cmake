# fills ${RESULT} with ELEMENTS matching given REGEX
macro(filter result elements regex)
    set(_result "")
    foreach(element ${elements})
        if(${element} MATCHES ${regex})
            set(_result ${_result} ${element})
        endif()
    endforeach()
    set(${result} ${_result})
endmacro()

# fills ${RESULT} with ELEMENTS not matching given REGEX
macro(filter_out result elements regex)
    set(_result "")
    foreach(element ${elements})
        if(NOT ${element} MATCHES ${regex})
            set(_result ${_result} ${element})
        endif()
    endforeach()
    set(${result} ${_result})
endmacro()

# sets a ${VAR} to VALUE if it's not yet defined
macro(set_default var value)
    if (NOT DEFINED ${var})
        set(${var} ${value})
    endif()
endmacro()

macro(ensure_one_enabled)
    set(_choice "")
    foreach(var ${ARGN})
        if(${var})
            if(${_choice})
                message(FATAL_ERROR "only one of: ${_choice} ${var} can be set")
            endif()
            set(_choice ${var})
        endif()
    endforeach()

    if(NOT ${_choice})
        message(FATAL_ERROR "you must enable one of ${ARGN}")
    endif()
endmacro()

# finds paths to immediate subdirectories of ROOT_DIR whose names start with 
# CHILD_NAME and places them in ${RESULT}
macro(find_matching_children result root_dir child_name)
    file(GLOB _children ${root_dir}/*/)
    filter(${result} "${_children}" "^.*/${child_name}[^/]*$")
endmacro()

# adds subdirectories of SDK_DIR with names starting with LIB_NAME to
# CMAKE_PREFIX_PATH to look into them when doing find_path/find_library
macro(find_library_dir sdk_dir lib_name)
    find_matching_children(_lib_dir ${sdk_dir} ${lib_name})
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${_lib_dir})
endmacro()

macro(split_dirs_files dirs_result files_result paths)
    set(${dirs_result} "")
    set(${files_result} "")

    foreach(_path in ${paths})
        if(IS_DIRECTORY ${_path})
            list(APPEND ${dirs_result} ${_path})
        else()
            list(APPEND ${files_result} ${_path})
        endif()
    endforeach()
endmacro()

# recursively looks for files ending with EXTENSION in ROOT_DIR, adding then
# to child of source_group GROUP to make things tidy in VS projexts
macro(find_sources result root_dir extension group)
    set(_paths ${root_dir})
    set(${result} "")

    while(_paths)
        list(GET _paths 0 _curr_path)
        list(REMOVE_AT _paths 0)

        file(GLOB _subpaths ${_curr_path}/*)
        split_dirs_files(_subdirs _subfiles "${_subpaths}")
        filter(_sources "${_subfiles}" "^.*${extension}$")

        string(REGEX REPLACE "^${root_dir}" "" _stripped_path ${_curr_path})
        if(NOT "" STREQUAL "${_stripped_path}")
            string(REGEX REPLACE "/" "\\\\" _stripped_path ${_stripped_path})
        endif()
        source_group("${group}${_stripped_path}" FILES ${_sources})
        set(${result} ${${result}} ${_sources})

        list(APPEND _paths ${_subdirs})
    endwhile()
endmacro()

macro(append_matching_paths result pattern)
    foreach(_dirname ${ARGN})
        file(GLOB_RECURSE _paths ${_dirname}/*)
        foreach(_path ${_paths})
            get_filename_component(_path "${_path}" ABSOLUTE)
            if(${_path} MATCHES ${pattern})
                list(APPEND ${result} ${_path})
            endif()
        endforeach()
    endforeach()
endmacro()

macro(add_external_library name)
    find_package(${name} ${ARGN})

    if("${name}" STREQUAL "Qt" AND QT_USE_FILE)
        include(${QT_USE_FILE})
    endif()

    string(TOUPPER ${name} name_upper)
    include_directories(${${name_upper}_INCLUDE_DIR})
    set(LIBS ${LIBS}
             ${${name_upper}_LIBRARY}
             ${${name_upper}_LIBRARIES})

    if(NOT "${name}" STREQUAL "${name_upper}")
        include_directories(${${name}_INCLUDE_DIR})
        set(LIBS ${LIBS}
                 ${${name}_LIBRARY}
                 ${${name}_LIBRARIES})
    endif()
endmacro()

