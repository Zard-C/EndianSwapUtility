find_package(GTest REQUIRED)

add_executable(test_swap_endian src/test_swap_endian.cpp)
target_link_libraries(test_swap_endian GTest::GTest GTest::Main)

install(
    TARGETS test_swap_endian
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)