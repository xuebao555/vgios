//! \file GiPaintView.h
//! \brief 定义iOS绘图视图类 GiPaintView
// Copyright (c) 2012-2015, https://github.com/rhcad/vgios, BSD License

#import "GiPaintViewDelegate.h"
#import "GiImageCache.h"
#include "GiUtil.h"

#include <vector>
#include "mgvector.h"


class GiCoreView;
class GiView;
class GiViewAdapter;


@class GiMagnifierView;

//! 动态图形的绘图视图类
@interface GiDynDrawView : UIView {
    GiViewAdapter   *_adapter;
}

- (id)initView:(CGRect)frame :(GiViewAdapter *)adapter;
+ (void)draw:(GiViewAdapter *)adapter;

@end

//! 在层上显示图形的渲染类
@interface GiLayerRender : NSObject {
    CALayer         *_layer;
    GiViewAdapter   *_adapter;
    __block long    _drawing;
    __block mgvector<long>* _docs;
    __block long    _gs;
    dispatch_queue_t _queue;
}

- (id)initWithAdapter:(GiViewAdapter *)adapter;
- (void)stopRender;
- (void)clearCachedData;
- (void)startRender:(mgvector<long>*)docs :(long)gs;
- (BOOL)renderInContext:(CGContextRef)ctx;

@end

//! iOS绘图视图类
/*! \ingroup GROUP_IOS
    \see GiPaintViewDelegate, GiPaintViewXIB
 */
@interface GiPaintView : UIView<UIGestureRecognizerDelegate>

@property(nonatomic, readonly) UIPanGestureRecognizer *panRecognizer;           //!< 拖动手势识别器
@property(nonatomic, readonly) UITapGestureRecognizer *tapRecognizer;           //!< 单指点击手势识别器
@property(nonatomic, readonly) UITapGestureRecognizer *twoTapsRecognizer;       //!< 单指双击手势识别器
@property(nonatomic, readonly) UILongPressGestureRecognizer *pressRecognizer;   //!< 单指长按手势识别器
@property(nonatomic, readonly) UIPinchGestureRecognizer *pinchRecognizer;       //!< 双指放缩手势识别器
@property(nonatomic, readonly) UIRotationGestureRecognizer *rotationRecognizer; //!< 双指旋转手势识别器
@property(nonatomic)           BOOL gestureEnabled;     //!< 是否允许触摸交互

@property(nonatomic, readonly) GiImageCache         *imageCache;                //!< 图像对象缓存
@property(nonatomic, readonly) GiPaintView          *mainView;                  //!< 放大镜对应的主视图
@property(nonatomic, assign)   UIView               *viewToMagnify;             //!< 待放大的视图
@property(nonatomic, readonly) NSArray              *delegates;                 //!< GiPaintViewDelegate
@property(nonatomic, assign)   NSInteger            flags; //!< 由 GIViewFlags 按位组成的视图特性标志
@property(nonatomic, assign)   BOOL                 contextActionEnabled;       //!< 是否允许上下文操作

//! 创建普通图形视图，并添加到父视图、设置为当前视图，不需要额外释放
+ (GiPaintView *)createGraphView:(CGRect)frame :(UIView *)parentView :(int)flags;

//! 创建放大镜视图并添加到父视图，不需要额外释放
+ (GiPaintView *)createMagnifierView:(CGRect)frame
                             refView:(GiPaintView *)refView
                          parentView:(UIView *)parentView;

+ (void)addContextAction:(int)action block:(dispatch_block_t)block;
- (IBAction)onContextAction:(id)sender;     //!< 上下文按钮的响应函数

- (id)initWithFrame:(CGRect)frame flags:(int)f;

+ (GiPaintView *)activeView;                //!< 得到当前激活的绘图视图
- (void)activiteView;                       //!< 设置为当前活动视图，触摸时自动调用
- (UIView *)dynamicShapeView:(BOOL)create;  //!< 动态图形视图

