if(WDL_FOUND)
  return()
endif()

find_path(WDL_INCLUDE_DIR
  NAMES WDL/wdltypes.h
  PATHS ${PROJECT_LIB_DIR}/WDL
  NO_DEFAULT_PATH
)
mark_as_advanced(WDL_INCLUDE_DIR)

set(WDL_DIR "${WDL_INCLUDE_DIR}/WDL")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WDL REQUIRED_VARS WDL_DIR WDL_INCLUDE_DIR)

add_library(wdl INTERFACE)

target_compile_definitions(wdl INTERFACE WDL_NO_DEFINE_MINMAX)
target_include_directories(wdl INTERFACE ${WDL_INCLUDE_DIR})

if(NOT WIN32)
  find_package(SWELL REQUIRED)
  target_link_libraries(wdl INTERFACE SWELL::swell)
endif()

add_library(WDL::WDL ALIAS wdl)
