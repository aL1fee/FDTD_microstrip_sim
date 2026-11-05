#pragma once
#include "../Simulation/ObservationPlane.h"
#include "../Simulation/CavityController.h"
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <stdexcept>

// Render-thread-owned overlay; release while its OpenGL context is current.
class ObservationPlaneRenderer
{
    GLuint program=0, vao=0, vbo=0, texture=0;
    std::vector<float> pixels;
    bool textureValid=false;
    std::uint64_t lastRevision=0;
    const CavityController* lastField=nullptr;
    FieldSlice::Plane lastOrientation=FieldSlice::XY;
    Yee3D::Axis lastComponent=Yee3D::Z;
    std::size_t lastLayer=0, textureWidth=0, textureHeight=0;
    static GLuint compile(GLenum type, const char* source)
    {
        GLuint shader=glCreateShader(type);
        glShaderSource(shader,1,&source,nullptr); glCompileShader(shader);
        GLint ok=0; glGetShaderiv(shader,GL_COMPILE_STATUS,&ok);
        if(!ok) { glDeleteShader(shader); throw std::runtime_error("Observation plane shader compilation failed"); }
        return shader;
    }
    void initialize()
    {
        const auto vs=compile(GL_VERTEX_SHADER,R"(#version 330 core
layout(location=0) in vec3 p;
uniform mat4 mvp;
uniform ivec2 axes;
uniform vec3 extent;
uniform vec2 uvScale, uvOffset;
out vec2 uv;
void main(){
    gl_Position=mvp*vec4(p,1);
    uv=vec2(p[axes.x]/extent[axes.x],p[axes.y]/extent[axes.y])*uvScale+uvOffset;
})");
        const auto fs=compile(GL_FRAGMENT_SHADER,R"(#version 330 core
uniform vec4 color;
uniform sampler2D fieldTexture;
uniform bool useField;
in vec2 uv;
out vec4 result;
void main(){
    if(useField){
        float value=clamp(texture(fieldTexture,uv).r,-1.0,1.0);
        float fade=1.0-abs(value);
        result=vec4(value<0.0 ? fade:1.0,fade,value>0.0 ? fade:1.0,color.a);
    }else result=color;
})");
        program=glCreateProgram(); glAttachShader(program,vs); glAttachShader(program,fs); glLinkProgram(program);
        glDeleteShader(vs); glDeleteShader(fs);
        GLint ok=0; glGetProgramiv(program,GL_LINK_STATUS,&ok);
        if(!ok) { release(); throw std::runtime_error("Observation plane shader linking failed"); }
        glGenVertexArrays(1,&vao); glGenBuffers(1,&vbo);
        glBindVertexArray(vao); glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,72*sizeof(float),nullptr,GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),nullptr); glEnableVertexAttribArray(0);
    }
