# FindMPG123.cmake
# This module finds the MPG123 library and defines MPG123::MPG123 target.

# Set the search paths for include and library files
find_path(MPG123_INCLUDE_DIR mpg123.h)
find_library(MPG123_LIBRARY NAMES mpg123)

# Check if both include and library were found
if (MPG123_INCLUDE_DIR AND MPG123_LIBRARY)
    # Both were found, set the package to FOUND
    set(MPG123_FOUND TRUE)

    # Define MPG123_INCLUDE_DIRS and MPG123_LIBRARIES variables
    set(MPG123_INCLUDE_DIRS ${MPG123_INCLUDE_DIR})
    set(MPG123_LIBRARIES ${MPG123_LIBRARY})

    # Define an imported target for easier usage
    add_library(MPG123::MPG123 UNKNOWN IMPORTED)
    set_target_properties(MPG123::MPG123 PROPERTIES
        IMPORTED_LOCATION "${MPG123_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${MPG123_INCLUDE_DIR}"
    )
else()
    # Set to not found if either was missing
    set(MPG123_FOUND FALSE)
endif()

# Print some information in case of debugging
message(STATUS "MPG123_FOUND: ${MPG123_FOUND}")
message(STATUS "MPG123_INCLUDE_DIRS: ${MPG123_INCLUDE_DIRS}")
message(STATUS "MPG123_LIBRARIES: ${MPG123_LIBRARIES}")

# Make the variables available to the parent scope
mark_as_advanced(MPG123_INCLUDE_DIR MPG123_LIBRARY)