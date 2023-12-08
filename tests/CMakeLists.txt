include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

add_executable(Test 
    test.cpp 
    doctest.h
)

target_link_libraries(${PROJECT_NAME} reaper_plugin)

add_test(NAME test1 COMMAND Test)
