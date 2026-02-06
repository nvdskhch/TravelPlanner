# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TravelTests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TravelTests_autogen.dir\\ParseCache.txt"
  "TravelTests_autogen"
  )
endif()
