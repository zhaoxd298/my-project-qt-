#ifndef ANIMAT_H
#define ANIMAT_H

#include <QImage>
#include "Types.h"
#include "../play_prog/ProgList.h"


#define PA_ACTION_CNT 46
#define EX_ACTION_CNT 42

/*
* 动画效果操作总步数
*/
enum AnimatCNT
{
    FadeInOutCNT 		= 150,		// 淡入、淡出
    FlashBlackCNT 		= 25,		// 亮灭闪
    FlashReverseCNT 	= 25,		// 反白闪
    HShutterCNT			= 16,		// 水平百叶窗
    VShutterCNT			= 16,		// 垂直百叶窗
    MosaicCNT			= 16,		// 马赛克
};

/*
* 动画效果
*/
enum AnimatType
{
    PA_Random					= 0,	// 随机
    PA_Static					= 1,	// 立刻显示
    PA_StaticAlways				= 2,	// 永久停留
    PA_MoveToLeft_S				= 3,	// 连续左移
    PA_MoveToLeft				= 4,	// 左移 (4)
    PA_MoveToRight_S			= 5, 	// 连续右移(5)	2013-09-17:以虚拟的方式加入
    PA_MoveToRight				= 6,	// 右移 (new:6, old:5)
    PA_MoveToTop				= 7,	// 上移
    PA_MoveToBottom				= 8, 	// 下移
    PA_FadeIn					= 9,	// 淡入
    PA_Rain						= 10,	// 下雨
    PA_BubbleUp					= 11,	// 喷泉
    PA_FlashBlack				= 12,	// 亮灭闪
    PA_FlashReverse				= 13,	// 反白闪
    PA_ReelOpen					= 14,	// 卷轴展开
    PA_ReelClose				= 15,	// 卷轴闭合
    PA_LeftRay					= 16,	// 左镭射
    PA_RightRay					= 17,	// 右镭射
    PA_TopRay					= 18, 	// 上镭射
    PA_BottomRay				= 19,	// 下镭射
    PA_LeftZoom					= 20,	// 向左拉伸
    PA_RightZoom				= 21,	// 向右拉伸
    PA_TopZoom					= 22,	// 向上拉伸
    PA_BottomZoom				= 23,	// 向下拉伸
    PA_FromLeftBottom			= 24,	// 左下脚飞入
    PA_FromLeftTop				= 25,	// 左上脚飞入
    PA_FromRightBottom			= 26,	// 右下脚飞入
    PA_FromRightTop				= 27,	// 右上脚飞入
    PA_ScrToRight				= 28,	// 从左向右展开
    PA_ScrToLeft				= 29,	// 从右向左展开
    PA_ScrToBottom				= 30,	// 从上向下展开
    PA_ScrToTop					= 31,	// 从下向上展开
    PA_ScrHOpen					= 32,	// 水平对开
    PA_ScrHClose				= 33,	// 水平闭合
    PA_ScrVOpen					= 34,	// 垂直对开
    PA_ScrVClose				= 35, 	// 垂直闭合
    PA_HShutter					= 36,	// 水平百叶窗
    PA_VShutter					= 37,	// 垂直百叶窗
    PA_MoveTopBottom			= 38,	// 上下交叉对进
    PA_MoveLeftRight			= 39,	// 左右交叉对进
    PA_ScrTopBottom				= 40,	// 上下交叉对展
    PA_ScrLeftRight				= 41,	// 左右交叉对展
    PA_Mosaic					= 42,	// 马赛克
    PA_DiamondOpen				= 43,	// 菱形扩散
    PA_DiamondClose				= 44,	// 菱形收缩
    PA_MoveToBottomDiagonal		= 45,	// 斜向下移
    PA_CoverToBottomDiagonal	= 46,	// 斜向左展
};

/*
* 退出动画效果
*/
enum ExitAnimatType
{
    EX_None						= 0,	// 不清屏
    EX_Now						= 1,	// 立刻清屏
    EX_Random					= 2,	// 随机
    EX_MoveToLeft				= 3,	// 左移
    EX_MoveToRight				= 4,	// 右移
    EX_MoveToTop				= 5,	// 上移
    EX_MoveToBottom				= 6, 	// 下移
    EX_FadeOut					= 7,	// 淡出
    EX_Rain						= 8,	// 下雨
    EX_BubbleUp					= 9,	// 喷泉
    EX_FlashBlack				= 10,	// 亮灭闪
    EX_FlashReverse				= 11,	// 反白闪
    EX_ReelOpen					= 12,	// 卷轴展开
    EX_ReelClose				= 13,	// 卷轴闭合
    EX_LeftRay					= 14,	// 左镭射
    EX_RightRay					= 15,	// 右镭射
    EX_TopRay					= 16, 	// 上镭射
    EX_BottomRay				= 17,	// 下镭射
    EX_LeftZoom					= 18,	// 向左拉伸
    EX_RightZoom				= 19,	// 向右拉伸
    EX_TopZoom					= 20,	// 向上拉伸
    EX_BottomZoom				= 21,	// 向下拉伸
    EX_ToLeftBottom				= 22,	// 左下脚飞出
    EX_ToLeftTop				= 23,	// 左上脚飞出
    EX_ToRightBottom			= 24,	// 右下脚飞出
    EX_ToRightTop				= 25,	// 右上脚飞出
    EX_ScrToRight				= 26,	// 从左向右展开
    EX_ScrToLeft				= 27,	// 从右向左展开
    EX_ScrToBottom				= 28,	// 从上向下展开
    EX_ScrToTop					= 29,	// 从下向上展开
    EX_ScrHOpen					= 30,	// 水平对开
    EX_ScrHClose				= 31,	// 水平闭合
    EX_ScrVOpen					= 32,	// 垂直对开
    EX_ScrVClose				= 33, 	// 垂直闭合
    EX_HShutter					= 34,	// 水平百叶窗
    EX_VShutter					= 35,	// 垂直百叶窗
    EX_MoveTopBottom			= 36,	// 上下交叉对进
    EX_MoveLeftRight			= 37,	// 左右交叉对进
    EX_ScrTopBottom				= 38,	// 上下交叉对展
    EX_ScrLeftRight				= 39,	// 左右交叉对展
    EX_Mosaic					= 40,	// 马赛克
    EX_DiamondOpen				= 41,	// 菱形扩散
    EX_DiamondClose				= 42,	// 菱形收缩
    EX_MoveToBottomDiagonal		= 43,	// 斜向下移
    EX_CoverToBottomDiagonal	= 44,	// 斜向左展
};



class Animat {
public:
    Animat();

    /*
    * 获取指定动画效果需要的操作步数
    * @type：动画类型
    * @w：图片宽
    * @h：图片高
    */
    static int getAnimatActionCnt(AnimatType type, u32 w, u32 h);
    /*
    * 获取指定退出动画效果需要的操作步数
    * @type：动画类型
    * @w：图片宽
    * @h：图片高
    */
    static int getExitAnimatActionCnt(ExitAnimatType type, u32 w, u32 h);

    static int playAnimat(const PosSize& posSize, PlayStatus& playStatus, int step, AnimatType type);

    // 直接显示
    static bool showDirect(const PosSize& posSize, PlayStatus& playStatus, int step);

    // 图片左移
    static bool leftMoveImg(const PosSize& posSize, PlayStatus& playStatus, int step);
};

#endif // EFFECT_H
