# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Travel_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Travel_autogen.dir\\ParseCache.txt"
  "Travel_autogen"
  )
endif()
