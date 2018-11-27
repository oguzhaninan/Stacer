set(APP_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/stacer")
set(LIB_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/stacer-core")
set(TEST_OUT "${PROJECT_ROOT}/output/")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_definitions(-DGNU_SOURCE)

find_package(Qt5 COMPONENTS Core REQUIRED)

include_directories(
    ${PROJECT_ROOT}/stacer
    ${PROJECT_ROOT}/stacer-core
)

macro(app_test a_name a_file)
	string(CONCAT TEST_NAME "tests_" "${a_name}")
	add_executable(${TEST_NAME} ${APP_TESTS}/${a_file})
	add_test(NAME ${a_name} COMMAND ${TEST_NAME} WORKING_DIRECTORY ${TEST_OUT})
	target_link_libraries(${TEST_NAME} Qt5::Core)
	set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUT})
	set_property(TEST ${a_name} PROPERTY ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1")
endmacro()
macro(lib_test l_name l_file)
	string(CONCAT TEST_NAME "tests_" "${l_name}")
	add_executable(${TEST_NAME} ${LIB_TESTS}/${l_file})
	add_test(NAME ${l_name} COMMAND ${TEST_NAME} WORKING_DIRECTORY ${TEST_OUT})
	target_link_libraries(${TEST_NAME} stacer-core Qt5::Core)
	set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUT})
	set_property(TEST ${l_name} PROPERTY ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1")
endmacro()

#################3 ADD TESTS BELOW ############

lib_test("desktop_apps" desktop_apps.cpp)
