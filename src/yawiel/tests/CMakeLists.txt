add_executable(yawiel_test
  yawiel_test.cpp
  corpus_test.cpp
)

target_include_directories(yawiel_test PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_definitions(yawiel_test PRIVATE "BOOST_TEST_DYN_LINK=1")
target_link_libraries(yawiel_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

enable_testing()

#target_link_libraries(yawiel_test
#  yawiel
#  ${BOOST_LIBRARIES}
#)
