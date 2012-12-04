
set(engine_QT_HEADERS trafficengine.h)
set(engine_SRCS trafficengine.cpp)

QT4_WRAP_CPP(engine_MOCS ${engine_QT_HEADERS})
add_library(engine STATIC ${engine_SRCS} ${engine_MOCS})
