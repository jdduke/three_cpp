#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_DIR
# GLEW_LIBRARY
#

IF (WIN32)
    FIND_PATH(GLEW_INCLUDE_DIR GL/glew.h
        $ENV{PROGRAMFILES}/GLEW/include
        ${COMPILER_PATH}/PlatformSDK/Include
        $ENV{GLEW_ROOT_DIR}/include
        ${GLEW_ROOT_DIR}/include
        DOC "The directory where GL/glew.h resides")

    FIND_LIBRARY( GLEW_LIBRARY
        NAMES glew GLEW glew32 glew32s glew64
        PATHS
        $ENV{PROGRAMFILES}/GLEW/lib
        $ENV{GLEWROOT}/lib
        $ENV{GLEW_ROOT}/lib
        $ENV{GLEW_DIR}/lib
        $ENV{GLEW_ROOT_DIR}/lib
        ${GLEW_ROOT_DIR}/lib
        DOC "The GLEW library")
ELSE (WIN32)
    FIND_PATH(GLEW_INCLUDE_DIR GL/glew.h
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        ${GLEW_ROOT_DIR}/include
        DOC "The directory where GL/glew.h resides")

    # Prefer the static library.
    FIND_LIBRARY(GLEW_LIBRARY
        NAMES libGLEW.a glew64 GLEW glew glew32
        PATHS
        /usr/lib64
        /usr/lib
        /usr/local/lib64
        /usr/local/lib
        /sw/lib
        /opt/local/lib
        ${GLEW_ROOT_DIR}/lib
        DOC "The GLEW library")
ENDIF (WIN32)

GET_FILENAME_COMPONENT(GLEW_LIBRARY_DIR ${GLEW_LIBRARY} PATH)

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GLEW_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)
