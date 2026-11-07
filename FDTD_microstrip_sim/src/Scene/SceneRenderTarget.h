#pragma once
#include <glad/glad.h>
#include <stdexcept>

// Renderer-owned offscreen target. UI displays its stable texture ID; storage
// is resized only when the scene panel's framebuffer dimensions change.
class SceneRenderTarget
{
public:
    GLuint framebuffer=0,texture=0,depth=0;
    int width=0,height=0;
    void resize(int w,int h) {
        if(w==width && h==height) return;
        if(!framebuffer) {
            glGenFramebuffers(1,&framebuffer); glGenTextures(1,&texture); glGenRenderbuffers(1,&depth);
        }
        glBindTexture(GL_TEXTURE_2D,texture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glBindRenderbuffer(GL_RENDERBUFFER,depth);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,w,h);
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture,0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,depth);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Scene framebuffer is incomplete");
        glBindFramebuffer(GL_FRAMEBUFFER,0); glBindTexture(GL_TEXTURE_2D,0); glBindRenderbuffer(GL_RENDERBUFFER,0);
        width=w; height=h;
    }
    void release() {
        glDeleteFramebuffers(1,&framebuffer); glDeleteTextures(1,&texture); glDeleteRenderbuffers(1,&depth);
        framebuffer=texture=depth=0; width=height=0;
    }
};
