# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Netwerk_Client_Yahtzee_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Netwerk_Client_Yahtzee_autogen.dir\\ParseCache.txt"
  "Netwerk_Client_Yahtzee_autogen"
  )
endif()
