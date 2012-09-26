#
# Try to find RapidJSON library and include path.
# Once done this will define
#
# RAPID_JSON_FOUND
# RAPID_JSON_INCLUDE_DIR
#

FIND_PATH(RAPID_JSON_INCLUDE_DIR rapidjson/rapidjson.h
        ${CMAKE_SOURCE_DIR}/externals/rapidjson/include
        $ENV{PROGRAMFILES}/rapidjson/include
        ${COMPILER_PATH}/PlatformSDK/Include
        $ENV{JSON_ROOT_DIR}/include
        ${GLEW_ROOT_DIR}/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where rapidjson/rapidjson.h resides")

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set RAPID_JSON_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RAPID_JSON DEFAULT_MSG RAPID_JSON_INCLUDE_DIR)
