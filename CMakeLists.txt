cmake_minimum_required(VERSION 3.20)

project(breakout_ogre_sln CXX)

# Ogre CMakeLists.txt file will build its dependencies at 'cmake configure time', which means,
# configuring it for the first time will take a few minutes.
add_subdirectory("external/ogre")

add_executable(breakout_ogre
    "src/main.cpp"
    "src/ball.cpp"
    "src/brick.cpp"
    "src/player.cpp"
    "src/ball.h"
    "src/brick.h"
    "src/player.h"
)

set_property(TARGET breakout_ogre PROPERTY CXX_STANDARD 20)

# we are using ogre headers from the install directory, so you need to build and install ogre
# targets before building breakout_ogre.
target_include_directories(breakout_ogre PRIVATE 
    "${CMAKE_INSTALL_PREFIX}/include/OGRE"
    "${CMAKE_INSTALL_PREFIX}/include/OGRE/Bites"
    "${CMAKE_INSTALL_PREFIX}/include/OGRE/RTShaderSystem"
)
target_link_libraries(breakout_ogre PRIVATE
    OgreBites
)

# installing all build files
install(TARGETS breakout_ogre
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT breakout_ogre_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT breakout_ogre_Runtime
        NAMELINK_COMPONENT breakout_ogre_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT breakout_ogre_Development
)

# installing assets folder
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

# When executing the exe from visual studio, we the install directory as the working directory,
# this is to find the asset folder. We need to run the install target before hand se the asset
# folder is placed there.
set_target_properties( breakout_ogre
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
)