#include "font_manager.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

FontManager::FontManager() : ft(nullptr), face(nullptr), atlas_texture_id(0) {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    initializeOpenGLFunctions();
}

FontManager::~FontManager() {
    if (atlas_texture_id) {
        glDeleteTextures(1, &atlas_texture_id);
    }
    if (face) {
        FT_Done_Face(face);
    }
    if (ft) {
        FT_Done_FreeType(ft);
    }
}

bool FontManager::loadFont(const std::string& font_path, FT_UInt size) {
    if (face) {
        FT_Done_Face(face);
    }

    if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    // Create a texture atlas
    if (atlas_texture_id) {
        glDeleteTextures(1, &atlas_texture_id);
    }
    glGenTextures(1, &atlas_texture_id);
    glBindTexture(GL_TEXTURE_2D, atlas_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Pre-calculate texture atlas size
    FT_UInt atlas_width = 0;
    FT_UInt atlas_height = 0;
    for (unsigned long c = 0; c < 256; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        atlas_width += face->glyph->bitmap.width;
        atlas_height = std::max(atlas_height, face->glyph->bitmap.rows);
    }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        atlas_width,
        atlas_height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    int x = 0;
    for (unsigned long c = 0; c < 256; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }

        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            x,
            0,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        Character character = {
            (GLuint)x,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x
        };
        characters[c] = character;
        x += face->glyph->bitmap.width;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

const Character& FontManager::getCharacter(char32_t ch) const {
    return characters.at(ch);
}

GLuint FontManager::getAtlasTextureId() const {
    return atlas_texture_id;
}
