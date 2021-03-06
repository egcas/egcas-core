cmake_minimum_required(VERSION 2.8.11)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_BUILD_TYPE Release)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions(-std=c++11)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#        set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
        add_definitions(-std=c++11)
endif()

# Widgets finds its own dependencies.
find_package(Qt5 COMPONENTS Widgets Core Test Multimedia)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/view)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/structural)


file(GLOB_RECURSE tst_egcastest_structural_concrete_SOURCES "../../src/structural/concreteNodes/*.cpp")

set(tst_egcastest_structural_SOURCES
        tst_egcastest_structural.cpp 
        ../../src/structural/specialNodes/egcunarynode.cpp
        ../../src/structural/specialNodes/egcbinarynode.cpp
        ../../src/structural/specialNodes/egcflexnode.cpp
        ../../src/structural/specialNodes/egcnode.cpp
        ../../src/structural/specialNodes/egcbinaryoperator.cpp
        ../../src/structural/egcnodecreator.cpp
        ../../src/structural/entities/egcentity.cpp
        ${tst_egcastest_structural_concrete_SOURCES}
        ../../src/structural/specialNodes/egccontainernode.cpp
        ../../src/structural/iterator/egcnodeiterator.cpp
        ../../src/structural/entities/egcformulaentity.cpp
        ../../src/structural/specialNodes/egcbasenode.cpp
        ../../src/structural/specialNodes/egcemptynode.cpp
        ../../src/structural/specialNodes/egcargumentsnode.cpp
        ../../src/structural/visitor/egcnodevisitor.cpp 
        ../../src/structural/visitor/egcmaximavisitor.cpp 
        ../../src/structural/visitor/egcmathmlvisitor.cpp
        ../../src/structural/visitor/egcmathmllookup.cpp
        ../../src/structural/entities/formulamodificator.cpp
        ../../src/structural/iterator/formulascriter.cpp
        ../../src/structural/visitor/formulascrvisitor.cpp
        ../../src/structural/visitor/visitorhelper.cpp
        ../../src/structural/visitor/formulascrelement.cpp
        ../../src/utils/egcutfcodepoint.cpp
)

add_executable(tst_egcastest_structural ${tst_egcastest_structural_SOURCES} )
add_dependencies(tst_egcastest_structural egcas)

target_link_libraries(tst_egcastest_structural mmlegcas Qt5::Widgets Qt5::Core Qt5::Test  Qt5::Multimedia)
