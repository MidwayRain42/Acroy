add_library(AcroyRenderer STATIC)

target_sources(AcroyRenderer
    PRIVATE
        Private/Renderer.cpp
        Private/Mesh.cpp
        Private/Material.cpp
        Private/SimpleMaterial.cpp
        Private/ResourceManager.cpp
        Private/Skybox.cpp
        
    PUBLIC
        Public/Renderer.hpp
        Public/Mesh.hpp
        Public/Material.hpp
        Public/SimpleMaterial.hpp
        Public/Camera.hpp
        Public/PerspectiveCamera.hpp
        Public/Transform.hpp
        Public/Light.hpp
        Public/ResourceManager.hpp
        Public/Skybox.hpp
)

target_include_directories(AcroyRenderer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(AcroyRenderer
    PUBLIC AcroyRenderContext
    PRIVATE stb_image tinyobjloader
)