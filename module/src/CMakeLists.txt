
set(SRC  GFmain.cpp geoshape.cpp GFimg.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp   )
add_executable(GFDC ${SRC})
target_link_libraries(GFDC ${THIRD_PARTY_LIB})


set(SRC  main.cpp geoshape.cpp GFimg.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp io.cpp )
add_executable(cmdDC ${SRC})
target_link_libraries(cmdDC ${THIRD_PARTY_LIB})