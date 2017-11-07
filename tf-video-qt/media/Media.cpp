#include "Media.h"

#include "Log.h"
#include "Image.h"
#include "Image.h"


Media::Media()
{

}


bool Media::decodeBmp(void *imgCache, const char* filename, int xSize, int ySize)
{
    //u32 R, G, B;
    int width = 0;
    int height = 0;
    u32 bitCountPerPix = 0;
    Pixel color;
    u32 bmppicth = 0;
    u8 *pEachLinBuf = NULL;
    u8 BytePerPix;

    if ( NULL == imgCache ) {
        print_error("imgCache is empty!");
        return false;
    }

    FILE *pf = fopen(filename, "rb");
    if (!pf) {
        print_error("fopen bmp failed");
        return false;
    }

    // 此处缺少对文件正确性的判断！！！！！！！！！！！

    BITMAPFILE bmpfile;
    fread(&(bmpfile.bfHeader), sizeof(BITMAPFILEHEADER), 1, pf);
    fread(&(bmpfile.biInfo.bmiHeader), sizeof(BITMAPINFOHEADER), 1, pf);

    bitCountPerPix = bmpfile.biInfo.bmiHeader.biBitCount;
    width = bmpfile.biInfo.bmiHeader.biWidth;
    height = bmpfile.biInfo.bmiHeader.biHeight;

    bmppicth = ((width * bitCountPerPix + 31) >> 5) << 2;

    pEachLinBuf = new u8[bmppicth];
    if (!pEachLinBuf) {
        print_error("malloc error");
        return false;
    }
    BytePerPix = (bitCountPerPix) >> 3;

    for (int h=height-1; h>=0; h--) {
        fread(pEachLinBuf, bmppicth, 1, pf);  //read a row
        for (int w=0; w<width; w++) {
            color.B() = pEachLinBuf[w*BytePerPix+0];
            color.G() = pEachLinBuf[w*BytePerPix+1];
            color.R() = pEachLinBuf[w*BytePerPix+2];
            color.A() = 0;

            /*确保数据没有超出申请的缓冲区*/
            if ( (h<ySize) && (w<xSize) ) {
                ((Pixel (*)[xSize])imgCache)[h][w] = color;
            }

        }
    }
    delete(pEachLinBuf);
    fclose(pf);

    return true;
}
