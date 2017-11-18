
if(NOT EXISTS ${GDAL_ROOT})
    set(GDAL_FOUND false)
    message(FATAL_ERROR "not found gdal")
endif()

if(EXISTS ${GDAL_ROOT})
    set(GDAL_FOUND true)
    message("use gdal")

    if(UNIX)
        set(GDAL_INCLUDE_DIR  ${GDAL_ROOT}/linux64/include)
        set(GDAL_LIBRARIES    ${GDAL_ROOT}/linux64/lib/libgdal.so
                             /usr/lib64/libhdf5.so
                             /usr/lib64/hdf/libmfhdf.a
            )
    else()
        set(GDAL_INCLUDE_DIR  ${GDAL_ROOT}/win32/include)
        set(GDAL_LIBRARIES    ${GDAL_ROOT}/win32/lib/gdal_i.lib
            #           ${GDAL_ROOT}/win64/runtime/hm426m.lib
            #            ${GDAL_ROOT}/win64/runtime/geos_c_i.lib
            #${GDAL_ROOT}/win64/runtime/hdf5_hldll.lib
            #${GDAL_ROOT}/win64/runtime/mfhdf_fcstubdll.lib
            #${GDAL_ROOT}/win64/runtime/mfhdf_fortrandll.lib
            #${GDAL_ROOT}/win64/runtime/geos_c_i.lib
            #${GDAL_ROOT}/win64/runtime/proj_i.lib
            #${GDAL_ROOT}/win64/runtime/hd426m.lib
            #${GDAL_ROOT}/win64/runtime/szip.lib
            #${GDAL_ROOT}/win64/runtime/hdf5dll.lib
            #${GDAL_ROOT}/win64/runtime/xdr_for_dll.lib
            #${GDAL_ROOT}/win64/runtime/zlib1.lib
            #${GDAL_ROOT}/win64/runtime/hdf5_hl_f90cstubdll.lib
            #${GDAL_ROOT}/win64/runtime/hdf5_hl_fortrandll.lib
            #${GDAL_ROOT}/win64/runtime/hdf_fcstubdll.lib
            #${GDAL_ROOT}/win64/runtime/hdf_fortrandll.lib
            
            )



    endif()
    include_directories(${GDAL_INCLUDE_DIR})
    set(THIRD_PARTY_LIB ${THIRD_PARTY_LIB} ${GDAL_LIBRARIES})
endif()

message(${GDAL_LIBRARIES})
message(${GDAL_INCLUDE_DIR})
