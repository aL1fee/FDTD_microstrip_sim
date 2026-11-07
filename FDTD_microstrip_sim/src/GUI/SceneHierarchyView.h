#pragma once
#include "../Scene/MainScene.h"
#include "../vendor/imgui/imgui.h"
#include <cstdio>

struct SceneHierarchyState
{
    bool editing=false, focusEditor=false;
    unsigned int objectId=0;
    char name[256]{};
};

inline void drawSceneHierarchy(MainScene& scene, SceneHierarchyState& state)
{
    const auto objects=scene.getSceneObjects();
    if(state.editing && std::none_of(objects.begin(),objects.end(),[&](PhysicalObject* object) { return object->getId()==state.objectId; }))
        state.editing=false;
    const bool wasEditing=state.editing;
    if(ImGui::TreeNodeEx("Scene objects",ImGuiTreeNodeFlags_DefaultOpen)) {
        if(objects.empty()) ImGui::TextDisabled("No objects. Use Add to create a model.");
        for(auto* object : objects) {
            ImGui::PushID(static_cast<int>(object->getId()));
            if(state.editing && state.objectId==object->getId()) {
                if(state.focusEditor) { ImGui::SetKeyboardFocusHere(); state.focusEditor=false; }
                ImGui::SetNextItemWidth(-1);
                const bool cancel=ImGui::IsKeyPressed(ImGuiKey_Escape,false);
                const bool enter=ImGui::InputText("##name",state.name,sizeof(state.name),
                    ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
                if(cancel) state.editing=false;
                else if(enter || ImGui::IsItemDeactivated()) {
                    object->setDisplayName(state.name);
                    state.editing=false;
                }
            } else {
                // Stable identity, including when names are duplicated or contain ##.
                if(ImGui::Selectable("##object",scene.getActiveObject()==object,ImGuiSelectableFlags_AllowDoubleClick)) {
                    scene.selectObjectById(object->getId());
                    if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        state.editing=true; state.focusEditor=true; state.objectId=object->getId();
                        std::snprintf(state.name,sizeof(state.name),"%s",object->getDisplayName().c_str());
                    }
                }
                ImGui::GetWindowDrawList()->AddText(ImGui::GetItemRectMin(),ImGui::GetColorU32(ImGuiCol_Text),object->getDisplayName().c_str());
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    } else state.editing=false;
    auto* object=scene.getActiveObject();
    const auto& io=ImGui::GetIO();
    // Hierarchy focus legitimately captures keyboard input from the viewport.
    // Handle these commands here, but never steal keys from an active editor.
    if(object && !wasEditing && !state.editing && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
       !io.WantTextInput && !ImGui::IsAnyItemActive() && !io.KeyCtrl && !io.KeyAlt && !io.KeySuper) {
        if(ImGui::IsKeyPressed(ImGuiKey_R,false)) scene.showModifyingVectors(object,Rotation);
        if(ImGui::IsKeyPressed(ImGuiKey_T,false)) scene.showModifyingVectors(object,Translation);
        if(ImGui::IsKeyPressed(ImGuiKey_Y,false)) scene.showModifyingVectors(object,Scaling);
    }
}
