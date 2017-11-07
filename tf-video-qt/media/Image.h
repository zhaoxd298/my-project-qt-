#ifndef IMAGE_H
#define IMAGE_H

#include "Types.h"

// 像素点，所有的缓冲区应该用Pixel类型，这样便不需要做强制类型转换了
class Pixel
{
private:
    u8 m_B;
    u8 m_G;
    u8 m_R;
    u8 m_A;
private:
    void initPixel(u8 a, u8 r, u8 g, u8 b)
    {
        m_A = a;
        m_R = r;
        m_G = g;
        m_B = b;
    }
public:
    Pixel()
    {
        initPixel(0, 0, 0, 0);
    }
    Pixel(u8 a, u8 r, u8 g, u8 b)
    {
        initPixel(a, r, g, b);
    }
    Pixel(u8 r, u8 g, u8 b)
    {
        initPixel(0, r, g, b);
    }
    Pixel(u32 pixel)
    {
        m_A = (pixel >> 24) & 0xff;
        m_R = (pixel >> 16) & 0xff;
        m_G = (pixel >> 8) & 0xff;
        m_B = (pixel & 0xff);
    }
    Pixel(const Pixel& pixel)
    {
        m_B = pixel.m_B;
        m_G = pixel.m_G;
        m_R = pixel.m_R;
        m_A = pixel.m_A;
    }

    u8& A()
    {
        return m_A;
    }
    u8& R()
    {
        return m_R;
    }
    u8& G()
    {
        return m_G;
    }
    u8& B()
    {
        return m_B;
    }

    Pixel& operator = (const u32 pixel)
    {
        m_A = (pixel >> 24) & 0xff;
        m_R = (pixel >> 16) & 0xff;
        m_G = (pixel >> 8) & 0xff;
        m_B = (pixel & 0xff);

        return *this;
    }

    Pixel& operator = (const Pixel& pixel)
    {
        m_B = pixel.m_B;
        m_G = pixel.m_G;
        m_R = pixel.m_R;
        m_A = pixel.m_A;

        return *this;
    }

    void print()
    {
        printf("0x%02x%02x%02x%02x\n", m_A, m_R, m_G, m_B);
    }
};

class Image {
private:
    int x, y, w, h;
    Pixel* cache;

public:
    Image() {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
        cache = NULL;
    }
};

#endif // IMAGE_H
