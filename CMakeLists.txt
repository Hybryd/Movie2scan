cmake_minimum_required(VERSION 2.6.2)
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER 2.6)
  if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER 2.8.3)
    cmake_policy(VERSION 2.8.4)
  else()
    cmake_policy(VERSION 2.6)
  endif()
endif()


set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )
 
if ( COMMAND cmake_policy )

  cmake_policy( SET CMP0003 NEW )  

endif()

project( movie2scan )


find_package( OpenCV REQUIRED )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations" )

# Executables target folder
set( EXECUTABLE_OUTPUT_PATH  ./bin )


# List all common sources
file( GLOB allsrc ./src/*.cpp )

file( GLOB main_movie2scan movie2scan/*.cpp )
add_executable( movie2scan ${main_movie2scan} ${allsrc})
target_link_libraries( movie2scan ${OpenCV_LIBS}  )

