
set(map_QT_HEADERS map.h mapreader.h)
set(map_SRCS circle.cpp polygon.cpp map.cpp mapreader.cpp spatialobject.cpp)

QT4_WRAP_CPP(map_MOCS ${map_QT_HEADERS})
add_library(map STATIC ${map_SRCS} ${map_MOCS})
