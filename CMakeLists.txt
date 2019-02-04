cmake_minimum_required(VERSION 3.8)
project(alghoritmsInExternalMemory)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp externalTranspose/externalTranspose.cpp)
add_executable(lab1 ${SOURCE_FILES})

set(SOURCE_FILES2 main2.cpp)
add_executable(lab2 ${SOURCE_FILES2})

set(SOURCE_FILES3 externalTranspose/externalTranspose.cpp)
add_executable(lab3 ${SOURCE_FILES3})

set(SOURCE_FILES4 externalMultiplication/externalMultiplication.cpp)
add_executable(lab4 ${SOURCE_FILES4})

set(SOURCE_FILES5 externalSort/exteranlSort.cpp)
add_executable(lab5 ${SOURCE_FILES5})

set(SOURCE_FILES6 nextAfterNext/nextAfterNext.cpp)
add_executable(lab6 ${SOURCE_FILES6})

set(SOURCE_FILES6 krTask1/task1.cpp)
add_executable(krTask1 ${SOURCE_FILES6})

set(SOURCE_FILES7 bloomFilter/bloom.cpp)
add_executable(bloom ${SOURCE_FILES7})

set(SOURCE_FILES8 dominantElements/dominantElements.cpp)
add_executable(dominantElements ${SOURCE_FILES8})

set(SOURCE_FILES9 stringAlgorithms/main3.cpp)
add_executable(stringAlgorithms ${SOURCE_FILES9})