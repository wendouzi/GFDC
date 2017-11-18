

IF(WIN32)
message(STATUS "now is windows")

include_directories(${WORKSPACE}/thirdparty/fltk/include)

set(FLTK_ROOT ${WORKSPACE}/thirdparty/fltk)
set(THIRD_PARTY_LIB ${FLTK_ROOT}/lib/fltk.lib
                    ${FLTK_ROOT}/lib/fltkforms.lib
                    ${FLTK_ROOT}/lib/fltkgl.lib
               ${FLTK_ROOT}/lib/fltkimages.lib
               ${FLTK_ROOT}/lib/fltkjpeg.lib
			   ${FLTK_ROOT}/lib/fltkpng.lib
			   ${FLTK_ROOT}/lib/fltkzlib.lib
               ${THIRD_PARTY_LIB})		   	   
ELSEIF(UNIX)
message(STATUS "now is linux")
set(FLTK_ROOT ${WORKSPACE}/thirdparty/fltk)
#set(FLTK_DIR  ${WORKSPACE}/thirdparty/fltk-1.3.3)
#find_package(FLTK REQUIRED NO_MODULE)
#include(${FLTK_USE_FILE})

#set(THIRD_PARTY_LIB fltk
#    ${THIRD_PARTY_LIB})
include_directories(${FLTK_ROOT}/include)
message(${FLTK_ROOT})
set(THIRD_PARTY_LIB 
               ${FLTK_ROOT}/lib/libfltk_images.a
		${FLTK_ROOT}/lib/libfltk.a
               ${FLTK_ROOT}/lib/libfltk_forms.a
               # ${FLTK_ROOT}/lib/libfltk_gl.a
                ${FLTK_ROOT}/lib/libfltk_png.a
                -lXft
               #/usr/lib64/libGL.so
                -lX11
                -ldl
                -lz
                # -lpng
                -ljpeg
                -lXfixes
                -lfontconfig
                -lXcursor
                -lXext
               ${THIRD_PARTY_LIB}
   )

#include_directories(${FLTK_INCLUDE_DIR}) 

ENDIF()

