cmake_minimum_required(VERSION 2.8.11)

add_compile_options(-std=c++11)

project(cpptask_tictactoe)

find_package(Qt5 REQUIRED Widgets Network)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_cpp(MOC_WRAPPERS mainwindow.h gameserver.h gameclient.h
    gamefield.h gamenetworkservicebase.h gamemanager.h)
qt5_wrap_ui(UIC_WRAPPERS mainwindow.ui)

add_executable(
    cpptask_tictactoe WIN32
    main.cpp mainwindow.cpp mainwindow.h createnetsession.cpp
    createnetsession.cpp gameserver.cpp gameserver.h gameclient.cpp
    gameclient.h gamefield.cpp gamefield.h gamenetworkservicebase.h
    gamenetworkservicebase.cpp gamemanager.cpp gamemanager.h
    ${MOC_WRAPPERS} ${UIC_WRAPPERS})

#target_compile_features(cpp_qmlgraph PRIVATE cxx_generalized_initializers)

target_link_libraries(cpptask_tictactoe Qt5::Widgets Qt5::Network)

# End of File
