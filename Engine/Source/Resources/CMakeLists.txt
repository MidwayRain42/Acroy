add_library(AcroyResources STATIC)

target_sources(AcroyResources
    PRIVATE
        Private/MeshFactory.cpp
        Private/ShaderLoader.cpp
        Private/TextureLoader.cpp
        Private/FileUtils.cpp
        
    PUBLIC
        Public/MeshFactory.hpp
        Public/ShaderLoader.hpp
        Public/TextureLoader.hpp
        Public/FileUtils.hpp
)

target_include_directories(AcroyResources
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(AcroyResources
    PRIVATE stb_image AcroyRenderer
)