public:
    ObservationPlaneRenderer() = default;
    ObservationPlaneRenderer(const ObservationPlaneRenderer&) = delete;
    ObservationPlaneRenderer& operator=(const ObservationPlaneRenderer&) = delete;
    ~ObservationPlaneRenderer() { release(); }
    void release()
    {
        if(texture) glDeleteTextures(1,&texture);
        texture=0; textureValid=false; lastField=nullptr;
        if(vbo) glDeleteBuffers(1,&vbo);
        if(vao) glDeleteVertexArrays(1,&vao);
        if(program) glDeleteProgram(program);
        vbo=vao=program=0;
    }
    void draw(const ObservationPlane& plane,const glm::mat4& view,const glm::mat4& projection,const CavityController* field=nullptr)
    {
        if((!plane.visible || plane.opacity<=0) && !plane.domainVisible) return;
        const bool showField=plane.visible && plane.opacity>0 && plane.colorByField && field && field->ready() && field->grid()==plane.grid;
        FieldSlice slice(plane.grid.cells,plane.orientation,plane.layer,plane.component);
        const bool upload=showField && (!textureValid || lastField!=field || lastRevision!=field->fieldRevision() ||
            lastOrientation!=plane.orientation || lastComponent!=plane.component || lastLayer!=slice.layer);
        if(upload)
        {
            pixels.resize(slice.width()*slice.height());
            for(std::size_t v=0;v<slice.height();++v) for(std::size_t u=0;u<slice.width();++u)
                pixels[v*slice.width()+u]=field->sample(slice.index(u,v),plane.component);
        }
        GLint oldActiveTexture,oldTexture,oldSampler,unpackBuffer,unpackAlignment,unpackRowLength,unpackSkipRows,unpackSkipPixels;
        glGetIntegerv(GL_ACTIVE_TEXTURE,&oldActiveTexture); glActiveTexture(GL_TEXTURE0);
        glGetIntegerv(GL_TEXTURE_BINDING_2D,&oldTexture); glGetIntegerv(GL_SAMPLER_BINDING,&oldSampler);
        glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING,&unpackBuffer);
        glGetIntegerv(GL_UNPACK_ALIGNMENT,&unpackAlignment); glGetIntegerv(GL_UNPACK_ROW_LENGTH,&unpackRowLength);
        glGetIntegerv(GL_UNPACK_SKIP_ROWS,&unpackSkipRows); glGetIntegerv(GL_UNPACK_SKIP_PIXELS,&unpackSkipPixels);
        GLint oldProgram,oldVao,oldBuffer,srcRgb,dstRgb,srcAlpha,dstAlpha,eqRgb,eqAlpha,depthFunc;
        GLboolean depthWrite; GLfloat width;
        const bool blend=glIsEnabled(GL_BLEND)!=0,cull=glIsEnabled(GL_CULL_FACE)!=0,depth=glIsEnabled(GL_DEPTH_TEST)!=0;
        glGetIntegerv(GL_CURRENT_PROGRAM,&oldProgram); glGetIntegerv(GL_VERTEX_ARRAY_BINDING,&oldVao);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&oldBuffer); glGetBooleanv(GL_DEPTH_WRITEMASK,&depthWrite);
        glGetIntegerv(GL_BLEND_SRC_RGB,&srcRgb); glGetIntegerv(GL_BLEND_DST_RGB,&dstRgb);
        glGetIntegerv(GL_BLEND_SRC_ALPHA,&srcAlpha); glGetIntegerv(GL_BLEND_DST_ALPHA,&dstAlpha);
        glGetIntegerv(GL_BLEND_EQUATION_RGB,&eqRgb); glGetIntegerv(GL_BLEND_EQUATION_ALPHA,&eqAlpha);
        glGetIntegerv(GL_DEPTH_FUNC,&depthFunc); glGetFloatv(GL_LINE_WIDTH,&width);
        if(!program) initialize();
        float vertices[12]; const auto corners=plane.cornersMeters();
        for(int k=0;k<4;++k) for(int axis=0;axis<3;++axis)
            vertices[k*3+axis]=static_cast<float>(corners[k][axis]*1000); // scene uses mm
        glBindVertexArray(vao); glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
        glUseProgram(program); const auto mvp=projection*view;
        glUniformMatrix4fv(glGetUniformLocation(program,"mvp"),1,GL_FALSE,glm::value_ptr(mvp));
        glUniform1i(glGetUniformLocation(program,"useField"),showField);
        glUniform1i(glGetUniformLocation(program,"fieldTexture"),0);
        glUniform2i(glGetUniformLocation(program,"axes"),slice.horizontal,slice.vertical);
        glUniform3f(glGetUniformLocation(program,"extent"),
            static_cast<float>(plane.grid.cells[0]*plane.grid.spacing*1000),
            static_cast<float>(plane.grid.cells[1]*plane.grid.spacing*1000),
            static_cast<float>(plane.grid.cells[2]*plane.grid.spacing*1000));
        glUniform2f(glGetUniformLocation(program,"uvScale"),
            static_cast<float>(plane.grid.cells[slice.horizontal])/slice.width(),
            static_cast<float>(plane.grid.cells[slice.vertical])/slice.height());
        glUniform2f(glGetUniformLocation(program,"uvOffset"),
            slice.horizontal==plane.component ? 0.f : .5f/static_cast<float>(slice.width()),
            slice.vertical==plane.component ? 0.f : .5f/static_cast<float>(slice.height()));
        if(showField)
        {
            if(!texture) glGenTextures(1,&texture);
            glBindTexture(GL_TEXTURE_2D,texture); glBindSampler(0,0);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
            if(upload)
            {
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
                glPixelStorei(GL_UNPACK_ALIGNMENT,1); glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
                glPixelStorei(GL_UNPACK_SKIP_ROWS,0); glPixelStorei(GL_UNPACK_SKIP_PIXELS,0);
                if(!textureValid || textureWidth!=slice.width() || textureHeight!=slice.height())
                    glTexImage2D(GL_TEXTURE_2D,0,GL_R32F,static_cast<GLsizei>(slice.width()),static_cast<GLsizei>(slice.height()),0,GL_RED,GL_FLOAT,pixels.data());
                else glTexSubImage2D(GL_TEXTURE_2D,0,0,0,static_cast<GLsizei>(slice.width()),static_cast<GLsizei>(slice.height()),GL_RED,GL_FLOAT,pixels.data());
                textureWidth=slice.width(); textureHeight=slice.height(); textureValid=true;
                lastField=field; lastRevision=field->fieldRevision(); lastOrientation=plane.orientation;
                lastComponent=plane.component; lastLayer=slice.layer;
            }
        }
        glEnable(GL_BLEND); glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
        glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); glDepthMask(GL_FALSE); glDisable(GL_CULL_FACE);
        const float alpha=std::max(0.f,std::min(plane.opacity,1.f));
        if (plane.visible && alpha > 0)
        {
        glUniform4f(glGetUniformLocation(program,"color"),.15f,.55f,.65f,alpha);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
        glUniform1i(glGetUniformLocation(program,"useField"),0);
        glUniform4f(glGetUniformLocation(program,"color"),.1f,.3f,.35f,alpha);
        glLineWidth(1); glDrawArrays(GL_LINE_LOOP,0,4);
        }
        if (plane.domainVisible)
        {
            glUniform1i(glGetUniformLocation(program,"useField"),0);
            float edges[72]; int cursor=0;
            for(int axis=0;axis<3;++axis) for(int a=0;a<2;++a) for(int b=0;b<2;++b)
                for(int end=0;end<2;++end)
                {
                    int corner[3]{}; corner[axis]=end; corner[(axis+1)%3]=a; corner[(axis+2)%3]=b;
                    for(int c=0;c<3;++c) edges[cursor++]=static_cast<float>(corner[c]*plane.grid.cells[c]*plane.grid.spacing*1000);
                }
            glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(edges),edges);
            glUniform4f(glGetUniformLocation(program,"color"),.1f,.3f,.35f,1);
            glLineWidth(1); glDrawArrays(GL_LINES,0,24);
        }
        glLineWidth(width); glDepthMask(depthWrite); glDepthFunc(depthFunc);
        if(!depth)glDisable(GL_DEPTH_TEST); if(cull)glEnable(GL_CULL_FACE); if(!blend)glDisable(GL_BLEND);
        glBlendFuncSeparate(srcRgb,dstRgb,srcAlpha,dstAlpha); glBlendEquationSeparate(eqRgb,eqAlpha);
        glUseProgram(oldProgram); glBindVertexArray(oldVao); glBindBuffer(GL_ARRAY_BUFFER,oldBuffer);
        glBindTexture(GL_TEXTURE_2D,oldTexture); glBindSampler(0,oldSampler); glActiveTexture(oldActiveTexture);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER,unpackBuffer);
        glPixelStorei(GL_UNPACK_ALIGNMENT,unpackAlignment); glPixelStorei(GL_UNPACK_ROW_LENGTH,unpackRowLength);
        glPixelStorei(GL_UNPACK_SKIP_ROWS,unpackSkipRows); glPixelStorei(GL_UNPACK_SKIP_PIXELS,unpackSkipPixels);
    }
};