- (id<NSLocking>)locker;                    //!< 返回显示锁定对象

- (GiCoreView *)coreView;                   //!< 得到跨平台内核视图
- (GiView *)viewAdapter;                    //!< 得到视图适配器对象, GiViewAdapter
- (GiViewAdapter *)viewAdapter2;            //!< 得到视图适配器对象

- (long)acquireFrontDoc;                    //!< 获取前端文档句柄
- (void)releaseDoc:(long)doc;               //!< 释放文档句柄


- (UIImage *)snapshotCG;
- (UIImage *)snapshot;                      //!< 得到静态图形的快照，自动释放

- (void)clearCachedData;                    //!< 释放临时数据内存
- (void)tearDown;                           //!< 停止后台任务
- (void)stopRecord:(BOOL)forUndo;           //!< 停止录制图形

- (void)hideContextActions;                 //!< 隐藏上下文按钮

- (void)addDelegate:(id<GiPaintViewDelegate>)d;     //!< 增加绘图消息观察者
- (void)removeDelegate:(id<GiPaintViewDelegate>)d;  //!< 去掉绘图消息观察者

// added on 2015-09-03
- (void)initialZoomPan;
- (void)extentZoomPan;
- (void)setViewScaleRangeFrom:(CGFloat)minScale To:(CGFloat)maxScale;
- (void)setBackgroundImage:(NSString *)bgImage width:(CGFloat) width height:(CGFloat) height scale:(CGFloat)scale;

- (void) setShapeInfo:(NSString *)key withValue:(NSString *)value;
- (NSString *) getShapeInfo:(NSString *)key;

- (void) showMessage:(NSString* )msg;

@end

/*! \category GiPaintView(GestureRecognizer)
    \brief GiPaintView 的手势响应函数部分
 */
@interface GiPaintView(GestureRecognizer)

- (void)setupGestureRecognizers;                                //!< 设置手势识别器
- (BOOL)panHandler:(UIPanGestureRecognizer *)sender;            //!< 拖动手势识别响应
- (BOOL)moveHandler:(UIGestureRecognizer *)sender;              //!< 放缩和旋转手势识别响应
- (BOOL)tapHandler:(UITapGestureRecognizer *)sender;            //!< 单指点击手势响应
- (BOOL)twoTapsHandler:(UITapGestureRecognizer *)sender;        //!< 单指双击手势响应
- (BOOL)pressHandler:(UILongPressGestureRecognizer *)sender;    //!< 单指长按手势响应
- (void)delayTap;                                               //!< 延时响应点击手势
- (void)dispatchTapPending;                                     //!< 分发挂起的单击手势

@end

/*! \category GiPaintView()
 \brief GiPaintView 的内部数据定义
 */
@interface GiPaintView() {
    GiViewAdapter   *_adapter;              //!< 视图回调适配器
    
    UIGestureRecognizer     *_recognizers[7];
    GiMagnifierView         *_magnifierView;            //!< 手指跟随放大镜
    
    std::vector<CGPoint>    _points;        //!< 手势生效前的轨迹
    CGPoint                 _startPt;       //!< 开始位置
    CGPoint                 _lastPt;        //!< 上次位置
    CGPoint                 _tapPoint;      //!< 点击位置
    int                     _tapCount;      //!< 点击次数
    int                     _touchCount;    //!< 触点个数
    BOOL            _gestureRecognized;     //!< 识别出手势
    BOOL                    _buttonHandled;
    CGPoint                 _ignorePt;
}

@end

/*! \category GiPaintView(GestureInternel)
 \brief GiPaintView 的手势响应实现部分
 */
@interface GiPaintView(GestureInternel)

- (void)ignoreTouch:(CGPoint)pt :(UIView *)handledButton;
- (void)redrawForDelay:(id)changed;
- (void)onContextActionsDisplay:(NSMutableArray *)buttons;

@end
