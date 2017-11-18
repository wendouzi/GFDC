
set(SRC  GFmain.cpp geoshape.cpp GFimg.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp   )
add_executable(GFDC ${SRC})
target_link_libraries(GFDC ${THIRD_PARTY_LIB})

