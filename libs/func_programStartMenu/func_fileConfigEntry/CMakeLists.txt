add_library(fileConfigEntry STATIC 
  fileConfigEntry.cpp
)

target_include_directories(PRIVATE 
  ${CMAKE_SOURCE_DIR}/libs
)

target_link_libraries(fileConfigEntry PRIVATE 
  programInitHelper
)
