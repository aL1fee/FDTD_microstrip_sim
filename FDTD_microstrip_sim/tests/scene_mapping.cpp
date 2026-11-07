// Integration test: link application objects except main.obj; requires OpenGL.
#include "../src/Renderer.h"
#include "../src/Simulation/SceneCavitySetup.h"
#include "../src/GUI/FieldSliceView.h"
#include "../src/GUI/FieldProbeView.h"
#include "../src/GUI/PhysicalPlaybackView.h"
#include "../src/GUI/SceneHierarchyView.h"
#include "../src/Scene/ObservationPlaneRenderer.h"
#include "../src/Simulation/Microstrip5880Preset.h"
#include <stdexcept>
Renderer* renderer = nullptr;
static void require(bool ok) { if (!ok) throw std::runtime_error("Scene mapping integration failed"); }
int main()
{
    require(glfwInit() != 0);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto* window = glfwCreateWindow(320, 240, "Scene mapping validation", nullptr, nullptr);
    require(window != nullptr);
    glfwMakeContextCurrent(window);
    require(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0);
    // Scene lifetime matches process; avoid unrelated legacy scene teardown.
    auto* scene = new MainScene(window);
    std::string name = "Trace";
    scene->addTrace(name, glm::vec3(0), 4.5f, .15f, .01f, glm::vec3(1), 1, 410);
    require(scene->getConductors().size() == 1);
    CavityController cavity;
    SceneCoverage coverage;
    const CavityController::Settings settings{CavityController::Material::Scene, 1};
    resetSceneCavity(cavity, *scene, settings, &coverage);
    require(cavity.error().empty());
    std::cout << "Default thin trace PEC samples: " << cavity.conductorSamples() << std::endl;
    require(cavity.conductorSamples() == 0);
    auto* trace = scene->getConductors().front();
    *trace->getOriginX() = 5; *trace->getOriginY() = 1; *trace->getOriginZ() = 1;
    *trace->getLength() = 2; *trace->getWidth() = 2; *trace->getHeight() = 2;
    resetSceneCavity(cavity, *scene, settings, &coverage);
    std::cout << "Resized scene trace PEC samples: " << cavity.conductorSamples() << std::endl;
    require(cavity.error().empty() && cavity.conductorSamples() == 108);
    require(coverage.size() == 1 && coverage[0].samples == 108 && coverage[0].sizeMm == glm::vec3(2));
    for (int n=0; n<100; ++n) cavity.stepOnce();
    require(cavity.sample({12,4,4}) == 0);
    // Use the actual grid settings object consumed by the editor controls.
    auto* simulation = new SimulationSpace(window, scene);
    simulation->setDimensions(glm::vec3(8,4,4));
    *simulation->getCellSize() = .25f; // Same stored value edited by the GUI
    CavityController::Settings sceneSettings{CavityController::Material::Scene,1,simulation->gridSettings3D()};
    resetSceneCavity(cavity, *scene, sceneSettings, &coverage);
    require(cavity.cells()==Yee3D::Index{32,16,16} && cavity.spacing()==.00025);
    require(cavity.conductorSamples()==1176 && coverage[0].samples==1176);
    std::cout << "Shared scene controls: 32 x 16 x 16, 250 um, 1176 PEC samples" << std::endl;
    for(auto material : {CavityController::Material::Vacuum, CavityController::Material::Uniform, CavityController::Material::Scene})
    {
        auto configured=sceneSettings; configured.material=material; configured.permittivity=4;
        resetSceneCavity(cavity,*scene,configured,&coverage);
        require(cavity.cells()==Yee3D::Index{32,16,16} && cavity.spacing()==.00025);
        require(cavity.conductorSamples()==(material==CavityController::Material::Scene ? 1176u : 0u));
    }
    std::cout << "Configured grid preserved across Vacuum, Uniform and Scene material choices." << std::endl;
    *trace->getOriginX() = 20;
    resetSceneCavity(cavity, *scene, settings, &coverage);
    require(cavity.conductorSamples() == 0);
    std::cout << "Actual scene insertion, resize, mapping and out-of-box checks passed." << std::endl;
    auto drivenSettings=settings; drivenSettings.excitation=CavityController::Excitation::SceneSources;
    resetSceneCavity(cavity,*scene,drivenSettings,&coverage);
    require(!cavity.error().empty()); // no Source object yet
    std::string sourceName="Source";
    scene->addPowerSource(sourceName,glm::vec3(1,2,2),glm::vec3(1,0,0),-40,.5f,1.f,glm::vec3(1),1,0);
    auto* source=scene->getPowerSources().back();
    *source->getFrequency()=10;
    resetSceneCavity(cavity,*scene,drivenSettings,&coverage);
    require(cavity.error().empty() && cavity.sources().size()==1);
    require(cavity.sources()[0].axis==Yee3D::Y && cavity.sources()[0].samples.size()==2);
    const auto originalIndex=cavity.sources()[0].samples[0];
    require(cavity.sample(originalIndex,Yee3D::Y)==0);
    cavity.stepOnce(); require(cavity.sample(originalIndex,Yee3D::Y)>0);
    *source->getOriginX()=2;
    require(cavity.sources()[0].samples[0]==originalIndex); // edits apply only at reset
    resetSceneCavity(cavity,*scene,drivenSettings,&coverage);
    const auto shifted=cavity.sources()[0].samples[0];
    require(shifted[0]==originalIndex[0]+2 && shifted[1]==originalIndex[1] && shifted[2]==originalIndex[2]);
    *source->getRotationAngle()=90;
    resetSceneCavity(cavity,*scene,drivenSettings,&coverage);
    require(cavity.error().empty() && cavity.sources()[0].samples[0]!=shifted);
    *source->getOriginX()=100;
    resetSceneCavity(cavity,*scene,drivenSettings,&coverage);
    require(!cavity.error().empty());
    *source->getOriginX()=2;
    resetSceneCavity(cavity,*scene,drivenSettings,&coverage);
    require(cavity.error().empty());
    std::cout << "Real Source object: normalized Ey drive, zero initialization, translation, rotation and outside rejection passed." << std::endl;
    PropagationValidation referenceCheck;
    const auto sceneStep=cavity.stepCount();
    referenceCheck.start();
    while(referenceCheck.running() && referenceCheck.completed().empty()) referenceCheck.advance(20);
    referenceCheck.cancel();
    require(referenceCheck.error().empty() && referenceCheck.completed().size()==1 && cavity.stepCount()==sceneStep);
    ImGui::CreateContext();
    auto& io=ImGui::GetIO(); io.DisplaySize=ImVec2(800,900); io.DeltaTime=1.0f/60;
    io.IniFilename=nullptr;
    unsigned char* pixels; int fontWidth,fontHeight;
    io.Fonts->GetTexDataAsRGBA32(&pixels,&fontWidth,&fontHeight);
    // The probe section must render safely before setup, including after failure.
    CavityController uninitialized;
    for(int attempt=0;attempt<2;++attempt)
    {
        if(attempt) require(!uninitialized.reset({CavityController::Material::Uniform,0}));
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(780,880));
        ImGui::Begin("Uninitialized probe panel");
        ImGui::SetNextItemOpen(true,ImGuiCond_Always);
        drawFieldProbes(uninitialized,LengthUnits::Unit::Micrometer);
        ImGui::End(); ImGui::Render();
        require(!uninitialized.ready());
    }
    std::cout << "Uninitialized and failed-setup probe sections render safely." << std::endl;
    for(int plane=0;plane<3;++plane)
    {
        int chosen=plane, layer=10000; // clamp stale slices after grid resize
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0,0)); ImGui::SetNextWindowSize(ImVec2(780,880));
        ImGui::Begin("Slice rendering validation");
        ImGui::SetNextItemOpen(true,ImGuiCond_Always);
        drawFieldProbes(cavity,LengthUnits::Unit::Micrometer);
        ImGui::SetNextItemOpen(true,ImGuiCond_Always);
        drawPropagationValidation(referenceCheck);
        int component=plane;
        drawFieldSlice(cavity,chosen,layer,LengthUnits::Unit::Micrometer,&component);
        ImGui::End(); ImGui::Render();
        require(ImGui::GetDrawData()->TotalVtxCount>0);
        for(int list=0;list<ImGui::GetDrawData()->CmdListsCount;++list)
            for(const auto& vertex : ImGui::GetDrawData()->CmdLists[list]->VtxBuffer)
                require(std::isfinite(vertex.pos.x) && std::isfinite(vertex.pos.y));
    }
    {
        AsyncCavity playback;
        for(int state=0;state<3;++state) {
            if(state==1) require(playback.reset({}));
            if(state==2) {
                auto config=playback.playbackSettings(); config.mode=PhysicalPlayback::PhysicalTime;
                playback.setPlayback(config); playback.setRunning(true);
            }
            playback.poll();
            ImGui::NewFrame();
            ImGui::SetNextWindowSize(ImVec2(780,880));
            ImGui::Begin("Physical playback validation");
            drawPhysicalPlayback(playback);
            ImGui::End(); ImGui::Render();
            require(ImGui::GetDrawData()->TotalVtxCount>0);
            for(int list=0;list<ImGui::GetDrawData()->CmdListsCount;++list)
                for(const auto& vertex : ImGui::GetDrawData()->CmdLists[list]->VtxBuffer)
                    require(std::isfinite(vertex.pos.x) && std::isfinite(vertex.pos.y));
        }
        playback.setRunning(false);
        std::cout << "Physical-time controls render before setup, paused, and running in both unit modes." << std::endl;
    }
    {
        auto* selected=scene->getSceneObjects().front();
        require(scene->selectObjectById(selected->getId()));
        const auto originalShader=selected->getShaderName();
        selected->setDisplayName("Feed trace ## 1");
        require(selected->getDisplayName()=="Feed trace ## 1" && selected->getShaderName()==originalShader);
        SceneHierarchyState hierarchy;
        ImVec2 firstRow;
        auto frame=[&](ImGuiKey key,bool down,bool typing) {
            if(key!=ImGuiKey_None) io.AddKeyEvent(key,down);
            ImGui::NewFrame();
            ImGui::SetNextWindowSize(ImVec2(248,500)); ImGui::SetNextWindowFocus();
            ImGui::Begin("Hierarchy keyboard validation");
            const auto cursor=ImGui::GetCursorScreenPos();
            firstRow=ImVec2(cursor.x+40,cursor.y+ImGui::GetTextLineHeightWithSpacing()+3);
            io.WantTextInput=typing;
            drawSceneHierarchy(*scene,hierarchy);
            ImGui::End(); ImGui::Render();
        };
        frame(ImGuiKey_None,false,false);
        for(auto key : {ImGuiKey_R,ImGuiKey_T,ImGuiKey_Y}) {
            frame(key,true,false);
            const auto expected=key==ImGuiKey_R ? Rotation : key==ImGuiKey_T ? Translation : Scaling;
            require(scene->getModifyingVectors()->getType()==expected);
            require(scene->getModifyingVectors()->isBeingRendered());
            frame(key,false,false);
        }
        frame(ImGuiKey_R,true,true); // typing 'r' must not activate rotation
        require(scene->getModifyingVectors()->getType()==Scaling);
        frame(ImGuiKey_R,false,false);
        require(scene->getActiveObject()==selected);
        auto mouse=[&](bool down) {
            io.AddMousePosEvent(firstRow.x,firstRow.y);
            io.AddMouseButtonEvent(ImGuiMouseButton_Left,down);
            frame(ImGuiKey_None,false,false);
        };
        mouse(true); mouse(false); mouse(true); mouse(false);
        require(hierarchy.editing && hierarchy.objectId==selected->getId());
        frame(ImGuiKey_None,false,true); // editor focus and auto-selection
        io.AddInputCharactersUTF8("Renamed trace"); frame(ImGuiKey_None,false,true);
        frame(ImGuiKey_Enter,true,true); frame(ImGuiKey_Enter,false,false);
        require(!hierarchy.editing && selected->getDisplayName()=="Renamed trace");
        // Start another edit and cancel it: model name must not change.
        hierarchy.editing=true; hierarchy.focusEditor=true; hierarchy.objectId=selected->getId();
        std::snprintf(hierarchy.name,sizeof(hierarchy.name),"%s",selected->getDisplayName().c_str());
        frame(ImGuiKey_None,false,true); frame(ImGuiKey_None,false,true);
        io.AddInputCharactersUTF8("Cancelled name"); frame(ImGuiKey_None,false,true);
        frame(ImGuiKey_Escape,true,true); frame(ImGuiKey_Escape,false,false);
        require(!hierarchy.editing && selected->getDisplayName()=="Renamed trace");
        selected->setDisplayName("");
        require(selected->getDisplayName()==originalShader+" #"+std::to_string(selected->getId()));
        std::cout << "Hierarchy R/T/Y, typing guard, literal editable names and stable selection identity passed." << std::endl;
        std::cout << "Double-click inline rename, Enter commit and Escape cancel passed." << std::endl;
    }
    ImGui::DestroyContext();
    std::cout << "Field-probe panels and independent reference result plots rendered." << std::endl;
    {
        ObservationPlane plane{GridSettings{{8,6,4},.001},FieldSlice::XY,1,true,.5f};
        ObservationPlaneRenderer overlay;
        glViewport(0,0,320,240); glDisable(GL_SCISSOR_TEST);
        glDisable(GL_BLEND); glEnable(GL_CULL_FACE); glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE); glDepthFunc(GL_LESS); glClearColor(1,1,1,1); glClearDepth(1);
        glUseProgram(0); glBindVertexArray(0); glBindBuffer(GL_ARRAY_BUFFER,0);
        while(glGetError()!=GL_NO_ERROR) {}
        auto renderPixel=[&]() {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            overlay.draw(plane,glm::mat4(1),glm::ortho(0.f,8.f,0.f,6.f,-10.f,10.f));
            std::array<float,4> pixel{};
            glReadPixels(160,120,1,1,GL_RGBA,GL_FLOAT,pixel.data());
            require(glGetError()==GL_NO_ERROR);
            return pixel;
        };
        auto half=renderPixel();
        require(std::abs(half[0]-.575f)<.02f && std::abs(half[1]-.775f)<.02f);
        GLboolean depthWrite; glGetBooleanv(GL_DEPTH_WRITEMASK,&depthWrite);
        require(depthWrite && glIsEnabled(GL_CULL_FACE) && glIsEnabled(GL_DEPTH_TEST) && !glIsEnabled(GL_BLEND));
        GLint bound; glGetIntegerv(GL_CURRENT_PROGRAM,&bound); require(bound==0);
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING,&bound); require(bound==0);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&bound); require(bound==0);
        float depth; glReadPixels(160,120,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth); require(depth==1);
        plane.opacity=0; require(renderPixel()[0]==1);
        plane.opacity=1; require(std::abs(renderPixel()[0]-.15f)<.02f);
        plane.visible=false; require(renderPixel()[0]==1);
        plane.visible=true; glClearDepth(0); require(renderPixel()[0]==1); glClearDepth(1);
        plane.visible=false; plane.domainVisible=true;
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        overlay.draw(plane,glm::mat4(1),glm::ortho(-1.f,9.f,-1.f,7.f,-10.f,10.f));
        std::vector<float> domainPixels(320*240*4);
        glReadPixels(0,0,320,240,GL_RGBA,GL_FLOAT,domainPixels.data());
        std::size_t colored=0;
        for(std::size_t i=0;i<domainPixels.size();i+=4) if(domainPixels[i]<.9f) ++colored;
        require(colored>100 && colored<5000 && glGetError()==GL_NO_ERROR);
        std::cout << "Active solver wireframe rendered with slice hidden." << std::endl;
        std::cout << "Observation plane GPU pixels: alpha, hide, depth occlusion and GL-state restoration passed." << std::endl;
    }
    {
        CavityController field;
        CavityController::Settings fieldSettings{CavityController::Material::Vacuum,1,GridSettings{{8,6,4},.001}};
        require(field.reset(fieldSettings));
        ObservationPlaneRenderer overlay;
        glClearColor(1,1,1,1); glClearDepth(1); glViewport(0,0,320,240);
        GLuint oldTexture,oldSampler,oldUnpack;
        glGenTextures(1,&oldTexture); glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,oldTexture);
        glGenSamplers(1,&oldSampler); glBindSampler(0,oldSampler);
        glGenBuffers(1,&oldUnpack); glBindBuffer(GL_PIXEL_UNPACK_BUFFER,oldUnpack);
        glBufferData(GL_PIXEL_UNPACK_BUFFER,64,nullptr,GL_STATIC_DRAW);
        glPixelStorei(GL_UNPACK_ALIGNMENT,8); glPixelStorei(GL_UNPACK_ROW_LENGTH,3);
        glPixelStorei(GL_UNPACK_SKIP_ROWS,1); glPixelStorei(GL_UNPACK_SKIP_PIXELS,1);
        glActiveTexture(GL_TEXTURE3);
        auto verify=[&](FieldSlice::Plane orientation,Yee3D::Axis component,int layer) {
            ObservationPlane plane{field.grid(),orientation,layer,true,.7f,false,component,true};
            FieldSlice slice(field.cells(),orientation,layer,component);
            glm::mat4 view(0.f);
            view[slice.horizontal][0]=1; view[slice.vertical][1]=1; view[slice.normal][2]=1; view[3][3]=1;
            const auto projection=glm::ortho(0.f,static_cast<float>(field.cells()[slice.horizontal]*field.spacing()*1000),
                0.f,static_cast<float>(field.cells()[slice.vertical]*field.spacing()*1000),-100.f,100.f);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            overlay.draw(plane,view,projection,&field);
            for(int py : {40,100,180}) for(int px : {40,120,250})
            {
                const auto u=static_cast<std::size_t>(std::floor((px+.5)/320*field.cells()[slice.horizontal]+(slice.horizontal==component ? 0:.5)));
                const auto v=static_cast<std::size_t>(std::floor((py+.5)/240*field.cells()[slice.vertical]+(slice.vertical==component ? 0:.5)));
                const float value=std::max(-1.f,std::min(field.sample(slice.index(u,v),component),1.f));
                const float fade=1-std::abs(value);
                const float expected[3]={value<0 ? fade:1,fade,value>0 ? fade:1};
                float pixel[4]; glReadPixels(px,py,1,1,GL_RGBA,GL_FLOAT,pixel);
                for(int c=0;c<3;++c) require(std::abs(pixel[c]-(.3f+.7f*expected[c]))<.02f);
            }
            GLint state; glGetIntegerv(GL_ACTIVE_TEXTURE,&state); require(state==GL_TEXTURE3);
            glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING,&state); require(static_cast<GLuint>(state)==oldUnpack);
            glGetIntegerv(GL_UNPACK_ALIGNMENT,&state); require(state==8);
            glGetIntegerv(GL_UNPACK_ROW_LENGTH,&state); require(state==3);
            glGetIntegerv(GL_UNPACK_SKIP_ROWS,&state); require(state==1);
            glGetIntegerv(GL_UNPACK_SKIP_PIXELS,&state); require(state==1);
            glActiveTexture(GL_TEXTURE0); glGetIntegerv(GL_TEXTURE_BINDING_2D,&state); require(static_cast<GLuint>(state)==oldTexture);
            glGetIntegerv(GL_SAMPLER_BINDING,&state); require(static_cast<GLuint>(state)==oldSampler); glActiveTexture(GL_TEXTURE3);
            require(glGetError()==GL_NO_ERROR);
        };
        for(auto plane : {FieldSlice::XY,FieldSlice::XZ,FieldSlice::YZ})
            for(auto component : {Yee3D::X,Yee3D::Y,Yee3D::Z}) verify(plane,component,1);
        verify(FieldSlice::YZ,Yee3D::Z,1); // cached paused frame
        for(int n=0;n<30;++n) field.stepOnce();
        verify(FieldSlice::YZ,Yee3D::Z,1); // changed values after stepping
        verify(FieldSlice::YZ,Yee3D::Z,3); // moved plane
        require(field.reset(fieldSettings)); verify(FieldSlice::XY,Yee3D::Z,1);
        fieldSettings.excitation=CavityController::Excitation::XPulse;
        require(field.reset(fieldSettings)); verify(FieldSlice::XY,Yee3D::Z,1); // reset at the same step and grid
        fieldSettings.grid={{6,4,2},.001}; require(field.reset(fieldSettings)); verify(FieldSlice::XY,Yee3D::Z,1);
        glDeleteTextures(1,&oldTexture); glDeleteSamplers(1,&oldSampler); glDeleteBuffers(1,&oldUnpack);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0); glPixelStorei(GL_UNPACK_ALIGNMENT,4); glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS,0); glPixelStorei(GL_UNPACK_SKIP_PIXELS,0); glActiveTexture(GL_TEXTURE0);
        std::cout << "Scene field texture matches solver pixels: all planes/components, pause, step, move, reset, resize and texture-state restoration passed." << std::endl;
    }
    std::cout << "XY, XZ, YZ ImGui render lists and stale-slice clamping passed." << std::endl;
    {
        auto* pcbScene = new MainScene(window);
        auto* pcbSimulation = new SimulationSpace(window,pcbScene);
        Microstrip5880Preset::add(*pcbScene,*pcbSimulation);
        require(pcbScene->getConductors().size()==2 && pcbScene->getSubstrates().size()==1 && pcbScene->getPowerSources().size()==1);
        const auto mil=Microstrip5880Preset::mil;
        auto closeValue=[](float a,float b) { return std::abs(a-b)<1e-6f; };
        auto conductors=pcbScene->getConductors();
        require(closeValue(*conductors[0]->getLength(),200*mil) && closeValue(*conductors[0]->getWidth(),100*mil) && closeValue(*conductors[0]->getHeight(),5*mil));
        require(closeValue(*conductors[1]->getOriginY(),10*mil) && closeValue(*conductors[1]->getOriginZ(),43*mil));
        require(closeValue(*conductors[1]->getLength(),200*mil) && closeValue(*conductors[1]->getWidth(),14*mil) && closeValue(*conductors[1]->getHeight(),1.4f*mil));
        auto* dielectric=pcbScene->getSubstrates()[0];
        require(closeValue(dielectric->getRelativePermittivity(),2.2f) && closeValue(*dielectric->getOriginY(),5*mil) && closeValue(*dielectric->getOriginZ(),20*mil));
        require(closeValue(*dielectric->getLength(),200*mil) && closeValue(*dielectric->getWidth(),60*mil) && closeValue(*dielectric->getHeight(),5*mil));
        auto grid=pcbSimulation->gridSettings3D();
        require(grid.cells==Yee3D::Index{200,30,100} && grid.cellCount()==600000);
        require(std::abs(grid.spacing-25.4e-6)<1e-12);
        grid.validate();
        std::cout << "System #3 grid: " << grid.cellCount() << " cells; resident " << grid.residentBytes()/1048576.0
            << " MiB; conservative reset " << (grid.resetBytes()+grid.residentBytes())/1048576.0 << " MiB" << std::endl;
        CavityController pcb;
        SceneCoverage pcbCoverage;
        CavityController::Settings setup{CavityController::Material::Scene,2.2f,grid,CavityController::Excitation::SceneSources};
        resetSceneCavity(pcb,*pcbScene,setup,&pcbCoverage);
        if(!pcb.error().empty()) throw std::runtime_error(pcb.error());
        require(pcbCoverage.size()==2 && pcbCoverage[0].samples>0 && pcbCoverage[1].samples>0);
        require(pcb.sources().size()==1 && !pcb.sources()[0].samples.empty());
        auto* pcbSource=pcbScene->getPowerSources()[0];
        const float traceTop=*conductors[1]->getOriginY()+*conductors[1]->getHeight();
        require(closeValue(*pcbSource->getOriginY()-*pcbSource->getRadius(),traceTop));
        require(pcb.sources()[0].excluded==0 && pcb.sources()[0].axis==Yee3D::Y);
        for(const auto& sample : pcb.sources()[0].samples)
            require((sample[1]+.5)*grid.spacing > traceTop*1e-3);
        const auto point=pcb.sources()[0].samples.front();
        require(pcb.sample(point,Yee3D::Y)==0);
        pcb.stepOnce();
        require(std::isfinite(pcb.sample(point,Yee3D::Y)) && pcb.sample(point,Yee3D::Y)>0);
        std::cout << "System #3 full-resolution reset and step passed: carrier " << pcbCoverage[0].samples
            << ", trace " << pcbCoverage[1].samples << ", driven source " << pcb.sources()[0].samples.size() << " samples." << std::endl;
        const auto objects=pcbScene->getSceneObjects();
        require(objects.size()==4); // carrier, substrate, trace, source; no editor helpers
        for(auto* object : objects) {
            require(pcbScene->selectObjectById(object->getId()));
            require(pcbScene->getActiveObject()==object);
            require(pcbScene->getPropertyWindow()->getActiveObject()==object);
            require(pcbScene->getPropertyWindow()->getProperties()==object->getPropertyMap());
            require(pcbScene->getPropertyWindow()->getPropertiesInt()==object->getPropertyMapInt());
            require(*pcbScene->getModifyingVectors()->getOrigin()==object->getCenterLocation());
        }
        std::string extra="Trace";
        pcbScene->addTrace(extra,glm::vec3(0),mil,mil,mil,glm::vec3(1),1,1);
        require(pcbScene->getSceneObjects().size()==5);
        const auto extraId=pcbScene->getSceneObjects().back()->getId();
        require(pcbScene->selectObjectById(extraId));
        pcbScene->deleteActiveObject();
        require(pcbScene->getSceneObjects().size()==4 && !pcbScene->getActiveObject());
        require(!pcbScene->getPropertyWindow()->getActiveObject());
        require(!pcbScene->selectObjectById(extraId));
        require(!pcbScene->selectObjectById(pcbScene->getModifyingVectors()->getId()));
        std::cout << "Hierarchy enumeration, selection/property binding, gizmo placement, insertion and deletion passed." << std::endl;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

