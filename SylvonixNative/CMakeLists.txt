cmake_minimum_required(VERSION 3.10.0)
project(SylvonixNative VERSION 0.1.0 LANGUAGES C CXX)

set(SYLVONIX_NATIVE_SOURCES

    # Entry
    main.cpp
    Define.cpp

    # Core
    src/Application.cpp
    src/Device.cpp
    src/Window.cpp
    src/SwapChain.cpp
    src/Image.cpp
    src/Shader.cpp
    src/ComputePipeline.cpp
    src/Buffer.cpp
)

add_executable(
    SylvonixNative
    ${SYLVONIX_NATIVE_SOURCES}
)

target_include_directories(SylvonixNative PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(SylvonixNative PRIVATE
    C:/Dev/SDL3-3.4.2/include
)

target_link_libraries(SylvonixNative PRIVATE
    C:/Dev/SDL3-3.4.2/lib/x64/SDL3.lib
)

set_property(TARGET SylvonixNative PROPERTY CXX_STANDARD 20)
target_compile_definitions(SylvonixNative PRIVATE CMAKE_TARGET_EXPORTS)

# Copy SDL3.dll next to the output
add_custom_command(TARGET SylvonixNative POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Dev/SDL3-3.4.2/lib/x64/SDL3.dll"
        "$<TARGET_FILE_DIR:SylvonixNative>/SDL3.dll"
)

# --- VULKAN SDK ---
find_package(Vulkan REQUIRED)
target_link_libraries(SylvonixNative PRIVATE Vulkan::Vulkan)

# --- VOLK (header-only, but needs VOLK_IMPLEMENTATION in one .cpp) ---
target_include_directories(SylvonixNative PRIVATE
    C:/Dev/vulkanengine/volk-1.4.350
)

# --- VMA (header-only, but needs VMA_IMPLEMENTATION in one .cpp) ---
target_include_directories(SylvonixNative PRIVATE
    C:/Dev/VulkanMemoryAllocator-3.4.0/include
)

target_include_directories(SylvonixNative PRIVATE
    C:/Dev/vulkanengine/stb-master
)

# --- tinyobjloader ---
target_include_directories(SylvonixNative PRIVATE
    C:/Dev/vulkanengine/modelLoader
)

target_include_directories(SylvonixNative PRIVATE
    "C:/Program Files/KTX-Software/include"
)

target_include_directories(SylvonixNative PRIVATE
    C:/Dev/vulkanengine/glm
)

find_library(KTX_LIBRARY
    NAMES ktx
    HINTS "C:/Program Files/KTX-Software/lib"
    REQUIRED
)

target_include_directories(SylvonixNative PRIVATE
    "C:/Program Files/KTX-Software/include"
)

target_link_libraries(SylvonixNative PRIVATE ${KTX_LIBRARY})


# --- Slang from Vulkan SDK ---
find_library(Slang_LIBRARY
    NAMES slang slang-compiler
    HINTS "$ENV{VULKAN_SDK}/lib"
    REQUIRED
)

target_include_directories(SylvonixNative PRIVATE
    "$ENV{VULKAN_SDK}/include"
)

target_link_libraries(SylvonixNative PRIVATE ${Slang_LIBRARY})

target_compile_definitions(SylvonixNative PRIVATE
    VK_NO_PROTOTYPES
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SylvonixNative PROPERTY CXX_STANDARD 20)
endif()