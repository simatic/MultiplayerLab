cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)

include(FetchContent)
project(2M3 VERSION 1.0.0 LANGUAGES CXX)

set (BUILD_SHARED_LIBS FALSE)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1
)

FetchContent_MakeAvailable(sfml)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("Include" "Include/Common" "Include/Common/Components" "Include/Common/Managers" "Include/Common/Systems")
file(GLOB SOURCES CONFIGURE_DEPENDS Include/*.h Include/*.inl Include/Common/*.h Include/Common/Components/*.h Include/Common/Managers/*.h Include/Common/Systems/*.h Source/*.cpp Source/Common/*.h Source/Common/Components/*.h Source/Common/Managers/*.h Source/Common/Systems/*.h)

if (MSVC)
    # warning level 4 (see https://docs.microsoft.com/fr-fr/cpp/build/reference/compiler-option-warning-level?view=vs-2019)
    # We do not put /WX to consider all warnings as errors
    add_compile_options(/W4)
else()
    # lots of warnings 
    # We do not put -Werror to consider all warnings as errors
    add_compile_options(-g -Wall -Wextra)
endif()

add_executable(2M3
  ${SOURCES}
    "Include/Common/Components/Component.h" "Source/Common/Components/Component.cpp" "Include/Common/Components/Transform.h" "Source/Common/Components/Transform.cpp" "Include/Common/Components/Collider.h" "Source/Common/Components/Collider.cpp" "Include/Common/Systems/CollisionSystem.h" "Include/Common/Components/Sprite.h" "Source/Common/Systems/RenderSystem.cpp" "Source/Common/Components/Sprite.cpp" "Source/Common/Systems/CollisionSystem.cpp" "Include/Common/Systems/MovementSystem.h" "Source/Common/Systems/MovementSystem.cpp" "Include/Common/Components/Bullet.h" "Source/Common/Components/Bullet.cpp" "Include/Common/Systems/System.h" "Include/Common/Managers/Signature.h" "Source/Common/Managers/Signature.cpp" "Include/Common/Components/CarEngine.h" "Source/Common/Components/CarEngine.cpp" "Include/Common/Components/CarInput.h" "Include/Common/Systems/BulletSystem.h" "Source/Common/Systems/BulletSystem.cpp" "Include/Common/Components/Trajectory.h" "Source/Common/Components/Trajectory.cpp" "Include/Common/Systems/TrajectorySystem.h" "Source/Common/Systems/TrajectorySystem.cpp" "Include/Common/Systems/RenderTrajectorySystem.h" "Source/Common/Systems/RenderTrajectorySystem.cpp" "Include/Common/Systems/RenderParticleSystem.h" "Source/Common/Systems/RenderParticleSystem.cpp" "Include/Common/Systems/ParticleSystem.h" "Source/Common/Systems/ParticleSystem.cpp" "Include/Common/Components/Particles.h" "Source/Common/Components/Particles.cpp" "Include/Common/Components/Health.h" "Source/Common/Components/Health.cpp" "Include/Common/Components/HealthBar.h" "Source/Common/Components/HealthBar.cpp" "Include/Common/Systems/KeyboardInputSystem.h" "Source/Common/Systems/KeyboardInputSystem.cpp" "Include/Common/Components/Kinematics.h" "Include/Common/Systems/CarMovementSystem.h" "Source/Common/Systems/CarMovementSystem.cpp" "Include/Common/Systems/GunSystem.h" "Source/Common/Systems/GunSystem.cpp" "Include/Common/Components/Gun.h" "Source/Common/Components/Gun.cpp" "Include/Common/Systems/CarCollisionHandling.h" "Source/Common/Systems/CarCollisionHandling.cpp" "Source/Common/Systems/CarDeath.cpp")

target_link_libraries(2M3 PUBLIC sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

add_custom_target(copy-Media ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Media)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Media
                   DEPENDS ${CMAKE_SOURCE_DIR}/Media
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${CMAKE_SOURCE_DIR}/Media
                           ${CMAKE_CURRENT_BINARY_DIR}/Media
                           )
add_dependencies(2M3 copy-Media)

file(GLOB_RECURSE DYNAMIC_LIBS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/lib/${ARCH}/*.dll)
file(COPY ${DYNAMIC_LIBS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
