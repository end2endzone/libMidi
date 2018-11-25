add_executable(libmidi_samples
  ${LIBMIDI_EXPORT_HEADER}
  ${LIBMIDI_VERSION_HEADER}
  ${LIBMIDI_CONFIG_HEADER}
  samples.cpp
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(libmidi_samples PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(libmidi_samples PRIVATE rapidassist)
target_include_directories(libmidi_samples PRIVATE ${GTEST_INCLUDE_DIR})
add_dependencies(libmidi_samples libmidi)
target_link_libraries(libmidi_samples PUBLIC rapidassist libmidi ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )
