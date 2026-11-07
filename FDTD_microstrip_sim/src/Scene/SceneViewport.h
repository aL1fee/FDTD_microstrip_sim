#pragma once

// Logical window coordinates shared by the image panel and mouse-ray mapping.
struct SceneViewport
{
    float x=0,y=0,width=0,height=0;
    bool visible=false,hovered=false;
    bool contains(double px,double py) const {
        return visible && width>0 && height>0 && px>=x && py>=y && px<x+width && py<y+height;
    }
};
inline SceneViewport& sceneViewport() { static SceneViewport value; return value; }
