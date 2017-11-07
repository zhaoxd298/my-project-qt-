#include "Animat.h"
#include "Log.h"

#include <QDebug>

Animat::Animat()
{

}

int Animat::getAnimatActionCnt(AnimatType type, u32 w, u32 h)
{
    int cnt = 0;

    switch (type) {
        //case PA_Random:				// 随机
        case PA_Static:					// 立刻显示
            cnt = 1;
            break;
        case PA_StaticAlways:			// 永久停留
            cnt = 1;
            break;
        case PA_MoveToLeft_S:			// 连续左移
            cnt = w; break;
        case PA_MoveToLeft:				// 左移 (4)
            cnt = w; break;
        case PA_MoveToRight_S:			// 连续右移(5)	2013-09-17:以虚拟的方式加入
            cnt = w; break;
        case PA_MoveToRight:			// 右移 (new:6, old:5)
            cnt = w; break;
        case PA_MoveToTop:				// 上移
            cnt = h; break;
        case PA_MoveToBottom:			// 下移
            cnt = h; break;
        case PA_FadeIn:					// 淡入
            cnt = FadeInOutCNT;
            break;
        case PA_Rain:					// 下雨
            cnt = h << 2;
            break;
        case PA_BubbleUp:				// 喷泉
            cnt = h << 2;
            break;
        case PA_FlashBlack:				// 亮灭闪
            cnt = FlashBlackCNT;
            break;
        case PA_FlashReverse:			// 反白闪
            cnt = FlashReverseCNT;
            break;
        case PA_ReelOpen:				// 卷轴展开
            cnt = (w>>1) + 1;
            break;
        case PA_ReelClose:				// 卷轴闭合
            cnt = (w>>1) + 1;
            break;
        case PA_LeftRay:				// 左镭射
            cnt = w;
            break;
        case PA_RightRay:				// 右镭射
            cnt = w;
            break;
        case PA_TopRay:					// 上镭射
            cnt = h;
            break;
        case PA_BottomRay:				// 下镭射
            cnt = h;
            break;
        case PA_LeftZoom:				// 向左拉伸
            cnt = w << 1;
            break;
        case PA_RightZoom:				// 向右拉伸
            cnt = w << 1;
            break;
        case PA_TopZoom:				// 向上拉伸
            cnt = h << 1;
            break;
        case PA_BottomZoom:				// 向下拉伸
            cnt = h << 1;
            break;
        case PA_FromLeftBottom:			// 左下角飞入
            cnt = w>h ? w : h;
            break;
        case PA_FromLeftTop:			// 左上角飞入
            cnt = w>h ? w : h;
            break;
        case PA_FromRightBottom:		// 右下角飞入
            cnt = w>h ? w : h;
            break;
        case PA_FromRightTop:			// 右上角飞入
            cnt = w>h ? w : h;
            break;
        case PA_ScrToRight:				// 从左向右展开
            cnt = w;
            break;
        case PA_ScrToLeft:				// 从右向左展开
            cnt = w;
            break;
        case PA_ScrToBottom:			// 从上向下展开
            cnt = h;
            break;
        case PA_ScrToTop:				// 从下向上展开
            cnt = h;
            break;
        case PA_ScrHOpen:				// 水平对开
            cnt = (1==w%2) ? (w>>1)+1 : w>>1;
            break;
        case PA_ScrHClose:				// 水平闭合
            cnt = (1==w%2) ? (w>>1)+1 : w>>1;
            break;
        case PA_ScrVOpen:				// 垂直对开
            cnt = (1==h%2) ? (h>>1)+1 : h>>1;
            break;
        case PA_ScrVClose:				// 垂直闭合
            cnt = (1==h%2) ? (h>>1)+1 : h>>1;
            break;
        case PA_HShutter:				// 水平百叶窗
            cnt = HShutterCNT;
            break;
        case PA_VShutter:				// 垂直百叶窗
            cnt = VShutterCNT;
            break;
        case PA_MoveTopBottom:			// 上下交叉对进
            cnt = h;
            break;
        case PA_MoveLeftRight:			// 左右交叉对进
            cnt = w;
            break;
        case PA_ScrTopBottom:			// 上下交叉对展
            cnt = h;
            break;
        case PA_ScrLeftRight:			// 左右交叉对展
            cnt = w;
            break;
        case PA_Mosaic:					// 马赛克
            cnt = MosaicCNT * MosaicCNT;
            break;
        case PA_DiamondOpen:			// 菱形扩散
            cnt = ((w+1)>>1) + ((h+1)>>1);
            break;
        case PA_DiamondClose:			// 菱形收缩
            cnt = ((w+1)>>1) + ((h+1)>>1);
            break;
        case PA_MoveToBottomDiagonal:	// 斜向下移
            cnt = w + h;
            break;
        case PA_CoverToBottomDiagonal:	// 斜向左展
            cnt = w + h;
            break;
        default:
            print_error("unknow animat type:%d", type);
            break;
    }

    return cnt;
}


