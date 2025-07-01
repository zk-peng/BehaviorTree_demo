#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "BT::behaviortree_cpp" for configuration ""
set_property(TARGET BT::behaviortree_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(BT::behaviortree_cpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libbehaviortree_cpp.a"
  )

list(APPEND _cmake_import_check_targets BT::behaviortree_cpp )
list(APPEND _cmake_import_check_files_for_BT::behaviortree_cpp "${_IMPORT_PREFIX}/lib/libbehaviortree_cpp.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
