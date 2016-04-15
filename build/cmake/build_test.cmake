#----------------------------------------------------------------------------------------------------------------------
#  functions for build source file
#
#  Copyright (c) 2015 Intel NDG
#
#  binfeix.li@intel.com 2015/07/20 v1.0
#
#----------------------------------------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------------------------------------
# find all source code
# call style:
#     ADD_BUILD_TEST module 
#

MACRO(ADD_BUILD_TEST)
  MESSAGE("FUNCTION: ADD_BUILD_TEST(" ${ARGN} ")")
  
  SET(SOURCE_PATH ${ARGN})
  
  INCLUDE_DIRECTORIES(${SOURCE_PATH})
  FILE(GLOB_RECURSE CPP_SOURCE_FILES ${PROJECT_SOURCE_PATH}/*.c* ${PROJECT_SOURCE_PATH}/*.h*)
  FILE(GLOB_RECURSE C_SOURCE_FILES ${PROJECT_SOURCE_PATH}/*.c ${PROJECT_SOURCE_PATH}/*.h)
  
  ADD_LIBRARY()
  
ENDMACRO()
