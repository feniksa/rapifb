include(GNUInstallDirs)

add_executable(rapishow rapishow.cpp)
target_link_libraries(rapishow PRIVATE fb Boost::program_options Boost::log)

install(TARGETS rapishow)
add_custom_target(man-rapishow
  ALL
  DEPENDS rapishow.8.gz)

add_custom_command(
  OUTPUT rapishow.8.gz
  COMMAND gzip -9 --no-name --to-stdout ${CMAKE_CURRENT_SOURCE_DIR}/man/rapishow.8 >"${CMAKE_CURRENT_BINARY_DIR}/rapishow.8.gz"
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/man/rapishow.8)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rapishow.8.gz DESTINATION ${CMAKE_INSTALL_MANDIR}/man8)

add_executable(rapiplay rapiplay.cpp)
target_link_libraries(rapiplay PRIVATE fb Boost::program_options Boost::log)

install(TARGETS rapiplay)
add_custom_target(man-rapiplay
  ALL
  DEPENDS rapiplay.8.gz)
add_custom_command(
  OUTPUT rapiplay.8.gz
  COMMAND gzip -9 --no-name --to-stdout
  ${CMAKE_CURRENT_SOURCE_DIR}/man/rapiplay.8
  >"${CMAKE_CURRENT_BINARY_DIR}/rapiplay.8.gz"
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/man/rapiplay.8)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rapiplay.8.gz DESTINATION ${CMAKE_INSTALL_MANDIR}/man8)

add_executable(rapiresize rapiresize.cpp)
target_link_libraries(rapiresize PRIVATE fb Boost::program_options Boost::log)

install(TARGETS rapiresize)
add_custom_target(man-rapiresize
  ALL
  DEPENDS rapiresize.8.gz)
add_custom_command(
  OUTPUT rapiresize.8.gz
  COMMAND gzip -9 --no-name --to-stdout
  ${CMAKE_CURRENT_SOURCE_DIR}/man/rapiresize.8
  >"${CMAKE_CURRENT_BINARY_DIR}/rapiresize.8.gz"
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/man/rapiresize.8)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rapiresize.8.gz DESTINATION ${CMAKE_INSTALL_MANDIR}/man8)

