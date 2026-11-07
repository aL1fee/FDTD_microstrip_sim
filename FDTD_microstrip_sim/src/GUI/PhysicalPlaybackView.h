#pragma once
#include "../Simulation/AsyncCavity.h"
#include "../vendor/imgui/imgui.h"

inline void drawPhysicalPlayback(AsyncCavity& controller)
{
    auto playback=controller.playbackSettings();
    int mode=playback.mode;
    bool changed=ImGui::Combo("Playback units",&mode,"RF cycles / screen second\0Simulated ps / screen second\0");
    playback.mode=static_cast<PhysicalPlayback::Mode>(mode);
    if(mode==PhysicalPlayback::RFCycles)
        changed |= ImGui::InputDouble("Requested cycles/s",&playback.cyclesPerSecond,0,0,"%.6g",ImGuiInputTextFlags_EnterReturnsTrue);
    else
        changed |= ImGui::InputDouble("Requested ps/s",&playback.picosecondsPerSecond,0,0,"%.6g",ImGuiInputTextFlags_EnterReturnsTrue);
    double GHz=playback.referenceHz*1e-9;
    changed |= ImGui::InputDouble("Playback reference (GHz)",&GHz,0,0,"%.6g",ImGuiInputTextFlags_EnterReturnsTrue);
    playback.referenceHz=GHz*1e9;
    if(changed) {
        try { controller.setPlayback(playback); }
        catch(const std::exception&) { ImGui::TextUnformatted("Enter finite, positive playback values."); }
    }
    playback=controller.playbackSettings();
    ImGui::TextWrapped("Reference converts cycles to physical time; it does not change the RF source. "
        "Reset selects the first applied source frequency, the analytical cavity frequency, or 1 GHz for a pulse.");
    if(!controller.ready()) return;
    const double requested=playback.simulatedSecondsPerSecond();
    const double steps=controller.achievedStepsPerSecond();
    const double achieved=steps*controller.timestep();
    ImGui::Text("Requested: %.6g cycles/s | %.6g ps/s", requested*playback.referenceHz,requested*1e12);
    ImGui::Text("Achieved: %.6g cycles/s | %.6g ps/s", achieved*playback.referenceHz,achieved*1e12);
    ImGui::Text("Steps/s: %.6g requested | %.6g achieved",playback.stepsPerSecond(controller.timestep()),steps);
    ImGui::Text("Physical timestep: %.6g ps (unchanged)",controller.timestep()*1e12);
    if(!controller.isRunning()) ImGui::TextUnformatted("Paused. Achieved playback speed is zero.");
    else if(controller.playbackElapsed()<.5) ImGui::TextUnformatted("Measuring achieved speed...");
    else if(achieved<.9*requested)
        ImGui::TextWrapped("Below requested speed: worker workload / CPU duty limit / scheduling. "
            "Physical timesteps are never enlarged or skipped to catch up.");
    else ImGui::TextUnformatted("Tracking requested playback speed.");
    ImGui::TextWrapped("Achieved speed averages completed playback steps since Run or the last playback edit. "
        "The worker retains its 50% duty limit; rendering uses additional CPU time.");
}
