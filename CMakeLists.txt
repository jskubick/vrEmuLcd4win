cmake_minimum_required(VERSION 3.21)
project(fakelcd)

set(CMAKE_CXX_STANDARD 14)

add_executable(fakelcd main.cpp VrEmuLcd.c VrEmuLcd.h)
