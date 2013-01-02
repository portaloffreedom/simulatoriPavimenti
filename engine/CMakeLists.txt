
set(engine_QT_HEADERS behaviors/pathbehavior.h behaviors/defaultbehavior.h agentbehavior.h trafficengine.h agent.h)
set(engine_SRCS behaviors/pathbehavior.cpp behaviors/defaultbehavior.cpp agentbehavior.cpp trafficengine.cpp agent.cpp)

QT4_WRAP_CPP(engine_MOCS ${engine_QT_HEADERS})
add_library(engine STATIC ${engine_SRCS} ${engine_MOCS})
