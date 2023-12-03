include(GNUInstallDirs)

add_executable(rapishow rapishow.cpp)
target_link_libraries(rapishow PRIVATE fb Boost::program_options Boost::log)
install(TARGETS rapishow)
install(FILES man/rapishow TYPE MAN)

add_executable(rapiplay rapiplay.cpp)
target_link_libraries(rapiplay PRIVATE fb Boost::program_options Boost::log)
install(TARGETS rapiplay)
install(FILES man/rapiplay TYPE MAN)

add_executable(rapiresize rapiresize.cpp)
target_link_libraries(rapiresize PRIVATE fb Boost::program_options Boost::log)
install(TARGETS rapiresize)
install(FILES man/rapiresize TYPE MAN)
