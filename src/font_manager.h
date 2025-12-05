#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>
#include <map>

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Character {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

class FontManager : protected QOpenGLFunctions_3_3_Core {
public:
    FontManager();
    ~FontManager();

    bool loadFont(const std::string& font_path, FT_UInt size);
    const Character& getCharacter(char32_t ch) const;
    GLuint getAtlasTextureId() const;

private:
    FT_Library ft;
    FT_Face face;
    GLuint atlas_texture_id;
    std::map<char32_t, Character> characters;
};

#endif // FONT_MANAGER_H
