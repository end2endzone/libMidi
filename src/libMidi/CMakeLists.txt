set(LIBMIDI_HEADER_FILES ""
  ${LIBMIDI_INCLUDE_DIR}/libmidi/libMidi.h
)

add_library(libmidi
  ${LIBMIDI_HEADER_FILES}
  ${LIBMIDI_EXPORT_HEADER}
  ${LIBMIDI_VERSION_HEADER}
  ${LIBMIDI_CONFIG_HEADER}
  libMidi.cpp
  instruments.h
  pitches.h
  ${CMAKE_SOURCE_DIR}/src/common/varlength.h
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(libmidi PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for exported code.
target_include_directories(libmidi
  PUBLIC
    $<INSTALL_INTERFACE:${LIBMIDI_INSTALL_INCLUDE_DIR}>  # for clients using the installed library.
)
target_link_libraries(libmidi PUBLIC rapidassist)

install(TARGETS libmidi
        EXPORT libmidi-targets
        ARCHIVE DESTINATION ${LIBMIDI_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBMIDI_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBMIDI_INSTALL_BIN_DIR}
)
