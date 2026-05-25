add_library(Acroy STATIC)

add_subdirectory(Source/Renderer)
add_subdirectory(Source/Core)
add_subdirectory(Source/RHI)
add_subdirectory(Source/Resources)

target_link_libraries(Acroy PUBLIC AcroyCore AcroyRHI AcroyRenderer AcroyResources)


find_program(GLSLANG glslangValidator REQUIRED)

# Shader compile function
function(compile_glsl SHADER STAGE OUT_FILE)

    add_custom_command(
        OUTPUT ${OUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Shaders
        COMMAND ${GLSLANG}
            -V
            -S ${STAGE}
            --target-env opengl
            -e main
            ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}
            -o ${OUT_FILE}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}
        COMMENT "Compiling ${SHADER} -> ${OUT_FILE}"
        VERBATIM
    )

endfunction()

# Shader definitions
set(VERT_SRC Shaders/SimpleVert.glsl)
set(FRAG_SRC Shaders/SimpleFrag.glsl)

set(VERT_SPV ${CMAKE_BINARY_DIR}/Shaders/vertex.spv)
set(FRAG_SPV ${CMAKE_BINARY_DIR}/Shaders/fragment.spv)

compile_glsl(${VERT_SRC} vert ${VERT_SPV})
compile_glsl(${FRAG_SRC} frag ${FRAG_SPV})


# Shader build target
add_custom_target(Shaders ALL
    DEPENDS
        ${VERT_SPV}
        ${FRAG_SPV}
)

add_dependencies(Acroy Shaders)