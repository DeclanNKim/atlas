project(Utils_Test)

add_executable(ByteVector_test ByteVector_test.cpp)
target_link_libraries(ByteVector_test atlascore atlasinsts softfloat ${ATLAS_LIBS})

atlas_named_test(ByteVector_test_run ByteVector_test)
