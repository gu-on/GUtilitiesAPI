find_package(Python3 COMPONENTS Interpreter)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/gu_main.cpp
  COMMAND Python3::Interpreter ${CMAKE_SOURCE_DIR}/apigen.py ${CMAKE_SOURCE_DIR}/include/gu_api.hpp ${CMAKE_SOURCE_DIR}/source/gu_main.cpp
  COMMENT "Generating main.cpp using apigen.py script"
  DEPENDS ${CMAKE_SOURCE_DIR}/apigen.py ${CMAKE_SOURCE_DIR}/include/gu_api.hpp
)

add_library(
    reaper_plugin OBJECT
    api_manager.cpp
    gu_api.cpp
    gu_main.cpp
    gu_reaper_wrapper.cpp
    gu_maths.cpp
    gu_time_printer.cpp
    gu_profiler.cpp
    gu_ini_file.cpp
    gu_recursive_importer.cpp
    gu_audio_source.cpp
    gu_audio_buffer.cpp
    gu_wildcard_parser.cpp
)

target_include_directories(
  reaper_plugin SYSTEM PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/vendor/mINI/src
  ${CMAKE_SOURCE_DIR}/vendor/fmt/include
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

target_link_libraries(reaper_plugin fmt::fmt)

if(NOT WIN32)
  find_package(SWELL REQUIRED)
  target_link_libraries(reaper_plugin SWELL::swell)
endif()
