#include "GrappleBoxShader.hpp"

#include "util/gl_compile_program.hpp"

GrappleBoxShader::GrappleBoxShader() {
    m.program = gl_compile_program(
"#version 330\n"
"in vec2 a_position;\n"
"in vec2 a_texture_coord;\n"
"in vec4 a_color;\n"
"uniform vec2 u_position;                 // from transform\n"
"uniform vec2 u_scale;                    // from transform\n"
"uniform float u_rotation;                // from transform\n"
"uniform vec2 u_camera_position;          // from camera\n"
"uniform vec2 u_camera_viewport_size;     // from camera\n"
"uniform float u_camera_pixels_per_unit;  // from camera\n"
"out vec2 texture_coord;\n"
"out vec4 color;\n"
"        \n"
"void main() {\n"
"    mat2 rotation_matrix =\n"
"    mat2(cos(u_rotation),-sin(u_rotation),\n"
"        sin(u_rotation),cos(u_rotation));\n"
"    vec2 world_position = rotation_matrix * (a_position * u_scale) + u_position;\n"
"    vec2 camera_space_position = (world_position - u_camera_position) * u_camera_pixels_per_unit;\n"
"    vec2 clip_space_position = (camera_space_position / u_camera_viewport_size) * 2.0;\n"
"    gl_Position = vec4(clip_space_position, 0.0, 1.0);\n"
"    texture_coord = a_texture_coord;\n"
"    color = a_color;\n"
"}\n"
"\n",
"#version 330\n"
"in vec2 texture_coord;\n"
"in vec4 color;\n"
"in vec2 s_position;\n"
"uniform sampler2D u_frag_texture;\n"
"uniform float u_time; \n"
"uniform bool u_frag_use_texture;\n"
"out vec4 FragColor;\n"
"\n"
"vec3 palette(float t) {    \n"
"    vec3 a = vec3(0.508, 0.500, 0.508);    \n"
"    vec3 b = vec3(-0.482, 0.478, 0.418);   \n"
"    vec3 c = vec3(0.558, 1.000, 1.000);    \n"
"    vec3 d = vec3(0.138, -0.592, 0.667);   \n"
"    return a + b*cos( 6.283185*(c*t+d));   \n"
"}  \n"
"   \n"
"float sdBox( in vec2 p, in vec2 b )    \n"
"{  \n"
"    vec2 d = abs(p)-b; \n"
"    return length(max(d,0.0)) + min(max(d.x,d.y),0.0); \n"
"}  \n"
"\n"
"void main()    \n"
"{  \n"
"    // Normalized pixel coordinates (from 0 to 1)  \n"
"    vec2 uv = 2.0*texture_coord - 1.0; \n"
"    \n"
"    float d = sdBox(uv, vec2(1.0, 1.0));   \n"
"    d = -d;    \n"
"    \n"
"    vec3 normal = vec3(0.0);   \n"
"    vec3 lightPos = vec3(0.5, 0.5, 0.5);   \n"
"    \n"
"    float extent = 0.25;   \n"
"    float z = max(extent * d, 0.0);    \n"
"    vec3 lightRay = normalize(vec3(gl_FragCoord.xy, z) - vec3(0.5 , 0.5, 1.0)); \n"
"    \n"
"    float top = float(uv.x < uv.y)*float(uv.x > -uv.y);    \n"
"    float left = float(uv.x < uv.y)*float(uv.x < -uv.y);   \n"
"    float right = float(uv.x > uv.y)*float(uv.x > -uv.y);  \n"
"    float bot = float(uv.x > uv.y)*float(uv.x < -uv.y);    \n"
"    \n"
"    vec3 topNorm = vec3(0.0, 0.5, 0.5);    \n"
"    vec3 botNorm = vec3(0.0, -0.5, 0.5);   \n"
"    vec3 leftNorm = vec3(-0.5, 0.0, 0.5);  \n"
"    vec3 rightNorm = vec3(0.5, 0.0, 0.5);  \n"
"    \n"
"    vec3 norm = normalize(top*topNorm + bot*botNorm + left*leftNorm + right*rightNorm);    \n"
"    \n"
"    float lighting = max(0.0, dot(lightRay, norm));    \n"
"    lighting *= 0.7;   \n"
"    lighting += 0.2;   \n"
"    \n"
"    float M_PI = 3.14159265358;    \n"
"    \n"
"    float b = d;   \n"
"    //light rings  \n"
"    d = (0.4*d - 0.1*(length(uv)/0.5));    \n"
"    float angle = atan(uv.y, uv.x) + M_PI; \n"
"    \n"
"    float off = (sin(angle*1.0 + u_time)/80.0)*(cos(20.0*angle + 5.0 + \n"
"    5.0*u_time)/2.0); \n"
"    d+= off;   \n"
"    \n"
"    float off2 = (sin(3.5*angle*1.0 + u_time)/80.0)*(cos(12.0*angle + 5.0 \n"
"    + 5.0*u_time)/2.0);    \n"
"    b += off2; \n"
"    b = smoothstep(0.0, 0.2, b);   \n"
"    b = 0.1 / b;   \n"
"    \n"
"    d = sin(1.0*d + 2.0*u_time)/50.0;;  \n"
"    d = abs(d);    \n"
"    d = smoothstep(0.0, 0.03, d);  \n"
"    d = 0.125 / d; \n"
"    \n"
"    float highlight = d + b;   \n"
"    vec3 highlight_col = palette(length(uv))*highlight;    \n"
"    \n"
"    vec3 baseColor = vec3(0.4, 0.8, 0.9);  \n"
"    float alpha = 1.0;"
"    if (u_frag_use_texture) {  \n"
"        vec4 tex_color = texture(u_frag_texture, texture_coord);\n"
"        baseColor = tex_color.xyz;    \n"
"        alpha = tex_color.w;\n"
"    } else {   \n"
"        baseColor = color.xyz; \n"
"    }  \n"
"    \n"
"    vec3 col = vec3(lighting)*baseColor + highlight_col*0.8;   \n"
"   \n"
"    // Output to screen    \n"
"    FragColor = vec4(col, 1.0);\n"
"}\n");

    m.a_position_loc = glGetAttribLocation(m.program, "a_position");
    m.a_texture_coord_loc = glGetAttribLocation(m.program, "a_texture_coord");
    m.a_color_loc = glGetAttribLocation(m.program, "a_color");

    m.u_position_loc = glGetUniformLocation(m.program, "u_position");
    m.u_scale_loc = glGetUniformLocation(m.program, "u_scale");
    m.u_rotation_loc = glGetUniformLocation(m.program, "u_rotation");
    
    m.u_camera_position_loc = glGetUniformLocation(m.program, "u_camera_position");
    m.u_camera_viewport_size_loc = glGetUniformLocation(m.program, "u_camera_viewport_size");
    m.u_camera_pixels_per_unit_loc = glGetUniformLocation(m.program, "u_camera_pixels_per_unit");

    m.u_frag_texture_loc = glGetUniformLocation(m.program, "u_frag_texture");
    m.u_frag_use_texture_loc = glGetUniformLocation(m.program, "u_frag_use_texture");
    m.u_time_loc = glGetUniformLocation(m.program, "u_time");
}

