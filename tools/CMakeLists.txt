add_executable(rapishow rapishow.cpp)
target_link_libraries(rapishow PRIVATE fb)

add_executable(rapiplay rapiplay.cpp)
target_link_libraries(rapiplay PRIVATE fb)

add_executable(rapiresize rapiresize.cpp)
target_link_libraries(rapiresize PRIVATE fb)
