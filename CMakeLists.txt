#---------------------------------------------------#
#          CMake Project Configuration              #
#---------------------------------------------------#
# CMake will issue a warning if you don't set this
cmake_minimum_required( VERSION 2.8 )

# If you don't name the project, it will take the default name "Project"
# Note: this must be set in the beginning of the CMakeLists file
project("rbfSpaceDeformation")

#---------------------------------------------------#
#              OpenCV Configuration                 #
#---------------------------------------------------#
# Automatically find OpenCV files
find_package( OpenCV REQUIRED )

#---------------------------------------------------#
#              Project Files                        #
#---------------------------------------------------#
# List source files in a variable so we can use it later more conveniently
set( LDW_SRCS
	main.cpp
	rbf.cpp
	rbf.h
	MultiplatformTimer.h
)

#---------------------------------------------------#
#               Project Configuration               #
#---------------------------------------------------#
# Add all sources defined above to the program
link_directories( ${CMAKE_BINARY_DIR}/bin )
add_executable( ${PROJECT_NAME} ${LDW_SRCS} )

# Link libraries that are required for building this application
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

IF( BUILD_TESTS )
ADD_TEST( test_${THIS_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/${THIS_PROJECT_NAME} --test )
ENDIF( BUILD_TESTS )

SET_TARGET_PROPERTIES(${THIS_PROJECT_NAME} PROPERTIES FOLDER "testingApp")
