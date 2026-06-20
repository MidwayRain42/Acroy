add_library(Acroy STATIC)

add_subdirectory(Source/Renderer)
add_subdirectory(Source/Core)
add_subdirectory(Source/RenderContext)

target_link_libraries(Acroy PUBLIC AcroyCore AcroyRenderContext AcroyRenderer)

find_program(GLSLANG glslangValidator REQUIRED)

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

# Source shaders
set(SHADER_FILES
    Simple.vert vert
    Simple.frag frag
    Skybox.vert vert
    Skybox.frag frag
)

set(SPV_OUTPUTS)

list(LENGTH SHADER_FILES SHADER_COUNT)
math(EXPR SHADER_COUNT "${SHADER_COUNT} - 1")

foreach(i RANGE 0 ${SHADER_COUNT} 2)
    list(GET SHADER_FILES ${i} SHADER)
    math(EXPR j "${i} + 1")
    list(GET SHADER_FILES ${j} STAGE)

    get_filename_component(NAME ${SHADER} NAME)
    set(OUT_FILE ${CMAKE_BINARY_DIR}/Shaders/${NAME}.spv)

    compile_glsl(
        Shaders/${SHADER}
        ${STAGE}
        ${OUT_FILE}
    )

    list(APPEND SPV_OUTPUTS ${OUT_FILE})
endforeach()

add_custom_target(Shaders ALL
    DEPENDS ${SPV_OUTPUTS}
)

add_dependencies(Acroy Shaders)