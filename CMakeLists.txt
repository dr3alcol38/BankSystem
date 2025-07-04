cmake_minimum_required(VERSION 3.10)

project(BankSystem)

add_executable(BankSystem 
src/HelperFuncs.h
src/HelperFuncs.cpp
src/FileSystem.h
src/FileSystem.cpp
src/BankSystemMain.cpp 
src/BankAccount.h
src/BankAccount.cpp
src/UserProfile.h
src/UserProfile.cpp
src/RequestAndResponseManager.h
src/RequestAndResponseManager.cpp
)

#target_include_directories(BankSystem PRIVATE "${PROJECT_SOURCE_DIR}/dependencies/include")