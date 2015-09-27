//
//  GiMessageHelper.m
//  TouchVG
//
//  Created by Macbook on 9/1/15.
//  Copyright (c) 2015 Founder. All rights reserved.
//

#import "GiMessageHelper.h"

#pragma mark - GiMessageHelper

@implementation WDLabel

- (void)drawRect:(CGRect)rect
{
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    
    rect = CGRectInset(rect, 8.0f, 8.0f);
    CGContextSetShadow(ctx, CGSizeMake(0, 2), 4);
    
    [[UIColor colorWithWhite:0.0f alpha:0.5f] set];
    
    UIBezierPath *path = [UIBezierPath bezierPathWithRoundedRect:rect cornerRadius:9.0f];
    [path fill];
    
    [[UIColor whiteColor] set];
    path.lineWidth = 2;
    [path stroke];
    
    [super drawRect:rect];
}

@end

@implementation GiMessageHelper

- (void)hideMessage {
    [UIView animateWithDuration:0.2f
                     animations:^{ _label.alpha = 0.0f; }
                     completion:^(BOOL finished) {
                         [self removeLabel];
                     }];
}

- (void)removeLabel {
    if (_timer) {
        [_timer invalidate];
        _timer = nil;
    }
    if (_label) {
        [_label removeFromSuperview];
        _label = nil;
    }
}

- (void)showMessage:(NSString *)message inView:(UIView *)view {
    BOOL created = NO;
    
    [self removeLabel];
    
    if (!_label) {
        _label = [[WDLabel alloc] initWithFrame:CGRectInset(CGRectMake(0,0,100,40), -8, -8)];
        _label.textColor = [UIColor whiteColor];
        _label.font = [UIFont boldSystemFontOfSize:24.0f];
        _label.textAlignment = NSTextAlignmentCenter;
        _label.opaque = NO;
        _label.backgroundColor = nil;
        _label.alpha = 0;
        
        created = YES;
    }
    
    _label.text = message;
    [_label sizeToFit];
    
    CGRect frame = _label.frame;
    frame.size.width = MAX(frame.size.width, 80.f);
    frame = CGRectInset(frame, -20, -15);
    _label.frame = frame;
    _label.center = view.center;
    
    if (created) {
        [view addSubview:_label];
        
        [UIView animateWithDuration:0.2f animations:^{ _label.alpha = 1; }];
    }
    
    // start message dismissal timer
    if (_timer) {
        [_timer invalidate];
        _timer = nil;
    }
    
    _timer = [NSTimer scheduledTimerWithTimeInterval:0.7 target:self
                                            selector:@selector(hideMessage)
                                            userInfo:nil repeats:NO];
}

@end


@implementation Toast:NSObject

+ (GiMessageHelper *) msgHelper {
    static GiMessageHelper *gimessageHelper = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        gimessageHelper = [[GiMessageHelper alloc] init];
    });
    return gimessageHelper;
}


+ (void) showMessage:(NSString *)message inView:(UIView *)view
{
    [[Toast msgHelper] showMessage:message inView:view];
}

@end