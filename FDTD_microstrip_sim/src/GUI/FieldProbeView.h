#pragma once
#include "LengthUnits.h"
#include "../Simulation/PropagationValidation.h"
#include "../vendor/imgui/imgui.h"

inline void drawFieldProbes(CavityController& controller,LengthUnits::Unit unit)
{
    if(!ImGui::TreeNode("Field probes (this simulation)")) return;
    if(!controller.ready())
    {
        ImGui::TextWrapped("Probe controls become available after a successful Apply / Reset in this window. "
            "Choose a valid grid and excitation above, resolve any displayed setup error, then click Apply / Reset. "
            "Running the independent propagation reference check does not initialize your scene simulation.");
        ImGui::TreePop();
        return;
    }
    ImGui::TextWrapped("Each probe records one signed electric component at a fixed Yee sample on every solver step. "
        "Editing probe positions clears their histories but does not reset the fields.");
    int component=controller.probes()[0].component;
    bool changed=ImGui::Combo("Probe component",&component,"Ex\0Ey\0Ez\0");
    int indices[2][3];
    for(int p=0;p<2;++p) for(int axis=0;axis<3;++axis) indices[p][axis]=static_cast<int>(controller.probes()[p].index[axis]);
    changed |= ImGui::InputInt3("Probe 1 (i,j,k)",indices[0],ImGuiInputTextFlags_EnterReturnsTrue);
    changed |= ImGui::InputInt3("Probe 2 (i,j,k)",indices[1],ImGuiInputTextFlags_EnterReturnsTrue);
    if(changed)
    {
        Yee3D::Index first{},second{};
        for(int axis=0;axis<3;++axis)
        {
            const int maximum=static_cast<int>(controller.cells()[axis])-(axis==component ? 1 : 0);
            first[axis]=std::max(0,std::min(maximum,indices[0][axis]));
            second[axis]=std::max(0,std::min(maximum,indices[1][axis]));
        }
        controller.configureProbes(static_cast<Yee3D::Axis>(component),first,second);
    }
    float scale=1e-6f;
    for(const auto& probe : controller.probes()) for(std::size_t i=0;i<probe.count;++i) scale=std::max(scale,std::abs(probe.at(i)));
    for(int p=0;p<2;++p)
    {
        const auto& probe=controller.probes()[p];
        double position[3];
        for(int axis=0;axis<3;++axis) position[axis]=LengthUnits::fromMeters((probe.index[axis]+(axis==probe.component ? .5:0))*controller.spacing(),unit);
        ImGui::Text("Probe %d: (%.2f, %.2f, %.2f) %s | E%c = %.6g",p+1,position[0],position[1],position[2],
            LengthUnits::symbol(unit),"xyz"[probe.component],probe.at(probe.count-1));
        if(controller.conductorAt(probe.index,probe.component)) ImGui::TextUnformatted("Probe is inside mapped PEC: this component is held at zero.");
        ImGui::PlotLines(p==0 ? "##probe1" : "##probe2",probe.values.data(),static_cast<int>(probe.count),static_cast<int>(probe.offset()),
            nullptr,-scale,scale,ImVec2(-1,90));
    }
    const auto& p=controller.probes()[0];
    ImGui::Text("Time: %.3f .. %.3f ps | Shared vertical scale: +/- %.6g",
        p.firstStep()*controller.timestep()*1e12,p.lastStep*controller.timestep()*1e12,scale);
    ImGui::TextWrapped("Last 2048 steps retained. These are local field histories, not port voltage or power. "
        "CW phase, near fields and reflected waves can complicate travel-time interpretation.");
    ImGui::TreePop();
}

inline void drawPropagationValidation(PropagationValidation& check)
{
    check.advance(2); // bounded work per frame; no changes to the user's scene or solver
    if(!ImGui::TreeNode("Propagation reference check")) return;
    ImGui::TextWrapped("Controlled pulse test with two probes 10000 um apart. Runs vacuum and epsilon_r=4 at "
        "500 and 250 um spacing, before wall reflections reach the probes. This checks the solver and probe measurements; "
        "it does not validate your microstrip or calibrate Scene sources.");
    if(check.running())
    {
        ImGui::Text("Case %zu of 4 | Step %d / %d",check.completed().size()+1,check.stepsDone(),check.stepsRequired());
        if(ImGui::Button("Cancel reference check")) check.cancel();
    }
    else if(ImGui::Button("Run reference check")) check.start();
    if(!check.error().empty()) ImGui::TextWrapped("%s",check.error().c_str());
    for(const auto& result : check.completed())
    {
        ImGui::Text("eps %.0f, dx %.0f um: %s | delay error %.3f%%",result.epsilon,result.spacing*1e6,result.passed ? "PASS (<2%)" : "FAIL",100*result.relativeError);
        ImGui::Text("  Measured %.4f ps | Expected %.4f ps | Speed %.6g m/s",result.measuredDelay*1e12,result.expectedDelay*1e12,result.speed);
    }
    if(check.completed().size()==4)
    {
        const auto& r=check.completed();
        ImGui::Text("Refinement reduces error: vacuum %s, dielectric %s",
            r[1].relativeError<r[0].relativeError ? "YES":"NO",r[3].relativeError<r[2].relativeError ? "YES":"NO");
    }
    if(!check.completed().empty())
    {
        const auto& r=check.completed().back();
        ImGui::TextUnformatted("Most recent case: probe 1 (upper), probe 2 (lower), normalized Ez");
        for(int p=0;p<2;++p) ImGui::PlotLines(p==0 ? "##reference1" : "##reference2",r.traces[p].data(),static_cast<int>(r.traces[p].size()),0,
            nullptr,-.6f,.6f,ImVec2(-1,70));
        ImGui::Text("Time: 0 .. %.3f ps",(r.traces[0].size()-1)*r.timestep*1e12);
    }
    ImGui::TreePop();
}
