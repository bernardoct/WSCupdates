cmake_minimum_required(VERSION 3.8)
project(Gorelick_Model9_8_17)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O0 -march=native")

set(SOURCE_FILES
        Final_Gorelick_Model/calculations.cpp
        Final_Gorelick_Model/Dates.cpp
        Final_Gorelick_Model/Dates.h
        Final_Gorelick_Model/global.h
        Final_Gorelick_Model/InsuranceFund.cpp
        Final_Gorelick_Model/InsuranceFund.h
        Final_Gorelick_Model/ReservoirStorage.cpp
        Final_Gorelick_Model/ReservoirStorage.h
        Final_Gorelick_Model/Simulation.cpp
        Final_Gorelick_Model/Simulation.h
        Final_Gorelick_Model/TimeSeriesData.cpp
        Final_Gorelick_Model/TimeSeriesData.h
        Final_Gorelick_Model/triangleSimulation.cpp
        Final_Gorelick_Model/utility.cpp
        Final_Gorelick_Model/WaterUtility.cpp
        Final_Gorelick_Model/WaterUtility.h)

add_executable(Gorelick_Model9_8_17 ${SOURCE_FILES})