add_library(AcroyRHI STATIC)

target_sources(AcroyRHI
    PRIVATE
        Private/RHI.cpp
        Private/Texture.cpp
        Private/FrameBuffer.cpp
        Private/Shader.cpp
        Private/GraphicsPipeline.cpp
        Private/Buffer.cpp

    PUBLIC
        Public/RHI.hpp
        Public/Texture.hpp
        Public/FrameBuffer.hpp
        Public/Shader.hpp
        Public/GraphicsPipeline.hpp
        Public/Buffer.hpp
)

target_include_directories(AcroyRHI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(AcroyRHI
    PUBLIC
        AcroyCore

    PRIVATE
        glad
)
