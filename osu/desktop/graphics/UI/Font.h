//
// Created by MasterLogick on 2/4/20.
//

#ifndef OSU_LASER_C_FONT_H
#define OSU_LASER_C_FONT_H

#include <platformDependentDefines.h>
#include <glad/glad.h>

#define FONT_SMOOTH 0x80
#define FONT_UNICODE 0x40
#define FONT_ITALIC 0x20
#define FONT_BOLD 0x10
#define FONT_FIXED_HEIGHT 0x8
#define FONT_PACKED 0x1
#define FONT_CHAR_PACKET_SIZE 20
#define FONT_KERNING_BLOCK_SIZE 10
namespace osu {
    unsigned int charsToInt(const char *c);

    unsigned short int charsToWord(const char *c);

    struct Info {
        int fontSize;
        uint bitfield;
        uint charset;
        uint stretchH;
        uint aa;
        uint paddingUp;
        uint paddingRight;
        uint paddingDown;
        uint paddingLeft;
        uint spacingHoriz;
        uint spacingVert;
        uint outline;
        std::string fontName;
    };

    struct Common {
        uint lineHeight;
        uint base;
        uint scaleW;
        uint scaleH;
        uint pages;
        uint bitField;
        uint alphaChnl;
        uint redChnl;
        uint greenChnl;
        uint blueChnl;
    };
    struct Char {
        wchar_t id;
        unsigned short x;
        unsigned short y;
        unsigned short width;
        unsigned short height;
        short xoffset;
        short yoffset;
        short xadvance;
        unsigned int page;
        unsigned int chnl;
    };

    struct Page {
        GLuint id;
        int width;
        int height;
    };
    struct Kerning {
        wchar_t first;
        wchar_t second;
        signed short size;
    };

    class Font {
        //todo create defaultSize method
    private:
        Info info;
        Common common;
        Char *chars;
        Page *pages;
        unsigned int charactersCount;

        std::string *pageNames;
        Kerning *kernings;
        unsigned char kerningPairsAmount;
        GLuint vao;
        GLuint vbo;

        void loadTextures();

        Char *binSearch(wchar_t id);

        Kerning *getKerning(wchar_t i, wchar_t i1);
    public:
        static Font *Exo2_0_Black;
#ifdef COMPILE_WITH_ALL_FONTS
        static Font *Exo2_0_BlackItalic;
        static Font *Exo2_0_Bold;
        static Font *Exo2_0_BoldItalic;
        static Font *Exo2_0_Light;
        static Font *Exo2_0_LightItalic;
        static Font *Exo2_0_Medium;
        static Font *Exo2_0_MediumItalic;
        static Font *Exo2_0_Regular;
        static Font *Exo2_0_RegularItalic;
        static Font *Exo2_0_SemiBold;
        static Font *Exo2_0_SemiBoldItalic;
        static Font *Noto_Basic;
        static Font *Noto_CJK_Basic;
        static Font *Noto_CJK_Compatibility;
        static Font *Noto_Hangul;
        static Font *osuFont;
        static Font *Venera;
        static Font *Venera_Light;
        static Font *Venera_Medium;


#endif

        static bool is;

        static void initialise();

        Font(const char *name);

        void draw(wchar_t *str, int len, int x, int y, int size, float *color);
    };
}

#endif //OSU_LASER_C_FONT_H
