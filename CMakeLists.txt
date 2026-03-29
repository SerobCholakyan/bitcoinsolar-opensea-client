cmake_minimum_required(VERSION 3.15)
project(bitcoinsolar_opensea_client CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)

include_directories(
    ${CURL_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party
)

add_executable(bitcoinsolar_opensea_client
    src/main.cpp
    src/OpenSeaClient.cpp
    src/Gallery.cpp
    src/Metadata.cpp
    src/Rarity.cpp
    src/RarityEngine.cpp
    src/Marketplace.cpp
    src/MintingDashboard.cpp
)

target_link_libraries(bitcoinsolar_opensea_client
    ${CURL_LIBRARIES}
)
