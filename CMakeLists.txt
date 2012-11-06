project(simulatoriPavimenti)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(simulatoriPavimenti_SRCS simulatoriPavimenti.cpp main.cpp)
qt4_automoc(${simulatoriPavimenti_SRCS})
add_executable(simulatoriPavimenti ${simulatoriPavimenti_SRCS})
target_link_libraries(simulatoriPavimenti ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS simulatoriPavimenti RUNTIME DESTINATION bin)
