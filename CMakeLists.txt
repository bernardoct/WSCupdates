cmake_minimum_required(VERSION 3.14)
project(WSCupdates)

set(CMAKE_CXX_STANDARD 14)

include_directories(GorelickModel)

add_executable(WSCupdates
        GorelickModel/borgProblemDefinition.h
        GorelickModel/calculations.cpp
        GorelickModel/Dates.cpp
        GorelickModel/Dates.h
        GorelickModel/global.h
        GorelickModel/InsuranceFund.cpp
        GorelickModel/InsuranceFund.h
        GorelickModel/moeaframework.cpp
        GorelickModel/moeaframework.h
        GorelickModel/ReservoirStorage.cpp
        GorelickModel/ReservoirStorage.h
        GorelickModel/Simulation.cpp
        GorelickModel/Simulation.h
        GorelickModel/TimeSeriesData.cpp
        GorelickModel/TimeSeriesData.h
        GorelickModel/triangleSimulation.cpp
        GorelickModel/utility.cpp
        GorelickModel/WaterUtility.cpp
        GorelickModel/WaterUtility.h)
