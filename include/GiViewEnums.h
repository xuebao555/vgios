//! \file GiViewEnums.h
//! \brief 定义iOS绘图视图的枚举值
// Copyright (c) 2012-2015, https://github.com/rhcad/vgios, BSD License

#ifndef __TOUCHVG_VIEWENUMS_H
#define __TOUCHVG_VIEWENUMS_H

typedef NS_ENUM(int, GILineStyle) { //!< 线型
    GILineStyleSolid,       //!< ----------
    GILineStyleDash,        //!< － － － －
    GILineStyleDot,         //!< ..........
    GILineStyleDashDot,     //!< _._._._._
    GILineStyleDashDotdot,  //!< _.._.._.._
    GILineStyleNull         //!< Not draw.
};

typedef NS_ENUM(int, GIArrowHead) { //!< 线端箭头
    GIArrowHeadNone = 0,        //!< 无箭头
    GIArrowHeadSharpClosed,     //!< 实心尖头
    GIArrowHeadSharpLine,       //!< 简单线条尖头
    GIArrowHeadTLine,           //!< T型箭头
    GIArrowHeadSlashLine,       //!< 斜线箭头
    GIArrowHeadClosedCircle,    //!< 实心圆头
    GIArrowHeadOpenedCircle     //!< 空心圆头
};

typedef NS_ENUM(int, GIViewFlags) {     //!< 视图创建标志
    GIViewFlagsNoBackLayer      = 1,    //!< 不使用分层异步渲染
    GIViewFlagsNoDynDrawView    = 2,    //!< 不使用双视图显示
    GIViewFlagsNotDynDraw       = 4,    //!< 屏蔽动态显示
    GIViewFlagsZoomExtent       = 8,    //!< 自动放缩到图形范围
    GIViewFlagsNoCmd            = 16,   //!< 不使用交互命令
    GIViewFlagsMagnifier        = 32,   //!< 显示跟随放大镜
    GIViewFlagsDummyView        = (GIViewFlagsNoBackLayer|GIViewFlagsNoDynDrawView
                                   |GIViewFlagsZoomExtent|GIViewFlagsNoCmd)
};

typedef NS_ENUM(int, GISelDrawFlag) {
    GISelDrawDragLine      = 1<<1,     //!< 拖动提示的参考线
    GISelDrawDragDot       = 1<<2,     //!< 拖动的终点
    GISelDrawGroupBorder   = 1<<3,     //!< 成组图形的外框
    GISelDrawActionBorder  = 1<<4,     //!< 上下文按钮包络框
    GISelDrawSelBorder     = 1<<5,     //!< 选择集包络框
    GISelDrawBlueShape     = 1<<6,     //!< 蓝色显示选中的图形
    GISelDrawHandle        = 1<<7,     //!< 控制点
    
    GISelDrawXformBox      = 1<<16,    //!< 矩形变形框
    GISelDrawNearPt        = 1<<17,    //!< 最近点
};

#endif // __TOUCHVG_VIEWENUMS_H
