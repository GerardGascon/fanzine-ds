#pragma once

class ImageDisplay {
public:
    ImageDisplay();

    void DrawTop(const unsigned int* bitmap, const unsigned short* palette);
    void DrawBottom(const unsigned int* bitmap, const unsigned short* palette);
};
