#ifndef MEDIA_H
#define MEDIA_H

#include "Types.h"

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER {
    u16 bfType;			//位图文件的类型，必须为BM
    u32 bfSize;			//位图文件的大小，以字节为单位
    u16 bfReserved1;	//位图文件保留字，必须为0
    u16 bfReserved2;	//位图文件保留字，必须为0
    u32 bfOffBits;		//位图数据的起始位置，以相对于位图
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
        u32 biSize;				//本结构所占用字节数
        u32 biWidth;			//位图的宽度，以像素为单位
        u32 biHeight;			//位图的高度，以像素为单位
        u16 biPlanes;			//目标设备的级别，必须为1
        u16 biBitCount;			//每个像素所需的位数，必须是1（双色），
                                //4(16色），8(256色）16(高彩色) 24（真彩色）或32之一
        u32 biCompression;		//位图压缩类型，必须是0（不压缩），
                                //1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一
        u32 biSizeImage;		//位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)，以字节为单位
        u32 biXPelsPerMeter;	//位图水平分辨率，每米像素数
        u32 biYPelsPerMeter;	//位图垂直分辨率，每米像素数
        u32 biClrUsed;			//位图实际使用的颜色表中的颜色数
        u32 biClrImportant;		//位图显示过程中重要的颜色数
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
    u8 rgbBlue;
    u8 rgbGreen;
    u8 rgbRed;
    u8 rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
} BITMAPINFO;


typedef struct tagBITMAP {
    BITMAPFILEHEADER bfHeader;
    BITMAPINFO biInfo;
}BITMAPFILE;

#pragma pack(pop)

class Media {
public:
    Media();

    static bool decodeBmp(void *imgCache, const char* filename, int xSize, int ySize);
};

#endif // MEDIA_H
