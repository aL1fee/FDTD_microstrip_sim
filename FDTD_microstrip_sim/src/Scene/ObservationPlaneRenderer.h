#pragma once
#include "../Simulation/ObservationPlane.h"
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <stdexcept>

// Render-thread-owned overlay; release while its OpenGL context is current.
class ObservationPlaneRenderer
{
    GLuint program=0, vao=0, vbo=0;
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
        const auto vs=compile(GL_VERTEX_SHADER,"#version 330 core\nlayout(location=0) in vec3 p; uniform mat4 mvp; void main(){gl_Position=mvp*vec4(p,1);}");
        const auto fs=compile(GL_FRAGMENT_SHADER,"#version 330 core\nuniform vec4 color; out vec4 result; void main(){result=color;}");
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
        if(vbo) glDeleteBuffers(1,&vbo);
        if(vao) glDeleteVertexArrays(1,&vao);
        if(program) glDeleteProgram(program);
        vbo=vao=program=0;
    }
    void draw(const ObservationPlane& plane,const glm::mat4& view,const glm::mat4& projection)
    {
        if((!plane.visible || plane.opacity<=0) && !plane.domainVisible) return;
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
        glEnable(GL_BLEND); glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
        glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); glDepthMask(GL_FALSE); glDisable(GL_CULL_FACE);
        const float alpha=std::max(0.f,std::min(plane.opacity,1.f));
        if (plane.visible && alpha > 0)
        {
        glUniform4f(glGetUniformLocation(program,"color"),.15f,.55f,.65f,alpha);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
        glUniform4f(glGetUniformLocation(program,"color"),.1f,.3f,.35f,alpha);
        glLineWidth(1); glDrawArrays(GL_LINE_LOOP,0,4);
        }
        if (plane.domainVisible)
        {
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
    }
};
