
if(WIN32)
set(GDAL_ROOT ${WORKSPACE}/thirdparty/gdal)
else()
set(GDAL_ROOT ${WORKSPACE}/thirdparty/gdal/)
endif()
include(cmake/FindGDAL.cmake)

# spd log
include_directories(${WORKSPACE}/thirdparty/spdlog)


if(UNIX)
    set(BOOST_ROOT ${WORKSPACE}/thirdparty/boost)
   find_package(Boost COMPONENTS system filesystem REQUIRED)
endif(UNIX)
if(WIN32)
    # set(BOOST_ROOT ${WORKSPACE}/thirdparty/boost/boost_1_65_1)
    set(Boost_INCLUDE_DIR   ${WORKSPACE}/thirdparty/boost/winboost_1_58_0)
    message(${Boost_INCLUDE_DIR})
    set(Boost_LIBRARIES ${WORKSPACE}/thirdparty/boost/winboost_1_58_0/lib64-msvc-12.0/libboost_system-vc120-mt-1_58.lib
    ${WORKSPACE}/thirdparty/boost/winboost_1_58_0/lib64-msvc-12.0/libboost_filesystem-vc120-mt-1_58.lib
    )
    
    
    set(Boost_FOUND 1)
    # find_package(Boost COMPONENTS system filesystem REQUIRED)
    # set(Boost_INCLUDE_DIR  ${WORKSPACE}/thirdparty/boost/winboost_1_53_0/include)
    # set(Boost_LIBRARIES  ${WORKSPACE}/thirdparty/boost/winboost_1_53_0/lib64-msvc-9.0)
endif(WIN32)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    message(${Boost_LIBRARIES})
    set(THIRD_PARTY_LIB ${THIRD_PARTY_LIB} ${Boost_LIBRARIES})
endif(Boost_FOUND)

message("thirdparty done")
