# Fetched from threepp
function(add_test_executable name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE sphero Catch2::Catch2WithMain)
    target_include_directories(${name} PRIVATE "${PROJECT_SOURCE_DIR}/src")
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_subdirectory(cameras)
add_subdirectory(controls)
add_subdirectory(math)
add_subdirectory(utils)