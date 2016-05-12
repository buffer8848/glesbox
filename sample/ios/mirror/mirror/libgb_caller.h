//
//  libgb_caller.h
//  maskshow
//
//  Created by buffer on 16/4/19.
//  Copyright © 2016年 xxx. All rights reserved.
//

#ifndef libgb_caller_h
#define libgb_caller_h

#import <Foundation/Foundation.h>

@interface  GBCaller : NSObject 

+(void) image_draw: (uint8_t*)data width: (uint32_t)width height: (uint32_t)height windows: (unsigned long)id;

@end



#endif /* libgb_caller_h */
