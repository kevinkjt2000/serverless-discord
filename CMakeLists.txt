include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.295.tar.gz"
    SHA1 "8a3447594fa5948cc7c6888dd86a9e823259c69f"
)

cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 17)
project(serverless-discord LANGUAGES CXX)

add_executable(${PROJECT_NAME}
  src/ed25519.cpp
  src/handler.cpp
  src/main.cpp
  src/utils.cpp
)
target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_17")
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra")

hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)

hunter_add_package(iroha-ed25519)
find_package(ed25519 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC iroha::ed25519)

hunter_add_package(fmt)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)

set(HUNTER_aws_lambda_cpp_VERSION 0.2.7)
hunter_add_package(aws_lambda_cpp)
find_package(aws-lambda-runtime)
target_link_libraries(${PROJECT_NAME} PRIVATE AWS::aws-lambda-runtime)
aws_lambda_package_target(${PROJECT_NAME})





Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.4)
FetchContent_MakeAvailable(Catch2)
add_executable(tests
  src/ed25519.cpp 
  src/handler.cpp
  src/utils.cpp 
  tests/main.cpp
  tests/test_ed25519.cpp 
  tests/test_handler.cpp 
)
target_link_libraries(tests PRIVATE AWS::aws-lambda-runtime)
target_link_libraries(tests PRIVATE Catch2::Catch2)
target_link_libraries(tests PUBLIC iroha::ed25519)
target_link_libraries(tests PUBLIC fmt::fmt)
