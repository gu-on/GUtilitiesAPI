include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/vendor/mINI/src
)

add_executable(Test 
    test.cpp 
    doctest.h
)

target_link_libraries(Test reaper_plugin)

add_test(NAME test1 COMMAND Test)
