project(Actions_Test)

add_executable(Actions_test Actions_test.cpp)
target_link_libraries(Actions_test atlascore atlasinsts softfloat ${ATLAS_LIBS})

atlas_named_test(Actions_test_run Actions_test)
