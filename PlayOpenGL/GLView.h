//
//  GLView.h
//  PlayOpenGL
//
//  Created by William Roe on 16/11/2012.
//  Copyright (c) 2012 William Roe. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "IRenderingEngine.hpp"
#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>

@interface GLView : UIView {
@private
    EAGLContext* m_context;
    IRenderingEngine* m_renderingEngine;
    float m_timestamp;
}

- (void) drawView: (CADisplayLink*) displayLink;
- (void) didRotate: (NSNotification*) notification;

@end
