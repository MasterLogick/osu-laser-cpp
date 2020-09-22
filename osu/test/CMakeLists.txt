#define test resources paths
set(SMALL_OSB "${PROJECT_SOURCE_DIR}/osu-test-resources/cillia - FIRST (moph).osb")
set(BIG_OSB "${PROJECT_SOURCE_DIR}/osu-test-resources/Mili - world.execute(me); (Exile-).osb")

#generate pre-compiled paths header
configure_file(TestResourcesPaths.h.in ${PROJECT_BINARY_DIR}/TestResourcesPaths.h)

#generate list of tests
#todo rewrite with file
list(APPEND TESTS SimpleTest DevTest StartWithTest)

#configure tests
foreach (TEST IN LISTS TESTS)
    add_executable(${TEST} ${TEST}.cpp)
    target_include_directories(${TEST} PUBLIC ${PROJECT_SOURCE_DIR}/osu/desktop)
    target_include_directories(${TEST} PUBLIC ${PROJECT_BINARY_DIR})
    target_link_libraries(${TEST} PUBLIC ${CMAKE_PROJECT_NAME}_lib)
    add_test(NAME ${TEST} COMMAND ${TEST} PASS_RETURN_CODE 0 FAIL_RETURN_CODE 1)
endforeach (TEST)