project(simulatoriPavimenti)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

set (QT_USE_QTXML TRUE)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(simulatoriPavimenti_SRCS map/map.cpp map/mapreader.cpp simulatoriPavimenti.cpp main.cpp)
set(simulatoriPavimenti_HEADERS map/map.h map/mapreader.h simulatoriPavimenti.h)
#qt4_automoc(${simulatoriPavimenti_SRCS})
QT4_WRAP_CPP(simulatoriPavimenti_MOCS ${simulatoriPavimenti_HEADERS})
add_executable(simulatoriPavimenti ${simulatoriPavimenti_SRCS} ${simulatoriPavimenti_MOCS})
target_link_libraries(simulatoriPavimenti ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS simulatoriPavimenti RUNTIME DESTINATION bin)

add_subdirectory(map)