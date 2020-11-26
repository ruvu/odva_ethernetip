cmake_minimum_required(VERSION 3.5)
# Plain cmake build was inspired by abb_libegm:
# https://github.com/ros-industrial/abb_libegm/blob/master/CMakeLists.txt

# Read version from the package.xml file.
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/package.xml package_xml_str)
if(NOT package_xml_str MATCHES "<version>([0-9]+.[0-9]+.[0-9]+)</version>")
  message(FATAL_ERROR "Could not parse project version from package.xml. Aborting.")
endif()

# At this point we either have a proper version string, or we've errored
# out with a FATAL_ERROR above. So assume CMAKE_MATCH_1 contains our
# package's version.
project(odva_ethernetip VERSION ${CMAKE_MATCH_1} LANGUAGES CXX)


# find dependencies
find_package(Boost REQUIRED)
find_package(console_bridge REQUIRED)

## Declare a cpp library for just Ethernet/IP generic stuff
add_library(${PROJECT_NAME} SHARED
  src/connection.cpp
  src/copy_serializable.cpp
  src/cpf_item.cpp
  src/cpf_packet.cpp
  src/encap_header.cpp
  src/encap_packet.cpp
  src/identity_item_data.cpp
  src/io_scanner.cpp
  src/message_router_response.cpp
  src/path.cpp
  src/rr_data.cpp
  src/session.cpp
)
target_include_directories(odva_ethernetip PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PUBLIC ${Boost_INCLUDE_DIRS}
  PUBLIC ${console_bridge_INCLUDE_DIRS}
)
target_link_libraries(odva_ethernetip
 ${Boost_LIBRARIES}
 ${console_bridge_LIBRARIES}
)

install(
  DIRECTORY include/
  DESTINATION include
)
install(
  TARGETS odva_ethernetip
  EXPORT export_odva_ethernetip
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

# export targets
set(export_targets ${export_targets};odva_ethernetip)
export(EXPORT export_odva_ethernetip
  FILE "${PROJECT_BINARY_DIR}/export_odva_ethernetip.cmake")

# Create the ${PROJECT_NAME}Config.cmake.in
set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
configure_file(cmake/${PROJECT_NAME}Config.cmake.in
  "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY)

include(CMakePackageConfigHelpers)

# Create the ${PROJECT_NAME}ConfigVersion.cmake.
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY AnyNewerVersion
)


install(FILES
  "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "share/${PROJECT_NAME}/cmake" COMPONENT dev)
install(EXPORT export_odva_ethernetip
  DESTINATION "share/${PROJECT_NAME}/cmake"
  FILE export_odva_ethernetip.cmake
  COMPONENT dev)
