cmake_minimum_required(VERSION 3.21)
project(fakelcd)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(fakelcd main.cpp VrEmuLcd.c VrEmuLcd.h EmulatedLcdWindow.cpp EmulatedLcdWindow.h formatter/DisplayManager.cpp formatter/DisplayManager.h formatter/BaseFrame.cpp formatter/BaseFrame.h formatter/StringFrame.cpp formatter/StringFrame.h formatter/MultiStringFrame.cpp formatter/MultiStringFrame.h formatter/SmashedDecimal.cpp formatter/SmashedDecimal.h formatter/MarqueeFrame.cpp formatter/MarqueeFrame.h formatter/Constants.h formatter/CustomCharManager.cpp formatter/CustomCharManager.h formatter/BaseNumberRenderer.cpp formatter/BaseNumberRenderer.h formatter/NumberRenderer.cpp formatter/NumberRenderer.h formatter/NumberFrame.cpp formatter/NumberFrame.h formatter/TimeCounter5charFrame.cpp formatter/TimeCounter5charFrame.h formatter/HexFrame.cpp formatter/HexFrame.h formatter/Timer5charFrame.cpp formatter/Timer5charFrame.h formatter/TimerFrame.cpp formatter/TimerFrame.h formatter/ScaledTimekeeper.cpp formatter/ScaledTimekeeper.h formatter/Timekeeper.cpp formatter/Timekeeper.h formatter/MmSsFrame.cpp formatter/MmSsFrame.h)

target_link_libraries(fakelcd PRIVATE sfml-graphics)
target_compile_features(fakelcd PRIVATE cxx_std_17)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET fakelcd POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:fakelcd> $<TARGET_FILE_DIR:fakelcd> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS fakelcd)
