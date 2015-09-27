// GiDpiUtil.cpp
// Copyright (c) 2012-2015, https://github.com/rhcad/vgios, BSD License

#import <UIKit/UIKit.h>
#include <sys/sysctl.h>
#include <mach/mach_time.h>
#include "GiUtil.h"

/*

 @"i386"      on 32-bit Simulator
 @"x86_64"    on 64-bit Simulator
 @"iPod1,1"   on iPod Touch
 @"iPod2,1"   on iPod Touch Second Generation
 @"iPod3,1"   on iPod Touch Third Generation
 @"iPod4,1"   on iPod Touch Fourth Generation
 @"iPhone1,1" on iPhone
 @"iPhone1,2" on iPhone 3G
 @"iPhone2,1" on iPhone 3GS
 @"iPad1,1"   on iPad
 @"iPad2,1"   on iPad 2
 @"iPad3,1"   on 3rd Generation iPad
 @"iPhone3,1" on iPhone 4 (GSM)
 @"iPhone3,3" on iPhone 4 (CDMA/Verizon/Sprint)
 @"iPhone4,1" on iPhone 4S
 @"iPhone5,1" on iPhone 5 (model A1428, AT&T/Canada)
 @"iPhone5,2" on iPhone 5 (model A1429, everything else)
 @"iPad3,4" on 4th Generation iPad
 @"iPad2,5" on iPad Mini
 @"iPhone5,3" on iPhone 5c (model A1456, A1532 | GSM)
 @"iPhone5,4" on iPhone 5c (model A1507, A1516, A1526 (China), A1529 | Global)
 @"iPhone6,1" on iPhone 5s (model A1433, A1533 | GSM)
 @"iPhone6,2" on iPhone 5s (model A1457, A1518, A1528 (China), A1530 | Global)
 @"iPad4,1" on 5th Generation iPad (iPad Air) - Wifi
 @"iPad4,2" on 5th Generation iPad (iPad Air) - Cellular
 @"iPad4,4" on 2nd Generation iPad Mini - Wifi
 @"iPad4,5" on 2nd Generation iPad Mini - Cellular
 @"iPhone7,1" on iPhone 6 Plus
 @"iPhone7,2" on iPhone 6
 @"iPhone8,1" on iPhone 6S
 @"iPhone8,2" on iPhone 6S Plus
 
 */

int giGetScreenDpi()
{
    static int dpi = 0;
    
    if (dpi == 0) {
        size_t size = 15;
        char machine[15 + 1] = "";
        
        sysctlbyname("hw.machine", machine, &size, NULL, 0);
        
        if (strcmp(machine, "i386") == 0 || strcmp(machine, "x86_64") == 0 )
        {
            // Simulator
            dpi = 72;
        }
        else
        {
            // Identifier: http://theiphonewiki.com/wiki/Models
            bool iPadMini = (strcmp(machine, "iPad2,5") == 0 ||
                             strcmp(machine, "iPad2,6") == 0 ||
                             strcmp(machine, "iPad2,7") == 0 ||
                             strcmp(machine, "iPad4,4") == 0 ||
                             strcmp(machine, "iPad4,5") == 0);
            BOOL iPad = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
            
            dpi = (iPad && !iPadMini) ? 132 : 163;
        }
    }
    dpi = 96;
    return dpi;
}


static int machToMs(uint64_t start)
{
    uint64_t elapsedTime = mach_absolute_time() - start;
    static double ticksToNanoseconds = -1.0;
    
    if (ticksToNanoseconds < 0) {
        mach_timebase_info_data_t timebase;
        mach_timebase_info(&timebase);
        ticksToNanoseconds = (double)timebase.numer / timebase.denom * 1e-6;
    }
    
    return (int)(elapsedTime * ticksToNanoseconds);
}

int getTickCount()
{
    static uint64_t start = mach_absolute_time();
    return machToMs(start);
}

NSString *GiLocalizedString(NSString *name)
{
    if ([name length] == 0) {
        return name;
    }
    
    NSString *str = name;
    NSString *names[] = { @"TouchVG", @"vg1", @"vg2", @"vg3", @"vg4" };
    NSString *language = [[[NSUserDefaults standardUserDefaults]
                           objectForKey:@"AppleLanguages"] objectAtIndex:0];
    
    for (int i = 0; i < 5 && [str isEqualToString:name]; i++) {
        NSString *path = [[NSBundle mainBundle] pathForResource:names[i] ofType:@"bundle"];
        NSBundle *bundle = [NSBundle bundleWithPath:path];
        NSBundle *languageBundle = [NSBundle bundleWithPath:[bundle pathForResource:language ofType:@"lproj"]];
        str = NSLocalizedStringFromTableInBundle(name, nil, languageBundle, nil);
        str = str ? str : name;
    }
    
    return [str isEqualToString:name] ? NSLocalizedString(name, nil) : str;
}
