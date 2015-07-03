cmake_minimum_required(VERSION 2.6)
project("MeshRepair")
			
			
include_directories ("${PROJECT_SOURCE_DIR}")


set(EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}")
if(NOT EIGEN3_INCLUDE_DIR)
    message(FATAL_ERROR "Please set the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
endif()
include_directories ( "${EIGEN3_INCLUDE_DIR}" )


set(GLM_INCLUDE_DIR "$ENV{GLM_INCLUDE_DIR}")
if(NOT GLM_INCLUDE_DIR)
    message(FATAL_ERROR "Please set the environment variable GLM_INCLUDE_DIR to the include directory of your GLM installation.")
endif()
include_directories ( "${GLM_INCLUDE_DIR}" )


add_executable(MeshRepair 	main.cpp 
							bbox.cpp 
							geomtests.cpp 
							gridinterpolator.cpp 
							gridinterpolatortrilinear.cpp
							holerepairer.cpp
							multigridsolver.cpp
							trianglemesh.cpp
							MC/IJK.cpp
							MC/InfoVoxel.cpp
							MC/Label.cpp
							MC/MarchingCubes.cpp
							MC/MC_Patrons.cpp
							MC/Voxel048.cpp
			  )