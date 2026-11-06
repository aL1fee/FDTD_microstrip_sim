#pragma once
#include "LengthUnits.h"
#include "../Simulation/FieldSlice.h"
#include "../Simulation/CavityController.h"
#include "../vendor/imgui/imgui.h"

template<class Controller>
inline void drawFieldSlice(Controller& controller, int& plane, int& layer, LengthUnits::Unit unit, int* selectedComponent = nullptr)
{
    const auto cells = controller.cells();
    const double dx = controller.spacing();
    if(selectedComponent) ImGui::Combo("Electric component", selectedComponent, "Ex\0Ey\0Ez\0");
    const auto component=static_cast<Yee3D::Axis>(selectedComponent ? *selectedComponent : 2);
    if (ImGui::Combo("Slice plane", &plane, "XY (move Z)\0XZ (move Y)\0YZ (move X)\0"))
    {
        FieldSlice middle(cells, static_cast<FieldSlice::Plane>(plane), 0, component);
        layer = static_cast<int>(middle.samples[middle.normal]/2);
    }
    FieldSlice slice(cells, static_cast<FieldSlice::Plane>(plane), layer, component);
    layer = static_cast<int>(slice.layer);
    const char* axes[] = {"X", "Y", "Z"};
    ImGui::SliderInt((std::string(axes[slice.normal])+" sample").c_str(), &layer, 0, static_cast<int>(slice.samples[slice.normal])-1);
    slice = FieldSlice(cells, static_cast<FieldSlice::Plane>(plane), layer, component);
    ImGui::Text("%s = %.2f %s | Horizontal: +%s | Vertical: +%s",
        axes[slice.normal], LengthUnits::fromMeters(slice.position(dx),unit), LengthUnits::symbol(unit), axes[slice.horizontal], axes[slice.vertical]);
    ImGui::Text("E%s: blue -1 | white 0 | red +1 (normalized; colors clip beyond +/-1)", axes[component]);
    ImGui::TextUnformatted("Dark outlines: mapped PEC samples of this component, not exact metal surfaces.");

    // Domain locator: normalized box, not a second field plot.
    auto* draw = ImGui::GetWindowDrawList();
    const ImVec2 locator = ImGui::GetCursorScreenPos();
    auto project = [&](double x,double y,double z) {
        return ImVec2(locator.x+20+100*static_cast<float>(x)+40*static_cast<float>(z),
                      locator.y+90-65*static_cast<float>(y)-25*static_cast<float>(z));
    };
    const ImU32 ink=ImGui::GetColorU32(ImGuiCol_Text);
    for(int axis=0;axis<3;++axis) for(int a=0;a<2;++a) for(int b=0;b<2;++b)
    {
        std::array<double,3> p{}; p[(axis+1)%3]=a; p[(axis+2)%3]=b;
        const auto start=project(p[0],p[1],p[2]); p[axis]=1;
        draw->AddLine(start,project(p[0],p[1],p[2]),ink);
    }
    ImVec2 corners[4];
    for(int k=0;k<4;++k)
    {
        std::array<double,3> p{}; p[slice.normal]=slice.position(dx)/(cells[slice.normal]*dx);
        p[slice.horizontal]=(k==1 || k==2) ? 1 : 0;
        p[slice.vertical]=(k>=2) ? 1 : 0;
        corners[k]=project(p[0],p[1],p[2]);
    }
    draw->AddConvexPolyFilled(corners,4,ImGui::GetColorU32(ImVec4(.3f,.3f,.3f,.18f)));
    draw->AddPolyline(corners,4,ink,ImDrawFlags_Closed,2);
    draw->AddText(project(1,0,0),ink,"X"); draw->AddText(project(0,1,0),ink,"Y"); draw->AddText(project(0,0,1),ink,"Z");
    draw->AddText(ImVec2(locator.x+180,locator.y+25),ink,"Slice location in domain");
    draw->AddText(ImVec2(locator.x+180,locator.y+42),ink,"(schematic)");
    ImGui::Dummy(ImVec2(350,110));

    const auto available = ImGui::GetContentRegionAvail();
    const float pitch = std::min(std::max(1.0f,available.x-12)/cells[slice.horizontal],
                                std::max(200.0f,available.y-35)/cells[slice.vertical]);
    const ImVec2 origin=ImGui::GetCursorScreenPos();
    // Clip half-width endpoint tiles to the physical domain boundary.
    auto edge = [&](std::size_t i,int axis,double side) {
        return std::max(0.0,std::min(static_cast<double>(cells[axis]), i+(axis==component ? .5 : 0)+side));
    };
    for(std::size_t v=0;v<slice.height();++v) for(std::size_t u=0;u<slice.width();++u)
    {
        const auto index=slice.index(u,v);
        const float value=std::max(-1.0f,std::min(controller.sample(index,component),1.0f));
        const float fade=1-std::abs(value);
        const ImU32 color=ImGui::GetColorU32(ImVec4(value<0 ? fade:1,fade,value>0 ? fade:1,1));
        const ImVec2 lo(origin.x+pitch*static_cast<float>(edge(u,slice.horizontal,-.5)),
                        origin.y+pitch*static_cast<float>(cells[slice.vertical]-edge(v,slice.vertical,.5)));
        const ImVec2 hi(origin.x+pitch*static_cast<float>(edge(u,slice.horizontal,.5)),
                        origin.y+pitch*static_cast<float>(cells[slice.vertical]-edge(v,slice.vertical,-.5)));
        draw->AddRectFilled(lo,hi,color);
        if(controller.conductorAt(index,component)) draw->AddRect(lo,hi,ink);
    }
    ImGui::Dummy(ImVec2(pitch*cells[slice.horizontal],pitch*cells[slice.vertical]));
    ImGui::Text("%s: 0 .. %.2f %s | %s: 0 .. %.2f %s",
        axes[slice.horizontal],LengthUnits::fromMeters(cells[slice.horizontal]*dx,unit),LengthUnits::symbol(unit),
        axes[slice.vertical],LengthUnits::fromMeters(cells[slice.vertical]*dx,unit),LengthUnits::symbol(unit));
}
