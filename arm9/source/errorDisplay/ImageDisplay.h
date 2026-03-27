#pragma once

class ImageDisplay {
public:
    ImageDisplay();

    void DrawTop(const unsigned int* tiles, int tilesLen, const unsigned short* map, int mapLen, const unsigned short* palette, int paletteLen);
    void DrawBottom(const unsigned int* tiles, int tilesLen, const unsigned short* map, int mapLen, const unsigned short* palette, int paletteLen);
};