GrappleBoxShader::~GrappleBoxShader() { glDeleteProgram(m.program); }

GrappleBoxShader::GrappleBoxShader(GrappleBoxShader&& other) noexcept
    : m(other.m) {
    other.m.program = 0;
}

GrappleBoxShader& GrappleBoxShader::operator=(
    GrappleBoxShader&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    // Delete current program
    glDeleteProgram(m.program);

    // Move from other
    m = std::move(other.m);

    // Reset other
    other.m.program = 0;

    return *this;
}

void GrappleBoxShader::updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale, float u_rotation, 
                        glm::vec2 camera_pos, glm::vec2 camera_viewport_size, float camera_pixels_per_unit,
                        int frag_use_texture, GLuint texture, float u_time) {
        // Uniforms
        // -- Vertex shader
        glUniform2f(m.u_position_loc, user_pos.x,
                    user_pos.y);
        glUniform2f(m.u_scale_loc, u_scale.x,
                    u_scale.y);
        glUniform1f(m.u_rotation_loc, u_rotation);
        glUniform2f(m.u_camera_position_loc,
                    camera_pos.x, camera_pos.y);
        glUniform2f(m.u_camera_viewport_size_loc,
                    camera_viewport_size.x, camera_viewport_size.y);
        glUniform1f(m.u_camera_pixels_per_unit_loc,
                    camera_pixels_per_unit);
        // -- Fragment shader
        glUniform1i(m.u_frag_use_texture_loc, frag_use_texture);

        // Texture
        if (frag_use_texture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(m.u_frag_texture_loc, 0);
        }
        glUniform1f(m.u_time_loc, u_time);
}