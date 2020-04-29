################################################################################
#    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
#                                                                              #
#              This software is distributed under the terms of the             #
#              GNU Lesser General Public Licence (LGPL) version 3,             #
#                  copied verbatim in the file "LICENSE"                       #
################################################################################

Macro(Generate_Version_Info)
 Find_Package(Git)

 If(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
   Execute_Process(COMMAND ${GIT_EXECUTABLE} describe --tags
                   OUTPUT_VARIABLE FAIRROOT_GIT_VERSION
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  )
   Execute_Process(COMMAND ${GIT_EXECUTABLE} log -1 --format=%cd
                   OUTPUT_VARIABLE FAIRROOT_GIT_DATE
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  )
   Message(STATUS "FairRoot ${FAIRROOT_GIT_VERSION} from ${FAIRROOT_GIT_DATE}")
   Configure_File(cmake/scripts/FairVersion.h.tmp ${CMAKE_BINARY_DIR}/FairVersion.h @ONLY)
 Else()
   Message(STATUS "FairRoot v${FAIRROOT_VERSION}")
   Configure_File(cmake/scripts/FairVersion.h.default ${BINARY_DIR}/FairVersion.h @ONLY)
 EndIf()
endmacro()
