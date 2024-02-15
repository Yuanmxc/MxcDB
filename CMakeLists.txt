cmake_minimum_required(VERSION 3.5)
project(MxcDB)

set(CMAKE_CXX_STANDARD 14)
link_libraries(-pthread)

SET(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS} -g -fno-strict-aliasing -O2 -Wall -Wextra -pipe
-D_GNU_SOURCE -D_REENTRANT -fPIC -Wno-deprecated -m64 -Wnonnull -Wstringop-overflow -Wuninitialized -Wunused)

aux_source_directory(src/db MXCDB_DB)
aux_source_directory(src/uyil MXCDB_UTIL)
#搜索目录下所有的cpp文件

add_executable(MxcDB
        a.cc
        ${MXCDB_DB}
        ${MXCDB_UTIL}

)