cmake_minimum_required(VERSION 3.7)
project(ReflectionLib)

if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

# Glob of headers for installing
file(GLOB headers "${CMAKE_CURRENT_LIST_DIR}/*.hh")


add_library(${PROJECT_NAME} STATIC ${LinkedObjects} "")
add_library(${PROJECT_NAME} ALIAS ${PROJECT_NAME}
    )

# Add extra headers to include here
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
    ${HeaderPath}
    )

# put sources here
target_sources(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/reflected_value.cc
    ${CMAKE_CURRENT_LIST_DIR}/reflected_layered_object.cc
    )


# any libraries above should be linked here
target_link_libraries(${PROJECT_NAME} PUBLIC
#    ${LibsPath}libCoreLib.a
    )



if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# This sets the linked objects to an external.  Tells the linker to not be dumb
set_source_files_properties(
    ${LinkedObjects}
    PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true
    )

# installs the headers to the correct location
foreach(header ${headers})
    install(
        FILES ${header} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
endforeach()

# installs our libs to */lib64/*.a and binaries to */bin/
install(TARGETS
    ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bin
    )

