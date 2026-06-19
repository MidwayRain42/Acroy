add_library(AcroyRenderContext STATIC)

target_sources(AcroyRenderContext
    PRIVATE
        Private/RenderContext.cpp
        Private/Texture.cpp
        Private/FrameBuffer.cpp
        Private/Shader.cpp
        Private/Buffer.cpp
        Private/VertexArray.cpp

    PUBLIC
        Public/RenderContext.hpp
        Public/Texture.hpp
        Public/FrameBuffer.hpp
        Public/Shader.hpp
        Public/Buffer.hpp
        Public/VertexArray.hpp
)

target_include_directories(AcroyRenderContext
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(AcroyRenderContext
    PUBLIC  AcroyCore
    PRIVATE glad
)
