cmake_minimum_required(VERSION 3.5)


message("Starting!")


if(NOT GTEST_LIBRARY)
#    message("!!!!not found!!!!")
    set(GTEST_INCLUDE_DIR "/opt/googletest-release-1.8.1/googletest/include/")
    set(GTEST_ROOT "/opt/googletest-release-1.8.1/googletest/" CACHE PATH "Path to Googletest")
    set(GTEST_LIBRARY "/opt/googletest-release-1.8.1/googletest/make/gtest-all.o")
    set(GTEST_MAIN_LIBRARY "/opt/googletest-release-1.8.1/googletest/make/gtest_main.o")
else()
    message("Found gtest!")
endif()

find_package(GTest REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIR})


file(GLOB srcFiles "*.cc")
add_executable(test_${PROJECT_NAME} 
        ${CMAKE_CURRENT_LIST_DIR}/test_reflected_value.cc
        ${CMAKE_CURRENT_LIST_DIR}/test_reflected_layered_object.cc
        ${CMAKE_CURRENT_LIST_DIR}/test_values.cc
        )


target_include_directories(test_${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_link_libraries(test_${PROJECT_NAME} PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test_${PROJECT_NAME} PRIVATE pthread)
target_link_libraries(test_${PROJECT_NAME} PRIVATE ${PROJECT_NAME})

add_custom_command(TARGET test_${PROJECT_NAME}
    POST_BUILD
    COMMAND test_${PROJECT_NAME} -c $<CONFIGURATION> --output-on-failure)


