project(simulatoriPavimenti)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

SET (QT_USE_QTXML TRUE)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(--std=c++11 SUPPORTS_STD_CXX11)
    check_cxx_compiler_flag(--std=c++0x SUPPORTS_STD_CXX01)
    if(SUPPORTS_STD_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c++11")
    elseif(SUPPORTS_STD_CXX01)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c++0x")
    else()
        message(ERROR "Compiler does not support --std=c++11 or --std=c++0x.")
    endif()
endif()


include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(simulatoriPavimenti_SRCS simulatoriPavimentiLayout.cpp simulatoriPavimenti.cpp main.cpp)
set(simulatoriPavimenti_HEADERS simulatoriPavimenti.h)

add_subdirectory(map)
add_subdirectory(engine)
add_subdirectory(service)

#qt4_automoc(${simulatoriPavimenti_SRCS})
QT4_WRAP_CPP(simulatoriPavimenti_MOCS ${simulatoriPavimenti_HEADERS})
add_executable(simulatoriPavimenti ${simulatoriPavimenti_SRCS} ${simulatoriPavimenti_MOCS})
target_link_libraries(simulatoriPavimenti ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} map engine service)
install(TARGETS simulatoriPavimenti RUNTIME DESTINATION bin)