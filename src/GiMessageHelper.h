//
//  GiMessageHelper.h
//  TouchVG
//
//  Created by Macbook on 9/1/15.
//  Copyright (c) 2015 Founder. All rights reserved.
//

//! The UILabel subclass for show message text.
@interface WDLabel : UILabel
@end

//! 提示文字辅助类
@interface GiMessageHelper : NSObject {
    WDLabel     *_label;
    NSTimer     *_timer;
    
    id sharedInstance;
}

- (void)showMessage:(NSString *)message inView:(UIView *)view;

@end

@interface Toast:NSObject

+ (GiMessageHelper*) msgHelper;

+ (void) showMessage:(NSString *)message inView:(UIView *)view;

@end