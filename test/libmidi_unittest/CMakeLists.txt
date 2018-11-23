add_executable(libmidi_unittest
  ${LIBMIDI_EXPORT_HEADER}
  ${LIBMIDI_VERSION_HEADER}
  ${LIBMIDI_CONFIG_HEADER}
  gtesthelper.cpp
  gtesthelper.h
  main.cpp
  TestMidiFile.cpp
  TestMidiFile.h
)

# Unit test projects requires to link with pthread if also linking with gtest
if(NOT WIN32)
  set(PTHREAD_LIBRARIES -pthread)
endif()

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(libmidi_unittest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(libmidi_unittest 
  PRIVATE
    ${GTEST_INCLUDE_DIR}
)
add_dependencies(libmidi_unittest libmidi)
target_link_libraries(libmidi_unittest PRIVATE rapidassist libmidi ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

# Copy test files database to target dir
add_custom_command( TARGET libmidi_unittest POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/test_files $<TARGET_FILE_DIR:libmidi_unittest>/test_files
                    COMMENT "Copying test_files database.")

# Copy test files to build dir for local execution (from within the IDE)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS libmidi_unittest
        EXPORT libmidi-targets
        ARCHIVE DESTINATION ${LIBMIDI_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBMIDI_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBMIDI_INSTALL_BIN_DIR}
)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Test.CommandLines.Windows.txt       
              ${CMAKE_CURRENT_SOURCE_DIR}/Test.CommandLines.Linux.txt
        DESTINATION ${LIBMIDI_INSTALL_BIN_DIR}
)
