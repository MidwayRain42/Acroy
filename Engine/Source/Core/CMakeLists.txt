add_library(AcroyCore STATIC)

target_sources(AcroyCore
    PRIVATE
        Private/Engine.cpp
        Private/Window.cpp
        Private/Input.cpp
        
    PUBLIC
        Public/Engine.hpp
        Public/Window.hpp
        Public/Types.hpp
        Public/Layer.hpp
        Public/Events.hpp
        Public/Input.hpp
)

target_include_directories(AcroyCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(AcroyCore
    PUBLIC
        AcroyRenderer AcroyResources
    
    PRIVATE
        glfw
)