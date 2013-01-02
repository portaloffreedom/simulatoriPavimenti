
set(service_QT_HEADERS timer.h)
set(service_SRCS timer.cpp randomservice.cpp)

QT4_WRAP_CPP(service_MOCS ${service_QT_HEADERS})
add_library(service STATIC ${service_SRCS} ${service_MOCS})