int Animat::getExitAnimatActionCnt(ExitAnimatType type, u32 w, u32 h)
{
    int cnt = 0;

    switch (type) {
        case EX_Now:					// 1 立刻清屏
            cnt = 1;
            break;
        case EX_MoveToLeft:				// 3 左移
            cnt = w;
            break;
        case EX_MoveToRight:			// 4 右移
            cnt = w;
            break;
        case EX_MoveToTop:				// 5 上移
            cnt = h;
            break;
        case EX_MoveToBottom:			// 6 下移
            cnt = h;
            break;
        case EX_FadeOut:				// 7	淡出		?
            cnt = FadeInOutCNT;
            break;
        case EX_Rain:					// 8	下雨		?
            cnt = h << 2;
            break;
        case EX_BubbleUp:				// 9	喷泉		?
            cnt = h << 2;
            break;
        case EX_FlashBlack:				// 10	亮灭闪	?
            cnt = FlashBlackCNT;
            break;
        case EX_FlashReverse:			// 11	反白闪	?
            cnt = FlashReverseCNT;
            break;
        case EX_ReelOpen:				// 12	卷轴展开
            cnt = (w>>1) + 1;
            break;
        case EX_ReelClose:				// 13	卷轴闭合
            cnt = (w>>1) + 1;
            break;
        case EX_LeftRay:				// 14	左镭射	?
            cnt = w;
            break;
        case EX_RightRay:				// 15	右镭射	?
            cnt = w;
            break;
        case EX_TopRay:					// 16 	上镭射	?
            cnt = h;
            break;
        case EX_BottomRay:				// 17	下镭射	?
            cnt = h;
            break;
        case EX_LeftZoom:				// 18	向左拉伸	?
            cnt = w << 1;
            break;
        case EX_RightZoom:				// 19	向右拉伸	?
            cnt = w << 1;
            break;
        case EX_TopZoom:				// 20	向上拉伸	?
            cnt = (h) << 1;
            break;
        case EX_BottomZoom:				// 21	向下拉伸	?
            cnt = (h) << 1;
            break;
        case EX_ToLeftBottom:			// 22	左下角飞出
            cnt = w>h ? w : h;
            break;
        case EX_ToLeftTop:				// 23	左上角飞出
            cnt = w>h ? w : h;
            break;
        case EX_ToRightBottom:			// 24	右下角飞出
            cnt = w>h ? w : h;
            break;
        case EX_ToRightTop:				// 25	右上角飞出
            cnt = w>h ? w : h;
            break;
        case EX_ScrToRight:				// 26	从左向右展开
            cnt = w;
            break;
        case EX_ScrToLeft:				// 27	从右向左展开
            cnt = w;
            break;
        case EX_ScrToBottom:			// 28	从上向下展开
            cnt = h;
            break;
        case EX_ScrToTop:				// 29	从下向上展开
            cnt = h;
            break;
        case EX_ScrHOpen:				// 30	水平对开
            cnt = (1==w%2) ? (w>>1)+1 : w>>1;
            break;
        case EX_ScrHClose:				// 31	水平闭合
            cnt = (1==w%2) ? (w>>1)+1 : w>>1;
            break;
        case EX_ScrVOpen:				// 32	垂直对开
            cnt = (1==h%2) ? (h>>1)+1 : h>>1;
            break;
        case EX_ScrVClose:				// 33 	垂直闭合
            cnt = (1==h%2) ? (h>>1)+1 : h>>1;
            break;
        case EX_HShutter:				// 34	水平百叶窗
            cnt = HShutterCNT;
            break;
        case EX_VShutter:				// 35	垂直百叶窗
            cnt = VShutterCNT;
            break;
        case EX_MoveTopBottom:			// 36	上下交叉对进
            cnt = h;
            break;
        case EX_MoveLeftRight:			// 37	左右交叉对进
            cnt = w;
            break;
        case EX_ScrTopBottom:			// 38	上下交叉对展
            cnt = h;
            break;
        case EX_ScrLeftRight:			// 39	左右交叉对展
            cnt = w;
            break;
        case EX_Mosaic:					// 40	马赛克
            cnt = MosaicCNT * MosaicCNT;
            break;
        case EX_DiamondOpen:			// 41	菱形扩散
            cnt = ((w+1)>>1) + ((h+1)>>1);
            break;
        case EX_DiamondClose:			// 42	菱形收缩
            cnt = ((w+1)>>1) + ((h+1)>>1);
            break;
        case EX_MoveToBottomDiagonal:	// 43	斜向下移
            cnt = w + h;
            break;
        case EX_CoverToBottomDiagonal:	// 44	斜向左展
            cnt = w + h;
            break;
        default:
            print_error("unknow exit animat type:%d\n", type);
            break;
    }

    return cnt;
}


