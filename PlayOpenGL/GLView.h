//
//  GLView.h
//  PlayOpenGL
//
//  Created by William Roe on 16/11/2012.
//  Copyright (c) 2012 William Roe. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h> 
#import <OpenGLES/ES1/gl.h> 
#import <OpenGLES/ES1/glext.h>

@interface GLView : UIView {
    EAGLContext* m_context;
}

- (void) drawView;

@end
