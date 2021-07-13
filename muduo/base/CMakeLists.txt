set(base_SRCS
  common/async_logging.cpp
  common/condition.cpp
  common/count_down_latch.cpp
  common/current_thread.cpp
  common/date.cpp
  common/file_util.cpp
  common/log_stream.cpp
  common/logfile.cpp
  common/logging.cpp
  common/process_info.cpp
  common/thread.cpp
  common/time_stamp.cpp
  common/time_zone.cpp
  common/threadpool.cpp
  )

add_library(muduo_base ${base_SRCS})
target_link_libraries(muduo_base pthread rt)

install(TARGETS muduo_base DESTINATION lib)

file(GLOB baseDefineHeaders "define/*.h")
install(FILES ${baseDefineHeaders} DESTINATION include/muduo/base/define)

file(GLOB baseCommonHeaders "common/*.h")
install(FILES ${baseCommonHeaders} DESTINATION include/muduo/base/common)


add_subdirectory(tests)

