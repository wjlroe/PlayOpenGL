//
//  GLView.mm
//  PlayOpenGL
//
//  Created by William Roe on 16/11/2012.
//  Copyright (c) 2012 William Roe. All rights reserved.
//

#import "GLView.h"

@implementation GLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer;
        eaglLayer.opaque = YES;
        
        m_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!m_context || ![EAGLContext setCurrentContext:m_context]) {
            return nil;
        }
        
        GLuint framebuffer, renderbuffer;
        glGenFramebuffersOES(1, &framebuffer);
        glGenRenderbuffersOES(1, &renderbuffer);
        
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);
        
        [m_context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
        
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, renderbuffer);
        
        glViewport(0, 0, CGRectGetWidth(frame), CGRectGetHeight(frame));
        
        [self drawView];
    }
    return self;
}

- (void)drawView
{
    glClearColor(0.9f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    [m_context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

-(void) dealloc
{
    if ([EAGLContext currentContext] == m_context) {
        [EAGLContext setCurrentContext:nil];
    }
}

@end
