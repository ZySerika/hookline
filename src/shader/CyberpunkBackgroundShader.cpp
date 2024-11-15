#include "CyberpunkBackgroundShader.hpp"

#include "util/gl_compile_program.hpp"

CyberpunkBackgroundShader::CyberpunkBackgroundShader() {
    m.program = gl_compile_program(
        // vertex shader
        "#version 330\n"
        "in vec2 a_position;\n"
        "void main() {\n"
        "   gl_Position = vec4(a_position, 0.0, 1.0);\n"
        "}\n",
        // ----- Fragment shader -----
        "#version 330\n"
        "uniform float u_time;\n"
        "uniform vec2 u_drawable_size;\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   vec2 norm_coords = gl_FragCoord.xy / u_drawable_size;\n"
        // Strobe
        "   float strobe = sin(u_time / 100.0) * 0.25 + 0.5;\n"  // 0 - 1
        // Color mix
        "   vec4 cyan = vec4(0.0, 0.7, 1.0, 1.0);\n"
        "   vec4 magenta = vec4(1.0, 0.0, 0.4, 1.0);"
        "   float mix_ratio = (sin(norm_coords.y + u_time * 0.001) * 0.5 + 0.5 "
        "+"
        "                      sin(norm_coords.x + u_time * 0.001) * 0.5 + "
        "0.5) / 2.0;\n"

        "   vec4 result_color = mix(cyan, magenta, mix_ratio);\n"
        "   FragColor = result_color;\n"
        "}\n");

    m.a_position_loc = glGetAttribLocation(m.program, "a_position");
    m.u_time_loc = glGetUniformLocation(m.program, "u_time");
    m.u_drawable_size_loc = glGetUniformLocation(m.program, "u_drawable_size");
}

CyberpunkBackgroundShader::~CyberpunkBackgroundShader() {
    glDeleteProgram(m.program);
}

CyberpunkBackgroundShader::CyberpunkBackgroundShader(
    CyberpunkBackgroundShader&& other) noexcept {
    m = std::move(other.m);

    other.m.program = 0;
}

CyberpunkBackgroundShader& CyberpunkBackgroundShader::operator=(
    CyberpunkBackgroundShader&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    // Delete current program
    glDeleteProgram(m.program);

    // Move
    m = std::move(other.m);

    // Reset other
    other.m.program = 0;

    return *this;
}