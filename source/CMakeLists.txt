find_package(Python3 COMPONENTS Interpreter)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  COMMAND Python3::Interpreter ${CMAKE_SOURCE_DIR}/apigen.py ${CMAKE_SOURCE_DIR}/include/api_cfg.hpp ${CMAKE_SOURCE_DIR}/source/main.cpp
  COMMENT "Generating main.cpp using apigen.py script"
  DEPENDS ${CMAKE_SOURCE_DIR}/apigen.py ${CMAKE_SOURCE_DIR}/include/api_cfg.hpp
)

add_library(
    reaper_plugin OBJECT
    main.cpp
    reaper_wrapper.cpp
    maths.cpp
    time_printer.cpp
    profiler.cpp
    api.cpp
    ini_file.cpp
    api_cfg.cpp
    filesystem.cpp
    audio_source.cpp
    audio_buffer.cpp
    wildcard_parser.cpp
)

target_include_directories(
  reaper_plugin SYSTEM PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/vendor
  ${CMAKE_SOURCE_DIR}/vendor/mini/src
  ${CMAKE_SOURCE_DIR}/vendor/reaper-sdk/sdk)

target_link_libraries(${PROJECT_NAME} reaper_plugin)

if(WIN32)
  target_compile_options(reaper_plugin PUBLIC
    /W3
    /WX
    )
   target_compile_definitions(reaper_plugin PUBLIC NOMINMAX)
else()
  target_compile_options(reaper_plugin PUBLIC
    -Wall -Wextra -Werror -Wpedantic
  )
endif()

find_package(WDL REQUIRED)
target_link_libraries(reaper_plugin WDL::WDL)

if(NOT WIN32)
  find_package(SWELL REQUIRED)
  target_link_libraries(reaper_plugin SWELL::swell)
endif()