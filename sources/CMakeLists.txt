project(DataGenerator)
cmake_minimum_required(VERSION 3.22)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost REQUIRED COMPONENTS program_options) 

if(Boost_FOUND)
    add_executable(app main.cpp)
    set(CMAKE_BUILD_TYPE Debug)
    #add necessary directories
    include_directories(app PUBLIC ${Boost_INCLUDE_DIRS}) 
    include_directories(§{PROJECT_SOURCE_DIR}/Connectors)
    include_directories(§{PROJECT_SOURCE_DIR}/dataGenerator)
    include_directories("/home/linuxbrew/.linuxbrew/Cellar/libpqxx/7.7.3/include")

    #append all libraries
    list(APPEND EXTRA_LIBRARIES ${Boost_LIBRARIES})
    list(APPEND EXTRA_LIBRARIES pq)
    list(APPEND EXTRA_LIBRARIES "/home/linuxbrew/.linuxbrew/Cellar/libpqxx/7.7.3/lib/libpqxx.so")

    target_link_libraries(app ${EXTRA_LIBRARIES})
endif()

# add the executable
