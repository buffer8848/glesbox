#----------------------------------------------------------------------------------------------------------------------
#  functions for build utils file
#
#  Copyright (c) 2015 Intel NDG
#
#  author:  binfeix.li@intel.com
#  version: v1.0
#  time:    2015/07/21
#
#----------------------------------------------------------------------------------------------------------------------


#----------------------------------------------------------------------------------------------------------------------
# find all modules
# call style:
#     FIND_MODULES path 
#----------------------------------------------------------------------------------------------------------------------
MACRO(FIND_MODULES path_name)
  FILE(GLOB_RECURSE ALL_FILES LIST_DIRECTORIES true ${path_name} "*")
  FILE(GLOB_RECURSE ALL_NOTDIR LIST_DIRECTORIES false ${path_name} "*")
  #MESSAGE("all found: " ${ALL_FILES} " directory Finded: " ${ALL_NOTDIR})
  
  #filter folder
  FOREACH(file ${ALL_NOTDIR})
    LIST(REMOVE_ITEM ALL_FILES ${file})
  ENDFOREACH()
  #MESSAGE("removed found: " ${ALL_FILES})
  
  #filter folder
  FOREACH(file ${ALL_FILES})
    MESSAGE("directory Finded: " ${file})
    ADD_SUBDIRECTORY(${file})
  ENDFOREACH()
ENDMACRO()
