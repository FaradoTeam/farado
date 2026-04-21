project(farado-vault-server VERSION 1.0.0)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cpprestsdk REQUIRED)
find_package(jwt-cpp REQUIRED)

set(SERVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api/rest_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api/middleware/auth_middleware.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api/handlers/items_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api/handlers/auth_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(SERVER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/application.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api/rest_server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api/middleware/auth_middleware.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api/handlers/items_handler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api/handlers/auth_handler.h
)

set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/log/log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/config/config.cpp
)

set(COMMON_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/log/log.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/config/config.h
)

add_executable(
    ${PROJECT_NAME}
    ${SERVER_SOURCES}
    ${SERVER_HEADERS}
    ${COMMON_SOURCES}
    ${COMMON_HEADERS}
)

target_link_libraries(
    ${PROJECT_NAME}
    boost::boost
    nlohmann_json::nlohmann_json
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    cpprestsdk::cpprest
    jwt-cpp::jwt-cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
