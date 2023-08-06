include_directories(
    include
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(example
    src/example.cpp
)

#install
install(TARGETS example
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)