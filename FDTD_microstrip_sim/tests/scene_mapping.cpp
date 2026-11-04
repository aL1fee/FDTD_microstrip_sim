// Integration test: link application objects except main.obj; requires OpenGL.
#include "../src/Renderer.h"
#include "../src/Simulation/SceneCavitySetup.h"
#include "../src/GUI/FieldSliceView.h"
#include "../src/Scene/ObservationPlaneRenderer.h"
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
    ImGui::CreateContext();
    auto& io=ImGui::GetIO(); io.DisplaySize=ImVec2(800,900); io.DeltaTime=1.0f/60;
    io.IniFilename=nullptr;
    unsigned char* pixels; int fontWidth,fontHeight;
    io.Fonts->GetTexDataAsRGBA32(&pixels,&fontWidth,&fontHeight);
    for(int plane=0;plane<3;++plane)
    {
        int chosen=plane, layer=10000; // clamp stale slices after grid resize
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0,0)); ImGui::SetNextWindowSize(ImVec2(780,880));
        ImGui::Begin("Slice rendering validation");
        int component=plane;
        drawFieldSlice(cavity,chosen,layer,LengthUnits::Unit::Micrometer,&component);
        ImGui::End(); ImGui::Render();
        require(ImGui::GetDrawData()->TotalVtxCount>0);
        for(int list=0;list<ImGui::GetDrawData()->CmdListsCount;++list)
            for(const auto& vertex : ImGui::GetDrawData()->CmdLists[list]->VtxBuffer)
                require(std::isfinite(vertex.pos.x) && std::isfinite(vertex.pos.y));
    }
    ImGui::DestroyContext();
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
    std::cout << "XY, XZ, YZ ImGui render lists and stale-slice clamping passed." << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
}

