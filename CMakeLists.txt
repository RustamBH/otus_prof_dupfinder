cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

#--------------------------------------------------------------------------------------------------
# variables
set(PROJECT_NAME dupfinder)
#--------------------------------------------------------------------------------------------------
# project
project(${PROJECT_NAME} VERSION ${PROJECT_VESRION})

#--------------------------------------------------------------------------------------------------
# builds
add_executable(${PROJECT_NAME}
	Config.cpp
	Config.h
	DupFinder.cpp
	DupFinder.h
 	FileClass.cpp
	FileClass.h
	main.cpp
	vectormap.h
)

# properties
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

# boost
#
SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS program_options filesystem regex)
if(Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

#--------------------------------------------------------------------------------------------------
# deb pkg
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")	
	
set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
