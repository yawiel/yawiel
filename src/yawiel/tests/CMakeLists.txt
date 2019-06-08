add_executable(yawiel_test
  yawiel_test.cpp
  corpus_test.cpp
  ngram_counter_test.cpp
  eam_test.cpp
  colext_test.cpp
  container_test.cpp
)

include_directories(${Boost_INCLUDE_DIRS})
#target_include_directories(yawiel_test PRIVATE ${Boost_INCLUDE_DIRS})
#target_compile_definitions(yawiel_test PRIVATE "BOOST_TEST_DYN_LINK=1")
target_link_libraries(yawiel_test
#    #${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
#    #${Boost_SERIALIZATION_LIBRARY}
    ${Boost_LIBRARIES})
# JUST FOR DWS TESTING PURPOSE USE ALL STATIC.

# descomentar -- set(CMAKE_EXE_LINKER_FLAGS "-static")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
target_link_libraries(yawiel_test -static-libgcc -static-libstdc++)
# /lib64/libc.a /lib64/libm.a /usr/lib/libdl.a
enable_testing()

#target_link_libraries(yawiel_test
#  yawiel
#  ${BOOST_LIBRARIES}
#)
