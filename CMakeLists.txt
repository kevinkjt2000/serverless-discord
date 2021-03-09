include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.295.tar.gz"
    SHA1 "8a3447594fa5948cc7c6888dd86a9e823259c69f"
)

cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 17)
project(serverless-discord LANGUAGES CXX)

add_executable(${PROJECT_NAME} "main.cpp")

hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)

set(HUNTER_aws_lambda_cpp_VERSION 0.2.7)
hunter_add_package(aws_lambda_cpp)
find_package(aws-lambda-runtime)
target_link_libraries(${PROJECT_NAME} PRIVATE AWS::aws-lambda-runtime)
target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_17")
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra")

aws_lambda_package_target(${PROJECT_NAME})