int Animat::playAnimat(const PosSize& posSize, PlayStatus& playStatus, int step, AnimatType type)
{
    bool ret = false;

    bool (*animat)(const PosSize& posSize, PlayStatus& playStatus, int step);

    switch (type) {
        case PA_Random:					// 0  随机
            return false;
            break;
        case PA_Static:					// 1  立刻显示
        case PA_StaticAlways:			// 2  永久停留
            animat = showDirect;
            break;
        case PA_MoveToLeft_S:			// 3  连续左移
        case PA_MoveToLeft:				// 4  左移 (4)
            animat = leftMoveImg;
            //animat = showDirect;
            break;
        default:
            animat = showDirect;
            break;
    }

    ret = animat(posSize, playStatus, step);

    return ret;
}

bool Animat::showDirect(const PosSize& posSize, PlayStatus& playStatus, int)
{
    fb_t (*swapCache)[posSize.w] = (fb_t (*)[posSize.w])playStatus.swapCache;
    fb_t (*imgCache)[posSize.imgWidth] = (fb_t (*)[posSize.imgWidth])playStatus.imgCache;

    u32 line_size = posSize.imgWidth * BYTE_PER_PIXEL;
    for (u32 y=0; y<posSize.imgHeight; y++) {
        memcpy(swapCache[y+posSize.yOffset]+posSize.xOffset, imgCache[y], line_size);
    }

    playStatus.movCnt += 1;
    return true;
}

bool Animat::leftMoveImg(const PosSize& posSize, PlayStatus& playStatus, int step)
{
    fb_t (*swapCache)[posSize.w] = (fb_t (*)[posSize.w])playStatus.swapCache;
    fb_t (*imgCache)[posSize.imgWidth] = (fb_t (*)[posSize.imgWidth])playStatus.imgCache;

    if (step <= 0) {
        step = 1;
    }

    int leftStep = playStatus.totalMovCnt - playStatus.movCnt;
    if (step > leftStep) {
        step = leftStep;
    }

    u32 x = playStatus.movCnt;

    u32 lineSize = (posSize.imgWidth - step) * BYTE_PER_PIXEL;
    u32 stSize = step * BYTE_PER_PIXEL;
    u32 xOffset = posSize.xOffset;
    u32 yOffset = posSize.yOffset;

    for (u32 y=0; y<posSize.imgWidth; y++) {
        /*将交换区数据向左移动一个像素点*/
        memcpy(swapCache[y+yOffset]+xOffset, swapCache[y+yOffset]+xOffset+step, lineSize);

        /*将图像缓冲区指定的一列数据拷贝到交换区最后一列*/
        memcpy(swapCache[y+yOffset]+posSize.imgWidth+xOffset-step, imgCache[y]+x, stSize);

    }

    playStatus.movCnt += step;

    //myDebug("*******************\n");

    return true;
}
