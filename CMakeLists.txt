cmake_minimum_required(VERSION 3.21)
project(fakelcd)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(fakelcd main.cpp VrEmuLcd.c VrEmuLcd.h EmulatedLcdWindow.cpp EmulatedLcdWindow.h formatter/CharacterDisplay.cpp formatter/CharacterDisplay.h formatter/BaseFrame.cpp formatter/BaseFrame.h formatter/StringFrame.cpp formatter/StringFrame.h formatter/MultiStringFrame.cpp formatter/MultiStringFrame.h)

target_link_libraries(fakelcd PRIVATE sfml-graphics)
target_compile_features(fakelcd PRIVATE cxx_std_17)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET fakelcd POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:fakelcd> $<TARGET_FILE_DIR:fakelcd> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS fakelcd)
