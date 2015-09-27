//! \file GiViewImpl.h
//! \brief 定义iOS绘图视图类的内部实现接口
// Copyright (c) 2012-2015, https://github.com/rhcad/vgios, BSD License

#import <QuartzCore/QuartzCore.h>

#import "GiMessageHelper.h"
#import "GiPaintView.h"
#import "GiViewEnums.h"
#include "GiCanvasAdapter.h"
#include "gicoreview.h"
#include <vector>
#include "ARCMacro.h"



//! iOS绘图视图适配器
class GiViewAdapter : public GiView
{
private:
    GiPaintView     *_view;             //!< 静态图形视图
    UIView          *_dynview;          //!< 动态图形视图, GiDynDrawView
    GiCoreView      *_core;             //!< 内核视图分发器
    NSRecursiveLock *_lock;             //!< 内核对象临界区
    NSMutableArray  *_buttons;          //!< 上下文按钮的数组
    NSMutableDictionary *_buttonImages; //!< 按钮图像缓存
    GiImageCache    *_imageCache;       //!< 图像对象缓存
    GiMessageHelper *_messageHelper;    //!< 提示文字辅助对象
    int             _flags;             //!< 视图创建标志，由 GIViewFlags 组成
    bool            _actionEnabled;     //!< 是否允许上下文操作
    long            _appendIDs[20];     //!< 还未来得及重构显示的新增图形的ID、playh
    int             _oldAppendCount;    //!< 后台渲染前的待渲染新增图形数
    int             _regenCount;        //!< 渲染次数
    GiLayerRender   *_render;           //!< 后台渲染对象
    dispatch_queue_t _queues[2];        //!< 录制队列
    __block bool    _recordStopping[2]; //!< 录制队列待停止
    
public:
    
    /* flags : GIViewFlags
     */
    GiViewAdapter(GiPaintView *mainView, GiViewAdapter *refView, int flags);
    virtual ~GiViewAdapter();
    
    std::vector<id> delegates;          //!< GiPaintViewDelegate 观察者数组
    struct {
        unsigned int didCommandChanged:1;
        unsigned int didSelectionChanged:1;
        unsigned int didContentChanged:1;
        unsigned int didDynamicChanged:1;
        unsigned int didZoomChanged:1;
        unsigned int didDynDrawEnded:1;
        unsigned int didShapesRecorded:1;
        unsigned int didShapeWillDelete:1;
        unsigned int didShapeDeleted:1;
        unsigned int didShapeDblClick:1;
        unsigned int didShapeClicked:1;
        unsigned int didGestureShouldBegin:1;
        unsigned int didGestureBegan:1;
        unsigned int didGestureEnded:1;
    } respondsTo;
    
    void addDelegate(id<GiPaintViewDelegate> d);     //!< 增加绘图消息观察者
    void removeDelegate(id<GiPaintViewDelegate> d);  //!< 去掉绘图消息观察者
    

    GiCoreView *coreView() { return _core; }
    GiImageCache *imageCache() { return _imageCache; }
    UIView *mainView() { return _view; }
    UIView *getDynView(bool autoCreate);
    void clearCachedData();
    void stopRegen();
    void onFirstRegen();
    bool canShowMagnifier() const;
    long acquireFrontDoc(long* gs = NULL);
    id<NSLocking> locker() { return _lock; }
    int getFlags() const { return _flags; }
    int setFlags(int flags);
    
    int getAppendCount() const;
    void beginRender();
    bool renderInContext(CGContextRef ctx);
    int getAppendID(int index, long& playh) const;
    
    void undo();
    void redo();
    bool startRecord(NSString *path, bool forUndo);
    void stopRecord(bool forUndo);
    bool hasShapesRecorded();
    void onShapesRecorded(NSDictionary *info);
    void onDynDrawEnded();
    
    bool dispatchGesture(GiGestureType type, GiGestureState state, CGPoint pt);
    bool dispatchPan(GiGestureState state, CGPoint pt, bool switchGesture = false);
    bool twoFingersMove(UIGestureRecognizer *sender, int state = -1, bool switchGesture = false);
    
    
    // inherited from GiView
    virtual void regenAll(bool changed);
    virtual void regenAppend(int sid, long playh);
    virtual void redraw(bool changed);
    virtual bool isContextActionsVisible();
    virtual bool showContextActions(const mgvector<int>& actions,
                                    const mgvector<float>& buttonXY,
                                    float x, float y, float w, float h);
    virtual void hideContextActions();
    void setContextActionEnabled(bool enabled) { _actionEnabled = enabled; }
    bool getContextActionEnabled() const { return _actionEnabled; }
    
    virtual void commandChanged();
    virtual void selectionChanged();
    virtual void contentChanged();
    virtual void dynamicChanged();
    virtual void zoomChanged();
    virtual void shapeWillDelete(int sid);
    virtual void shapeDeleted(int sid);
    virtual bool shapeDblClick(int type, int sid, int tag);
    virtual bool shapeClicked(int type, int sid, int tag, float x, float y);
    
    virtual void showMessage(const char* text);
    virtual void getLocalizedString(const char* name, MgStringCallback* result);
    
private:
    void setContextButton(UIButton *btn, NSString *caption, NSString *imageName);
    int  regenLocked(bool changed, int sid, long playh, bool loading, long& doc0,
                     long& doc1, long& shapes1, long& gs, mgvector<long>*& docs);
    void regen_(bool changed, int sid, long playh, bool loading);
    void recordShapes(bool forUndo, long changeCount, long doc, long shapes);
};

//! 获取字符串的回调接口
typedef void (^GiStringBlock)(NSString *s);

//! 块回调代理的MgStringCallback实现类
struct GiStringCallback : MgStringCallback {
    GiStringBlock c;
    GiStringCallback(GiStringBlock c) : c(c) {}
    void onGetString(const char* text) { c(@(text)); }
};

GiColor CGColorToGiColor(CGColorRef color);