cmake_minimum_required(VERSION 3.20)
project(B3_21)

set(CMAKE_CXX_STANDARD 20)

#setting up wxWidgets
if (APPLE)
    set(wxWidgets_ROOT_DIR </opt/homebrew/bin/wx-config>)
else ()
    set(wxWidgets_ROOT_DIR </usr/include/wx-3.0>)
endif ()
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

#copy necessary files
configure_file(${CMAKE_SOURCE_DIR}/backend/message_queue/message_queue_file ${CMAKE_BINARY_DIR} COPYONLY)
file(COPY ${CMAKE_SOURCE_DIR}/frontend/images DESTINATION ${CMAKE_BINARY_DIR})

#including project files
include_directories(${PROJECT_SOURCE_DIR})
#link_directories(${PROJECT_SOURCE_DIR})


#adding executables
add_executable(B3_21 main.cpp backend/backup/backup.cpp
        backend/calc_exec/calc_exec.cpp backend/calc_exec/calc_operations.cpp backend/calc_exec/calc_math.cpp
        backend/for_society/for_society.cpp
        backend/number/number.cpp
        backend/program_buffer_exec/program.cpp
        backend/message_queue/message_queue.cpp
        backend/number_buffer/number_buffer.cpp
        frontend/visualization/visualization_class.cpp frontend/visualization/updater.cpp
        frontend/interface/definition/interface_def.cpp
        frontend/interface/frame/connection.cpp frontend/interface/frame/frame.cpp)

target_link_libraries(B3_21 PRIVATE ${wxWidgets_LIBRARIES})