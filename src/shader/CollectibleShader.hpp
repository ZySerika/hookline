/**
 * @file CollectibleShader.hpp
 *
 * GL Shader program for 2D meshes with per-vertex color, or a single texture.
 */

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct CollectibleShader {
    CollectibleShader();
    ~CollectibleShader();

    // Cannot copy a shader program
    CollectibleShader(const CollectibleShader& other) = delete;
    CollectibleShader& operator=(const CollectibleShader& other) = delete;

    CollectibleShader(CollectibleShader&& other) noexcept;

    CollectibleShader& operator=(CollectibleShader&& other) noexcept;

    struct {
        GLuint program = 0;

        // Attribute (per-vertex variable) locations
        GLuint a_position_loc = 0;
        GLuint a_texture_coord_loc = 0;
        GLuint a_color_loc = 0;

        // Uniform locations
        GLuint u_position_loc = 0;
        GLuint u_scale_loc = 0;
        GLuint u_rotation_loc = 0;
        GLuint u_camera_position_loc = 0;
        GLuint u_camera_viewport_size_loc = 0;
        GLuint u_camera_pixels_per_unit_loc = 0;
        //   -- Fragment shader
        GLuint u_time_loc = 0;
        GLuint u_drawable_size_loc = 0;
        GLuint u_size_ratio_loc = 0;
    } m;

    //call after binding vertex arrays
    void updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale, float u_rotation, 
                        glm::vec2 camera_pos, glm::vec2 camera_viewport_size, float camera_pixels_per_unit,
                        glm::vec2 drawable_size, float u_time, float size_ratio);
};