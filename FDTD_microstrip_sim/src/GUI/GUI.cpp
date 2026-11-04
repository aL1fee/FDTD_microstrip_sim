#include "GUI.h"
#include "LengthUnits.h"
#include "FieldSliceView.h"
#include "../Simulation/SceneCavitySetup.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace
{
    LengthUnits::Unit lengthUnit = LengthUnits::Unit::Micrometer;
    bool inputSceneLength(const char* label, float* sceneValue, const char* format = "%.2f")
    {
        float displayed = static_cast<float>(LengthUnits::fromScene(*sceneValue, lengthUnit));
        if (!ImGui::InputFloat(label, &displayed, 0, 0, format, ImGuiInputTextFlags_EnterReturnsTrue))
            return false;
        if (!std::isfinite(displayed)) return false;
        *sceneValue = static_cast<float>(LengthUnits::toScene(displayed, lengthUnit));
        return true;
    }
}

extern MainScene* _scene_main;
extern SimulationSpace* _simulation_space;
extern bool _propertyWindowOn;

extern bool _windowS1On;
extern bool _windowS2On;

GUI::GUI(GLFWwindow* w)
{
	window = w;
	statusWindow = new StatusWindow("...");
	firstRightPanelMinimized = true;
	propertyWindowMinimized = true;
	customFont = nullptr;
}


void GUI::buildMainMenuPanel()
{
	float windowWidth = ImGui::GetIO().DisplaySize.x;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 6)); // Adjust the second value to reduce height
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));  // Adjust the second value to reduce spacing
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(windowWidth, 20));  // Adjust the second value to reduce spacing
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, 18));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.9607f, 0.9607f, 0.9607f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.9607f, 0.9607f, 0.9607f, 1.0f));

	ImGui::Begin("CustomMenuBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) { /* Open action */ }
			if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Open action */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Save action */ }
			if (ImGui::MenuItem("Exit", "Alt+F4")) { /* Save action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Undo action */ }
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Redo action */ }
			if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* Redo action */ }
			if (ImGui::MenuItem("Paste", "Ctrl+P")) { /* Redo action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::BeginMenu("Adjust clipping plane")) 
			{ 
				if (ImGui::BeginMenu("Modify near-plane"))
				{
					float* near_plane_value = _scene_main->getNearPlaneValue();
					ImGui::SliderFloat("x", near_plane_value, .01f, 1.15f);
					_scene_main->updateProjMatrix();
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Modify far-plane"))
				{
					float* far_plane_value = _scene_main->getFarPlaneValue();
					ImGui::SliderFloat("x", far_plane_value, 50.0f, 300.0f);
					_scene_main->updateProjMatrix();
					ImGui::EndMenu();

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::BeginMenu("Length units"))
            {
                for (auto unit : {LengthUnits::Unit::Mil, LengthUnits::Unit::Millimeter, LengthUnits::Unit::Micrometer})
                    if (ImGui::MenuItem(LengthUnits::symbol(unit), nullptr, lengthUnit == unit))
                        lengthUnit = unit;
                ImGui::EndMenu();
            }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Commands")) { /* Undo action */ }
			if (ImGui::MenuItem("Theory of operation"))
			{
				theoryOpen = true;
				ImGui::SetWindowFocus("Theory of Operation");
			}
			if (ImGui::MenuItem("About")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(3);


}

void GUI::buildMenuUpperPanel()
{
	float windowWidth = ImGui::GetIO().DisplaySize.x;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 10)); // Adjust the second value to reduce height
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));  // Adjust the second value to reduce spacing
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(windowWidth, 30));  // Adjust the second value to reduce spacing
	ImGui::SetNextWindowPos(ImVec2(0, 23));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, 28));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.97f, 0.97f, 0.97f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.97f, 0.97f, 0.97f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.9607f, 0.9607f, 0.9607f, 1.0f));

	ImGui::Begin("UpperMenuBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Add"))
		{
			std::string s = "";
			if (ImGui::BeginMenu("Wire"))
			{
				if (ImGui::MenuItem("Image plane"))
				{
					_wireInputExpected = true;
					s = "> Adding an image plane wire";
					_scene_main->addWire(s, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f,
						glm::vec3(1.0f, .843f, 0.0f), 25, 560);
				}
				if (ImGui::MenuItem("XY plane"))
				{
					_wireInputExpected = true;
					s = "> Adding an XY plane wire";
					_wireXYplane = true;
					_scene_main->addWire(s, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f,
						glm::vec3(1.0f, .843f, 0.0f), 25, 560);
				}
				if (ImGui::MenuItem("ZY plane"))
				{
					_wireInputExpected = true;
					s = "> Adding a ZY plane wire";
					_wireZYplane = true;
					_scene_main->addWire(s, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f,
						glm::vec3(1.0f, .843f, 0.0f), 25, 560);
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Ribbon (in x-z plane)"))
			{
				extern bool _ribbonInputExpected;
				_ribbonInputExpected = true;

				s = "> Adding a ribbon";
				_scene_main->addRibbon(s, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f,
					glm::vec3(.95f, .69f, 0.21f), 25, 560);


			}
			if (ImGui::MenuItem("Carrier"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a carrier";
				_scene_main->addCarrier(s, glm::vec3(0.5f, 0.2f, .5f), 2.0f, 1.0f, 0.45f,
					glm::vec3(1.0f, 1.0f, .66f), 1500, 600);

				//_scene_main->addCarrier(s, glm::vec3(0.0f, 0.0f, .0f), 2.0f, 1.0f, 0.45f,
					//glm::vec3(1.0f, 1.0f, .66f), 1500, 600);


				//_scene_main->addCarrier(s, glm::vec3(0.5f, 0.2f, .5f), 4.5f, 7.0f, 0.45f,
				//	glm::vec3(1.0f, 1.0f, .66f), 1500, 600);



			}
			if (ImGui::MenuItem("Substrate"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a substrate";
				_scene_main->addSubstrate(s, glm::vec3(0.0f), 4.5f, 1.0f, 0.6f,
					glm::vec3(.4f, .8f, 1.0f), .00001f, 3.05f);
			}
			if (ImGui::MenuItem("Trace"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a trace";
				_scene_main->addTrace(s, glm::vec3(0.0f), 4.5f, .15f, 0.01f,
					glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410);
			}
			if (ImGui::MenuItem("Housing"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a housing";
				_scene_main->addHousing(s, glm::vec3(0.0f, 0.0f, -.2f), 9.0f, .2f, 2.0f,
					glm::vec3(.88f, .88f, .88f), 10.0f, 50);
			}
			if (ImGui::MenuItem("Power source"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a power source";
				_scene_main->addPowerSource(s, glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(1.0, 0.0f, 0.0f), -10.0f, .2f, .6f,
					glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410);
			}
			if (ImGui::MenuItem("Power detector"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a power detector";
				_scene_main->addPowerDetector(s, glm::vec3(10.0f, 1.16f, 3.49f),
					glm::vec3(1.0, 0.0f, 0.0f), 25.5f, .1f, .2f,
					glm::vec3(1.0f, 1.0f, 0.0f), 5.2f, 410);
			}
			if (ImGui::MenuItem("Tuning pad array"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a tuning pad array";
				_scene_main->addTuningPadArray(s, glm::vec3(0.0f, .0f, 1.0), 19, 2,
					.15f, .1f, .08f, .08f, glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410);
				//_scene_main->addTuningPadArray(s, glm::vec3(0.0f), 3, 2, .2f, .2f, .2f, .2f,
				//	glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410);
			}
			if (ImGui::MenuItem("System #1"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a system";
				_scene_main->addCarrier(s, glm::vec3(0.0f), 4.5f, 7.0f, 0.45f,
					glm::vec3(1.0f, 1.0f, .66f), 1500, 600);
				_scene_main->addSubstrate(s, glm::vec3(0.0f, .45f, 3.0f), 4.5f, 1.0f, 0.6f,
					glm::vec3(.4f, .8f, 1.0f), .00001f, 3.05f);
				_scene_main->addTrace(s, glm::vec3(0.0f, 1.05f, 3.42f), 4.5f, .15f, 0.01f,
					glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410);
				s = "> Adding a housing";
				_scene_main->addHousing(s, glm::vec3(0.0f, 0.0f, -.2f), 9.0f, .2f, 2.0f,
					glm::vec3(.88f, .88f, .88f), 10.0f, 50);
			}
			if (ImGui::MenuItem("System #2"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a system";
				_scene_main->addCarrier(s, glm::vec3(0.0f), 4.5f, 7.0f, 0.45f,
					glm::vec3(1.0f, 1.0f, .66f), 1500, 600);
				_scene_main->addSubstrate(s, glm::vec3(0.0f, .45f, 3.0f), 4.5f, 1.0f, 0.6f,
					glm::vec3(.4f, .8f, 1.0f), .00001f, 3.05f);
				_scene_main->addTrace(s, glm::vec3(0.0f, 1.05f, 3.42f), 4.5f, .15f, 0.01f,
					glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410);
				_scene_main->addPowerSource(s, glm::vec3(0.0f, 1.16f, 3.49f),
					glm::vec3(1.0, 0.0f, 0.0f), -10.0f, .1f, .2f,
					glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410);
				_scene_main->addTuningPadArray(s, glm::vec3(.14f, 1.05, 3.07), 19, 2,
					.15f, .1f, .08f, .08f, glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410);
				_scene_main->addTuningPadArray(s, glm::vec3(.14f, 1.05, 3.66), 19, 2,
					.15f, .1f, .08f, .08f, glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410);

				_scene_main->addHousing(s, glm::vec3(-0.2f, 0.0f, 0.0f), .2f, 7.0f, 1.4f,
					glm::vec3(.88f, .88f, .88f), 10.0f, 50);

			}
			statusWindow->setMessage(s);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Select"))
		{
			if (ImGui::MenuItem("Testing line"))
			{
				extern bool _acceptingLeftClickBufferInput;
				extern bool _testingLineExpected;
				_testingLineExpected = true;

			}
			if (ImGui::MenuItem("Generate a ray"))
			{
				extern bool _rayExpected;
				_rayExpected = true;
			}
			if (ImGui::MenuItem("Select an object"))
			{
				extern bool _selectingObjectExpected;
				_selectingObjectExpected = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Delete"))
		{
			if (ImGui::MenuItem("Delete the object", "del"))
			{
				_scene_main->deleteActiveObject();
			}
			if (ImGui::MenuItem("Delete all objects"))
			{
				_scene_main->deleteAllObjects();
			}
			if (ImGui::MenuItem("Delete all lines"))
			{
				_scene_main->clearLines();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Modify"))
		{
			if (ImGui::MenuItem("Rotate an object", "R"))
			{
				PhysicalObject* obj = _scene_main->getActiveObject();
				// redundant check
				if (obj == nullptr) {
					string s = "> No object selected!\n";
					statusWindow->setMessage(s);
				}
				else {
					_scene_main->showModifyingVectors(obj, Rotation);
				}
			}
			if (ImGui::MenuItem("Translate an object", "T"))
			{
				PhysicalObject* obj = _scene_main->getActiveObject();
				// redundant check
				if (obj == nullptr) {
					string s = "> No object selected!\n";
					statusWindow->setMessage(s);
				}
				else {
					_scene_main->showModifyingVectors(obj, Translation);
				}
			}
			if (ImGui::MenuItem("Scale an object", "Y"))
			{
				PhysicalObject* obj = _scene_main->getActiveObject();
				// redundant check
				if (obj == nullptr) {
					string s = "> No object selected!\n";
					statusWindow->setMessage(s);
				}
				else {
					_scene_main->showModifyingVectors(obj, Scaling);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(3);

}

void GUI::buildMenuLowerPanel()
{
	float windowWidth = ImGui::GetIO().DisplaySize.x;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 10)); // Adjust the second value to reduce height
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));  // Adjust the second value to reduce spacing
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(windowWidth, 30));  // Adjust the second value to reduce spacing
	ImGui::SetNextWindowPos(ImVec2(0, 54));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, 28));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.97f, 0.97f, 0.97f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.97f, 0.97f, 0.97f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.9607f, 0.9607f, 0.9607f, 1.0f));

	ImGui::Begin("LowerMenuBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		std::string simDimStr = std::to_string(
			*_simulation_space->getSimulationDimension());
		std::string simTitle = "Simulation " + simDimStr + "D";
		bool oldSimulationRunning = _simulation_space->isOldSimulationRunning();
		ImGui::BeginDisabled(oldSimulationRunning);
		if (ImGui::BeginMenu(simTitle.c_str()))
		{
			int* option = _simulation_space->getSimulationDimension();
			if (ImGui::MenuItem("1D", nullptr, *option == 1))
				*option = 1;

			if (ImGui::MenuItem("2D", nullptr, *option == 2))
				*option = 2;

			if (ImGui::MenuItem("3D", nullptr, *option == 3))
				*option = 3;

			ImGui::EndMenu();
		}
		ImGui::EndDisabled();

		ImGui::Separator();

		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::BeginDisabled(oldSimulationRunning);
			if (ImGui::BeginMenu("Simulation dimensions"))
			{
				glm::vec3* currDims = _simulation_space->getDimensions();

				float simulationDimensions[3] = { static_cast<float>(LengthUnits::fromScene(currDims->x, lengthUnit)),
                    static_cast<float>(LengthUnits::fromScene(currDims->y, lengthUnit)),
                    static_cast<float>(LengthUnits::fromScene(currDims->z, lengthUnit)) };

				if (ImGui::SliderFloat3((std::string("Dimensions (") + LengthUnits::symbol(lengthUnit) + ")").c_str(), simulationDimensions, 0.0f, static_cast<float>(LengthUnits::fromScene(20, lengthUnit)), "%.2f")) {
					currDims->x = static_cast<float>(LengthUnits::toScene(simulationDimensions[0], lengthUnit));
					currDims->y = static_cast<float>(LengthUnits::toScene(simulationDimensions[1], lengthUnit));
					currDims->z = static_cast<float>(LengthUnits::toScene(simulationDimensions[2], lengthUnit));
					_simulation_space->setCellUpdate(true);
				}
				ImGui::EndMenu();
			}
			ImGui::EndDisabled();
			if (ImGui::MenuItem("Cell Toggle ON/OFF"))
			{
				_simulation_space->setRenderingCell(!_simulation_space->
					getRenderingCellOn());
			}
			ImGui::BeginDisabled(oldSimulationRunning);
			if (ImGui::BeginMenu("Cell size"))
			{
				float* cellSize = _simulation_space->getCellSize();
				float displayedSize = static_cast<float>(LengthUnits::fromScene(*cellSize, lengthUnit));
                if (ImGui::SliderFloat((std::string("Cell size (") + LengthUnits::symbol(lengthUnit) + ")").c_str(), &displayedSize,
                    static_cast<float>(LengthUnits::fromScene(.02, lengthUnit)), static_cast<float>(LengthUnits::fromScene(.5, lengthUnit)), "%.2f"))
				{
                    *cellSize = static_cast<float>(LengthUnits::toScene(displayedSize, lengthUnit));
					_simulation_space->setCellUpdate(true);
				}
				ImGui::EndMenu();
			}
			ImGui::EndDisabled();
			if (ImGui::BeginMenu("Cell color"))
			{
				glm::vec3* currColor = _simulation_space->getCellColor();

				float simulationDimensions[3] = { currColor->x, currColor->y, currColor->z };

				if (ImGui::SliderFloat3("Color RGB", simulationDimensions, 0.0f, 1.0f, "%.2f")) {
					currColor->x = simulationDimensions[0];
					currColor->y = simulationDimensions[1];
					currColor->z = simulationDimensions[2];
					_simulation_space->setCellColor(*currColor);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Cell opaqueness"))
			{
				float* currCellOpaqueness = _simulation_space->getCellOpaqueness();

				if (ImGui::SliderFloat("Cell opaqueness", currCellOpaqueness, 0.0f, 1.0f, "%.2f")) {
					_simulation_space->setCellOpaqueness(*currCellOpaqueness);
				}

				ImGui::EndMenu();
			}
            if (ImGui::BeginMenu("Observation plane"))
            {
                ImGui::Checkbox("Show in scene", &observationPlaneVisible);
                ImGui::Checkbox("Show active solver boundary", &solverBoundaryVisible);
                ImGui::SliderFloat("Opacity", &observationPlaneOpacity, 0.0f, 1.0f, "%.2f");
                ImGui::TextUnformatted("0 = transparent, 1 = opaque");
                ImGui::TextWrapped("Follows the active 3D field slice. It does not affect the simulation.");
                if (!cavity.ready()) ImGui::TextUnformatted("Open slice controls and Apply / Reset to initialize a slice.");
                if (ImGui::MenuItem("Open slice controls")) cavityOpen = true;
                ImGui::EndMenu();
            }

			ImGui::EndMenu();
		}

		// do enums or switches instead for 1,2,3d
		if (ImGui::BeginMenu("Execute"))
		{
			int* option = _simulation_space->getSimulationDimension();

		if (*option == 3 && ImGui::MenuItem("Open 3D vacuum cavity"))
			cavityOpen = true;

			if (ImGui::MenuItem("Toggle ON/OFF"))
			{
				if (*option == 1)
				{
					_simulation_space->setDrawing1D(
						!_simulation_space->isDrawing1D());

				}

			}
			if (ImGui::MenuItem("Reset"))
			{
				if (*option == 1)
				{
					_simulation_space->reset1D();
				}

			}
			if (ImGui::MenuItem("Run"))
			{
				if (*option == 1)
				{
					_simulation_space->setRunning1D(true);
				}

			}
			if (ImGui::MenuItem("Stop"))
			{
				if (*option == 1)
				{
					_simulation_space->setSimStopped(true);
					_simulation_space->setRunning1D(false);
				}
			}


			ImGui::EndMenu();
		}




		if (ImGui::BeginMenu("3"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Open action */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Save action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("4"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Undo action */ }
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Redo action */ }
			ImGui::EndMenu();
		}
		ImGui::Separator();
		if (ImGui::BeginMenu("5"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Open action */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Save action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("6"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Undo action */ }
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Redo action */ }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(3);

}

void GUI::buildLeftPanel()
{
	float windowHeight = ImGui::GetIO().DisplaySize.y;
	ImGui::SetNextWindowPos(ImVec2(0, 85));
	ImGui::SetNextWindowSize(ImVec2(256, windowHeight));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.976f, 0.976f, 0.976f, 1.0f));

	ImGui::Begin("LeftPanel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::End();
	ImGui::PopStyleColor();

	// Scene hierarchy
	ImGui::SetNextWindowPos(ImVec2(4, 89));
	ImGui::SetNextWindowSize(ImVec2(248, (windowHeight - 85 - 12) / 2));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4)); // Adjust the second value to reduce height
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 4));  // Adjust the second value to reduce spacing
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	
	ImGui::Begin("Scene hierarchy", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove);

	ImGui::Text("Scene hierarchy");

	if (ImGui::TreeNode("Root Node")) {
		if (ImGui::TreeNode("Child Node 1")) {
			ImGui::Text("This is Child Node 1");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Child Node 2")) {
			ImGui::Text("This is Child Node 2");
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(2);

	// Property
	ImGui::SetNextWindowPos(ImVec2(4, 89 + 5 + (windowHeight - 85 - 12) / 2));
	ImGui::SetNextWindowSize(ImVec2(248, (windowHeight - 85 - 12) / 2));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4)); // Adjust the second value to reduce height
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 4));  // Adjust the second value to reduce spacing
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));

	ImGui::Begin("Property window", nullptr, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove);
	// removed ImGuiWindowFlags_NoTitleBar

	if (_propertyWindowOn)
	{
		if (ImGui::BeginTable("PropertiesTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
			// Setup columns
			ImGui::TableSetupColumn("Property");
			ImGui::TableSetupColumn("Value");
			ImGui::TableHeadersRow();

			// Example properties
			//const char* properties[] = { "Width", "Height", "Depth" };
			//static float values[] = { 100.0f, 200.0f, 300.0f };
			PropertyWindow* propertyWindow = _scene_main->getPropertyWindow();
			std::vector<std::pair<std::string, float*>>* properties = propertyWindow->getProperties();
			std::vector<std::pair<std::string, int*>>* propertiesInt = propertyWindow->getPropertiesInt();
			PhysicalObject* activeObj = propertyWindow->getActiveObject();

			if (properties != nullptr && activeObj != nullptr)
			{
				for (auto it = properties->begin(); it != properties->end(); ++it) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
					ImGui::Text("%s", LengthUnits::displayName(it->first, lengthUnit).c_str());
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-1); // Make the input field take the full width of the column
					float* propertyValue = it->second;
					const std::string propertyName = LengthUnits::propertyName(it->first);
                    const bool edited = LengthUnits::isSceneLength(it->first)
                        ? inputSceneLength(("##value" + it->first).c_str(), propertyValue)
                        : ImGui::InputFloat(("##value" + it->first).c_str(), propertyValue, 0, 0, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue);
                    if (edited) {
						// TODO only rebuild the object which property had been updated
						if (propertyName == "X position" || propertyName == "Y position" ||
							propertyName == "Z position")
						{
							activeObj->setModelMatrixRegenExpected(true);
						}					
						if (propertyName == "Length" || propertyName == "Width" ||
							propertyName == "Height" || propertyName == "Radius") {
							if (*propertyValue < MIN_OBJECT_SIZE) {
								*propertyValue = MIN_OBJECT_SIZE;
							}
							if (*propertyValue > MAX_OBJECT_SIZE) {
								*propertyValue = MAX_OBJECT_SIZE;
							}
							activeObj->setModelMatrixRegenExpected(true);
						}
						if (propertyName == "X pad separ." || propertyName == "Z pad separ." ||
							propertyName == "X pad size" || propertyName == "Z pad size") {
							if (*propertyValue < MIN_PAD_SEPARATION_SIZE) {
								*propertyValue = MIN_PAD_SEPARATION_SIZE;
							}
							if (*propertyValue > MAX_PAD_SEPARATION_SIZE) {
								*propertyValue = MAX_PAD_SEPARATION_SIZE;
							}
							activeObj->setInstancePositionsChanged(true);
							activeObj->updateL();
							activeObj->updateW();
						}
						if (propertyName == "Diameter") {
							if (*propertyValue < WIRE_MIN_DIAMETER) {
								*propertyValue = static_cast<float>(WIRE_MIN_DIAMETER);
							}
							if (*propertyValue > WIRE_MAX_DIAMETER) {
								*propertyValue = static_cast<float>(WIRE_MAX_DIAMETER);
							}
						}
						if (propertyName == "Rotation angle")
						{
							if (*propertyValue < 0.0f) {
								*propertyValue = std::fmod(*propertyValue, 360.0f);
								*propertyValue += 360.0f;
							}
							if (*propertyValue > 360.0f) {
								*propertyValue = std::fmod(*propertyValue, 360.0f);
							}
						}
						if (propertyName == "Curve length" || propertyName == "Tail length" ||
							propertyName == "Curve height" || propertyName == "Thickness")
						{
							activeObj->setModelVertsRegenExpected(true);

							if (propertyName == "Curve length")
							{
								activeObj->setScaleL(*propertyValue);
							}
							else if (propertyName == "Curve height")
							{
								if (*propertyValue < RIBBON_MIN_CURVE_HEIGHT) {
									*propertyValue = static_cast<float>(RIBBON_MIN_CURVE_HEIGHT);
								}
								if (*propertyValue > RIBBON_MAX_CURVE_HEIGHT) {
									*propertyValue = static_cast<float>(RIBBON_MAX_CURVE_HEIGHT);
								}
							}
							else if (propertyName == "Tail length")
							{
								if (*propertyValue < RIBBON_MIN_CURVE_TAIL) {
									*propertyValue = static_cast<float>(RIBBON_MIN_CURVE_TAIL);
								}
								if (*propertyValue > RIBBON_MAX_CURVE_TAIL) {
									*propertyValue = static_cast<float>(RIBBON_MAX_CURVE_TAIL);
								}
							}
							else if (propertyName == "Thickness")
							{
								if (*propertyValue < RIBBON_MIN_CURVE_THICKNESS) {
									*propertyValue = static_cast<float>(RIBBON_MIN_CURVE_THICKNESS);
								}
								if (*propertyValue > RIBBON_MAX_CURVE_THICKNESS) {
									*propertyValue = static_cast<float>(RIBBON_MAX_CURVE_THICKNESS);
								}
							}
							activeObj->doBeforeGUIPropertyChange();
						}
						activeObj->setRebuiltExpected(true);
						ModifyingVectors_PO* modVecs = _scene_main->getModifyingVectors();
						if (modVecs != nullptr)
						{
							_scene_main->getModifyingVectors()->setRebuiltExpected(true);
							_scene_main->getModifyingVectors()->setOrigin(activeObj->getCenterLocation());
							_scene_main->getModifyingVectors()->setRotationAngle(*activeObj->getRotationAngle());
							_scene_main->getModifyingVectors()->generateModelMatrix();
						}

						std::cout << "One of the properties has been updated!" << std::endl;
					}
				}
			}
			if (propertiesInt != nullptr && activeObj != nullptr)
			{
				for (auto it = propertiesInt->begin(); it != propertiesInt->end(); ++it) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("%s", it->first.c_str());
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-1); // Make the input field take the full width of the column
					int* propertyValue = it->second;
					const bool edited = it->first == "3D polarization"
                        ? ImGui::Combo(("##value" + it->first).c_str(), propertyValue, "X\0Y\0Z\0")
                        : ImGui::InputScalar(("##value" + it->first).c_str(), ImGuiDataType_S32, propertyValue,
                            nullptr, nullptr, "%d", ImGuiInputTextFlags_EnterReturnsTrue);
                    if (edited) {
						// TODO only rebuild the object which property had been updated
						if (it->first == "X number of pads" || it->first == "Z number of pads")
						{
							if (*propertyValue < MIN_NUM_PADS) {
								*propertyValue = MIN_NUM_PADS;
							}
							if (*propertyValue > MAX_NUM_PADS) {
								*propertyValue = MAX_NUM_PADS;
							}
							activeObj->setInstancePositionsChanged(true);
							activeObj->updateL();
							activeObj->updateW();
						}
						activeObj->setRebuiltExpected(true);
						ModifyingVectors_PO* modVecs = _scene_main->getModifyingVectors();
						if (modVecs != nullptr)
						{
							_scene_main->getModifyingVectors()->setRebuiltExpected(true);
							_scene_main->getModifyingVectors()->setOrigin(activeObj->getCenterLocation());
						}

						std::cout << "One of the properties has been updated!" << std::endl;
					}
				}
			}


			ImGui::EndTable();
		}
	}

	ImGui::End();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(2);

	propertyWindowMinimized = ImGui::IsWindowCollapsed();

	//if (!propertyWindowMinimized)
	//{
	//	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);

	//	ImGui::SetNextWindowPos(ImVec2(4, 112 + 5 + (windowHeight - 85 - 12) / 2));
	//	ImGui::SetNextWindowSize(ImVec2(248, (windowHeight - 85 - 12 - 46) / 2));
	//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4)); // Adjust the second value to reduce height
	//	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 4));  // Adjust the second value to reduce spacing
	//	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	//	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	//	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	//	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));

	//	ImGui::Begin("Simulation window", nullptr, ImGuiWindowFlags_NoResize |
	//		ImGuiWindowFlags_NoMove);
	//	// removed ImGuiWindowFlags_NoTitleBar

	//	ImGui::End();
	//	ImGui::PopStyleColor(4);
	//	ImGui::PopStyleVar(2);
	//}




}

void GUI::buildLowerPanel() {
	float windowWidth = ImGui::GetIO().DisplaySize.x;
	float windowHeight = ImGui::GetIO().DisplaySize.y;
	if (windowHeight < 300) {
		return;
	}
	ImGui::SetNextWindowPos(ImVec2(255, windowHeight - 160));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.976f, 0.976f, 0.976f, 1.0f));

	ImGui::Begin("LowerPanel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	ImGui::SetNextWindowPos(ImVec2(259, windowHeight - 156));
	ImGui::SetNextWindowSize(ImVec2(windowWidth - 259 - 3, 153));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4)); 
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 4)); 
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));

	ImGui::Begin("Simulation window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove);

	ImGui::Text("Simulation window");

	// simulation window starts here

	bool oldSimulationRunning = _simulation_space->isOldSimulationRunning();

	int currentDim = *_simulation_space->getSimulationDimension();

	const char* options[] = { "1D", "2D", "3D" };
	int selectedOption = currentDim - 1;

	ImGui::SetCursorPos(ImVec2(20, 30));
	ImGui::Text("Dimension:");
	ImGui::PushItemWidth(50);
	ImGui::SetCursorPos(ImVec2(126, 26));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.90f, 0.90f, 0.90f, 1.0f)); // Set button color to gray
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Set hovered button color to light gray
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); // Set active button color to darker gray
	
	ImGui::BeginDisabled(oldSimulationRunning);
	if (ImGui::BeginCombo("##combo", options[selectedOption])) {
		for (int i = 0; i < IM_ARRAYSIZE(options); i++) {
			bool isSelected = (selectedOption == i);
			if (ImGui::Selectable(options[i], isSelected)) {
				selectedOption = i;
				_simulation_space->setSimulationDimension(i + 1);
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::EndDisabled();

	ImGui::PopStyleColor(3);

	ImGui::PopStyleColor();
	ImGui::PopItemWidth();

	float* cellSize = _simulation_space->getCellSize();
	glm::vec3* currentSimSpace = _simulation_space->getDimensions();

	ImGui::SetCursorPos(ImVec2(20, 54));
	ImGui::Text("Cell size (%s):", LengthUnits::symbol(lengthUnit));

	ImGui::SetCursorPos(ImVec2(126, 50));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushItemWidth(50);
	//ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));

	ImGui::BeginDisabled(oldSimulationRunning);
	if (inputSceneLength("##number", cellSize))
	{
		float minCellSize1D = currentSimSpace->x;
		float minCellSize2D = std::min({ currentSimSpace->x, currentSimSpace->z });
		float minCellSize3D = std::min({ currentSimSpace->x, currentSimSpace->y, currentSimSpace->z });
		switch (currentDim)
		{
		case 1:
			if (*cellSize >= minCellSize1D)
			{
				*cellSize = minCellSize1D;
			}
			break;
		case 2:
			if (*cellSize >= minCellSize2D)
			{
				*cellSize = minCellSize2D;
			}
			break;
		case 3:
			if (*cellSize >= minCellSize3D)
			{
				*cellSize = minCellSize3D;
			}
			break;
		}
		if (*cellSize <= 0.01f)
		{
			*cellSize = .01f;
		}

		std::cout << "Cell: " << *_simulation_space->getCellSize() << std::endl;
		_simulation_space->setCellUpdate(true);
	}
	ImGui::EndDisabled();

	ImGui::PopItemWidth();
	ImGui::PopStyleColor(); 

	float currentSimSpaceX = currentSimSpace->x;
	float finalSimSpaceX = currentSimSpaceX;

	ImGui::SetCursorPos(ImVec2(20, 78));
	ImGui::Text("Sim space X (%s):", LengthUnits::symbol(lengthUnit));

	ImGui::SetCursorPos(ImVec2(126, 74));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushItemWidth(50);

	ImGui::BeginDisabled(oldSimulationRunning);
	if (inputSceneLength("##simSpaceX", &finalSimSpaceX))
	{
		if (finalSimSpaceX <= 0.0f)
		{
			finalSimSpaceX = .04f;
		}
		_simulation_space->setDimensions(glm::vec3(finalSimSpaceX,
			currentSimSpace->y, currentSimSpace->z));
		float minCellSize = std::min({ currentSimSpace->x, currentSimSpace->y, currentSimSpace->z });
		if (*cellSize >= minCellSize)
		{
			*cellSize = minCellSize;
		}
		_simulation_space->setCellUpdate(true);
		_simulation_space->initilizeFields();
	}
	ImGui::EndDisabled();

	ImGui::PopItemWidth();
	ImGui::PopStyleColor();

	if (currentDim == 1)
	{
		
	}
	else if (currentDim == 2 || currentDim == 3)
	{
		float currentSimSpaceZ = currentSimSpace->z;
		float finalSimSpaceZ = currentSimSpaceZ;

		ImGui::SetCursorPos(ImVec2(20, 102));
		ImGui::Text("Sim space Z (%s):", LengthUnits::symbol(lengthUnit));

		ImGui::SetCursorPos(ImVec2(126, 98));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
		ImGui::PushItemWidth(50);

		ImGui::BeginDisabled(oldSimulationRunning);
		if (inputSceneLength("##simSpaceZ", &finalSimSpaceZ))
		{
			if (finalSimSpaceZ <= 0.0f)
			{
				finalSimSpaceZ = .04f;
			}
			_simulation_space->setDimensions(glm::vec3(currentSimSpaceX,
				currentSimSpace->y, finalSimSpaceZ));
			float minCellSize = std::min({ currentSimSpace->x, currentSimSpace->y, currentSimSpace->z });
			if (*cellSize >= minCellSize)
			{
				*cellSize = minCellSize;
			}
			_simulation_space->setCellUpdate(true);
			_simulation_space->initilizeFields();
		}
		ImGui::EndDisabled();

		ImGui::PopItemWidth();
		ImGui::PopStyleColor();

	}
	if (currentDim == 3)
	{
		float currentSimSpaceY = currentSimSpace->y;
		float finalSimSpaceY = currentSimSpaceY;

		ImGui::SetCursorPos(ImVec2(20, 126));
		ImGui::Text("Sim space Y (%s):", LengthUnits::symbol(lengthUnit));

		ImGui::SetCursorPos(ImVec2(126, 122));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
		ImGui::PushItemWidth(50);

		ImGui::BeginDisabled(oldSimulationRunning);
		if (inputSceneLength("##simSpaceY", &finalSimSpaceY))
		{
			if (finalSimSpaceY <= 0.0f)
			{
				finalSimSpaceY = .04f;
			}
			_simulation_space->setDimensions(glm::vec3(currentSimSpaceX,
				finalSimSpaceY, currentSimSpace->z));
			float minCellSize = std::min({ currentSimSpace->x, currentSimSpace->y, currentSimSpace->z });
			if (*cellSize >= minCellSize)
			{
				*cellSize = minCellSize;
			}
			_simulation_space->setCellUpdate(true);
			_simulation_space->initilizeFields();
		}
		ImGui::EndDisabled();

		ImGui::PopItemWidth();
		ImGui::PopStyleColor();
	}


	////////// 2nd column of properties


	ImGui::SetCursorPos(ImVec2(190, 30));
	ImGui::Text("CW frequency (GHz):");

	float* freq = _simulation_space->getCWFrequency();


	ImGui::SetCursorPos(ImVec2(310, 26));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushItemWidth(100);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));

	ImGui::SliderFloat("##freq slider", freq, .25, 40);

	ImGui::PopStyleColor(4);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor();

	////-
	//ImGui::SetCursorPos(ImVec2(190, 54));
	//ImGui::Text("CW frequency (GHz):");

	//ImGui::SetCursorPos(ImVec2(310, 50));
	//ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	//ImGui::PushItemWidth(50);

	//ImGui::InputFloat("##freq_exact", freq);

	//ImGui::PopItemWidth();
	//ImGui::PopStyleColor();
	////-

	float wavelength = static_cast<float>(LengthUnits::fromMeters(SPEED_OF_LIGHT / (*freq * GHZ_TO_HZ), lengthUnit));

	ImGui::SetCursorPos(ImVec2(190, 54));
	ImGui::Text("Wavelength (%s):", LengthUnits::symbol(lengthUnit));

	ImGui::SetCursorPos(ImVec2(310, 50));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushItemWidth(100);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));

	ImGui::SliderFloat("##wavelength slider", &wavelength, static_cast<float>(LengthUnits::fromScene(5, lengthUnit)), static_cast<float>(LengthUnits::fromScene(600, lengthUnit)), "%.2f");

	ImGui::PopStyleColor(4);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor();


	float inputPower = -40;

	ImGui::SetCursorPos(ImVec2(190, 78));
	ImGui::Text("Source power (dBm):");

	ImGui::SetCursorPos(ImVec2(310, 74));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushItemWidth(100);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));

	ImGui::SliderFloat("##powerIn slider", &inputPower, -110.0f, 50.0f);

	ImGui::PopStyleColor(4);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor();

	//



	ImGui::SetCursorPos(ImVec2(190, 102));
	ImGui::Text("Vacuum crossing (s):");

	float* crossingSeconds = _simulation_space->getCrossingSeconds();

	ImGui::SetCursorPos(ImVec2(310, 98));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushItemWidth(100);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));

	ImGui::SliderFloat("##vacuum crossing", crossingSeconds, 0.5f, 10.0f, "%.1f");

	ImGui::BeginDisabled(currentDim != 1);
	ImGui::SetCursorPos(ImVec2(190, 124));
	if (ImGui::Button("Step Once"))
		_simulation_space->stepOnce1D();
	ImGui::EndDisabled();
	ImGui::SameLine();
	ImGui::Text("Step %d | %.2f ps", _simulation_space->getAcceptedSteps(),
		_simulation_space->getSimulatedTime() * 1e12);

	ImGui::PopStyleColor(4);

	ImGui::PopItemWidth();
	ImGui::PopStyleColor();
	//

	////////// 3rd column of properties
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(.55f, .55f, .55f, 1.0f));

	bool cellOn = _simulation_space->getRenderingCellOn();

	ImGui::SetCursorPos(ImVec2(424, 30));
	ImGui::Text("Cell toggle ON/OFF:");
	ImGui::SetCursorPos(ImVec2(542, 26));
	if (ImGui::Checkbox("##Cell toggle", &cellOn)) {
		if (cellOn) {
			_simulation_space->setRenderingCell(true);
		}
		else {
			_simulation_space->setRenderingCell(false);
		}
	}

	if (currentDim == 1)
	{
		bool helperFieldLines1DOn = _simulation_space->isHelperFieldLines1DOn();
		ImGui::SetCursorPos(ImVec2(424, 54));
		ImGui::Text("Field helper lines:");
		ImGui::SetCursorPos(ImVec2(542, 50));
		if (ImGui::Checkbox("##helperFieldLines1D", &helperFieldLines1DOn)) {
			if (helperFieldLines1DOn) {
				_simulation_space->setHelperFieldLines1D(true);
			}
			else {
				_simulation_space->setHelperFieldLines1D(false);
			}
		}
	}











	ImGui::PopStyleColor(4);

	////////// 4rd column of properties
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(.55f, .55f, .55f, 1.0f));

	static bool permitState = true;
	bool permeabState = true;
	bool bcondsState = _simulation_space->getBoundaryCondition1DTest();
	bool reflState = true;
	bool disperState = true;

	ImGui::SetCursorPos(ImVec2(578, 30));
	ImGui::Text("Permittivity:");
	ImGui::SetCursorPos(ImVec2(700, 26));
	if (ImGui::Checkbox("##Enable Feature", &permitState)) {
		if (permitState) {
			permitState = !permitState;
		}
		else {
			
		}
	}

	ImGui::SetCursorPos(ImVec2(578, 54));
	ImGui::Text("Permeability:");
	ImGui::SetCursorPos(ImVec2(700, 50));
	if (ImGui::Checkbox("##Enable Feature", &permeabState)) {
		if (permeabState) {
		}
		else {
		}
	}

	ImGui::SetCursorPos(ImVec2(578, 78));
	ImGui::Text("Boundary conditions:");
	ImGui::SetCursorPos(ImVec2(700, 74));
	if (ImGui::Checkbox("##Bcond Feature", &bcondsState)) {
		if (bcondsState) {
			_simulation_space->setBoundaryCondition1DTest(true);
		}
		else {
			_simulation_space->setBoundaryCondition1DTest(false);
		}
	}

	ImGui::SetCursorPos(ImVec2(578, 102));
	ImGui::Text("Reflection:");
	ImGui::SetCursorPos(ImVec2(700, 98));
	if (ImGui::Checkbox("##Enable Feature", &reflState)) {
		if (reflState) {
		}
		else {
		}
	}

	ImGui::SetCursorPos(ImVec2(578, 126));
	ImGui::Text("Dispersion:");
	ImGui::SetCursorPos(ImVec2(700, 122));
	if (ImGui::Checkbox("##Enable Feature", &disperState)) {
		if (disperState) {
		}
		else {
		}
	}





	ImGui::PopStyleColor(4);

	////////// 5th column of properties







	ImGui::End();
	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(2);









	//ImGui::Text("Status window:");
	//ImGui::Text(statusWindow->getMessage().c_str());

	//ImGui::Text("> Loading..");
	//ImGui::Text("> Loading..");
	//ImGui::Text("> Loading..");

	ImGui::End();
	ImGui::PopStyleColor();
}

void GUI::buildRightPanels()
{
	float windowWidth = ImGui::GetIO().DisplaySize.x;
	float windowHeight = ImGui::GetIO().DisplaySize.y;
	if (windowWidth < 800 || windowHeight < 400) {
		return;
	}

	ImGui::SetNextWindowPos(ImVec2(windowWidth - 400, 85));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, (windowHeight - 160 - 83) / 2));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.97f, 0.97f, 0.97f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));

	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
	ImGui::Begin("S21", nullptr,ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

	_windowS1On = false;
	_windowS2On = false;

	firstRightPanelMinimized = ImGui::IsWindowCollapsed();

	ImGui::End();
	ImGui::PopStyleColor(4);

	int secondRightPanelY = 17;
	if (!firstRightPanelMinimized) {
		_windowS1On = true;
		secondRightPanelY = (int) (windowHeight - 160 - 83) / 2 - 1;
	}

	ImGui::SetNextWindowPos(ImVec2(windowWidth - 400, 85 + (float) secondRightPanelY));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, (float) (windowHeight - 160 - 83) / 2));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.97f, 0.97f, 0.97f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.93f, 0.93f, 0.93f, 1.0f));

	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
	ImGui::Begin("S11, S22", nullptr, ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

	if (!ImGui::IsWindowCollapsed()) {
		_windowS2On = true;
	}

	ImGui::End();
	ImGui::PopStyleColor(4);

}





void GUI::buildCavityWindow()
{
	if (!cavityOpen)
		return;
    static SceneCoverage coverage;
    // No implicit fallback solver: fields exist only after an explicit Apply.
    CavityController::Settings requested{static_cast<CavityController::Material>(cavityMaterialMode), cavityPermittivity};
    requested.excitation = static_cast<CavityController::Excitation>(cavityExcitation);
    std::string gridError;
    bool gridDimensionsValid = false;
    try {
        if (cavityGridSource == 0)
            requested.grid = _simulation_space->gridSettings3D();
        gridDimensionsValid = true;
        requested.grid.validate(cavity.allocatedBytes());
    } catch (const std::exception& e) { gridError = e.what(); }
    const bool pending = !cavity.ready() || cavityGridSource != activeGridSource || !gridError.empty() || requested.grid != cavity.grid() ||
        requested.material != cavity.settings().material || requested.permittivity != cavity.settings().permittivity || requested.excitation != cavity.settings().excitation;
    if (pending) cavity.setRunning(false);
    const double now = glfwGetTime();
    cavity.update(now - cavityLastFrame);
    cavityLastFrame = now;

    ImGui::SetNextWindowSize(ImVec2(620, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("3D vacuum cavity", &cavityOpen))
    {
        ImGui::TextWrapped("Grid and materials are independent. Configured domain uses the main Simulation controls. "
            "The outlined box in the scene marks the ACTIVE solver boundary; the slice stays inside that box.");
        bool selectionChanged = ImGui::Combo("Grid", &cavityGridSource, "Configured domain\0Fixed cavity validation preset\0");
        if (cavityGridSource == 1)
            ImGui::TextWrapped("Validation preset: 12000 x 10000 x 8000 um, 500 um cells. Main grid controls do not apply to this preset.");
        selectionChanged |= ImGui::Combo("Material", &cavityMaterialMode, "Vacuum\0Uniform dielectric\0Scene materials\0");
        if (cavityMaterialMode != 2)
            ImGui::TextWrapped("Scene objects are ignored for this material choice. Choose Scene materials to include the PCB geometry.");
        if (cavityMaterialMode == 1)
            selectionChanged |= ImGui::SliderFloat("Relative permittivity", &cavityPermittivity, 1.0f, 16.0f);
        if (ImGui::Combo("Excitation", &cavityExcitation, "Standing wave\0X pulse (both directions)\0Scene sources\0"))
        {
            selectionChanged = true;
            if(cavityExcitation==2) cavityComponent=1;
        }
        if(cavityExcitation==2)
            ImGui::TextWrapped("Source objects drive a smooth-start CW signal from zero fields. Use each Source's frequency, "
                "3D drive amplitude and global polarization. Legacy dBm is not used. Edit placement, then Apply / Reset. "
                "Place the drive in the dielectric gap, not inside PEC. Ey is selected by default.");
        if (cavityExcitation == 1)
            ImGui::TextWrapped("Localized Ez pulse near X = Lx/4; splits toward +X and -X. Walls reflect it. "
                "Use XY or XZ to watch travel, YZ to inspect a cross-section. This is not a driven microstrip port.");
        if (selectionChanged) cavity.setRunning(false);
        if (!gridError.empty()) ImGui::TextWrapped("%s", gridError.c_str());
        if (gridDimensionsValid && !selectionChanged)
        {
            const auto& g = requested.grid;
            ImGui::Text("Proposed grid: %zu x %zu x %zu = %zu cells", g.cells[0],g.cells[1],g.cells[2],g.cellCount());
            ImGui::Text("Solver memory: %.1f MiB | Reset peak estimate: %.1f MiB / 512 MiB",
                g.residentBytes()/1048576.0, (g.resetBytes()+std::max(cavity.allocatedBytes(), g.residentBytes()))/1048576.0);
            ImGui::TextWrapped("Whole cells fill the domain from (0,0,0); leftover fractions are omitted. "
                "Apply / Reset uses the proposed grid. Outer walls remain PEC; copper losses and driven ports are not included.");
            if (requested.material == CavityController::Material::Scene)
                for (auto* object : _scene_main->getConductors())
                    if (std::min({*object->getLength(), *object->getHeight(), *object->getWidth()})*1e-3 < g.spacing)
                        ImGui::TextWrapped("Object %u has a dimension smaller than one cell. It may be missed or poorly resolved.", object->getId());
        }
        if (pending || selectionChanged) ImGui::TextUnformatted("Settings changed: apply them before running.");
        ImGui::BeginDisabled(!gridError.empty() || selectionChanged);
        if (ImGui::Button("Apply / Reset"))
        {
            resetSceneCavity(cavity, *_scene_main, requested, &coverage);
            if (cavity.ready() && cavity.error().empty()) activeGridSource = cavityGridSource;
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(pending || selectionChanged || !cavity.error().empty());
        if (ImGui::Button(cavity.isRunning() ? "Stop" : "Run")) cavity.setRunning(!cavity.isRunning());
        ImGui::SameLine();
        if (ImGui::Button("Step Once")) cavity.stepOnce();
        ImGui::EndDisabled();
        if (!cavity.error().empty()) ImGui::TextWrapped("%s", cavity.error().c_str());

        if (!cavity.ready())
            ImGui::TextWrapped("No active 3D simulation. Choose a valid grid and click Apply / Reset. No field is available outside an active solver domain.");
        else
        {
        ImGui::Text("Active grid: %s", activeGridSource == 0 ? "Configured domain" : "Fixed cavity validation preset");
        if (pending || selectionChanged)
            ImGui::TextWrapped("Plot and scene outline still show the last APPLIED domain. Pending edits have not moved the solver boundary.");
        const auto cavityCells = cavity.cells();
        const double cavitySpacing = cavity.spacing();
        ImGui::Text("Active extent: %.2f x %.2f x %.2f %s | Cell: %.2f %s",
            LengthUnits::fromMeters(cavityCells[0]*cavitySpacing, lengthUnit),
            LengthUnits::fromMeters(cavityCells[1]*cavitySpacing, lengthUnit),
            LengthUnits::fromMeters(cavityCells[2]*cavitySpacing, lengthUnit), LengthUnits::symbol(lengthUnit),
            LengthUnits::fromMeters(cavitySpacing, lengthUnit), LengthUnits::symbol(lengthUnit));
        if (cavity.analyticalFrequency() > 0)
            ImGui::Text("Analytical resonance %.3f GHz", cavity.analyticalFrequency()*1e-9);
        if (cavity.settings().material == CavityController::Material::Scene)
        {
            ImGui::Text("PEC electric samples: %zu | Conductor objects at Reset: %zu", cavity.conductorSamples(), coverage.size());
            if (ImGui::TreeNode("Conductor coverage at last Reset"))
            {
                ImGui::TextWrapped("Positions and X/Y/Z sizes are in %s. Overlapping objects may share samples.", LengthUnits::symbol(lengthUnit));
                for (const auto& item : coverage)
                {
                    ImGui::Text("Object %u: %zu samples", item.id, item.samples);
                    ImGui::Text("  Position: %.2f, %.2f, %.2f | Size: %.2f, %.2f, %.2f",
                        LengthUnits::fromScene(item.originMm.x, lengthUnit), LengthUnits::fromScene(item.originMm.y, lengthUnit),
                        LengthUnits::fromScene(item.originMm.z, lengthUnit), LengthUnits::fromScene(item.sizeMm.x, lengthUnit),
                        LengthUnits::fromScene(item.sizeMm.y, lengthUnit), LengthUnits::fromScene(item.sizeMm.z, lengthUnit));
                }
                ImGui::TreePop();
            }
            if (cavity.conductorSamples() == 0)
                ImGui::TextWrapped("No electric samples are inside a Trace or Carrier. Check position and size, "
                    "then Reset. The grid can miss thin features. This count excludes the outer PEC walls.");

        }

        ImGui::Text("Step %d | %.2f ps", cavity.stepCount(), cavity.time()*1e12);
        if (cavity.settings().excitation == CavityController::Excitation::StandingWave)
        {
            if (cavity.measuredFrequency() > 0) ImGui::Text("Probe crossing estimate %.3f GHz", cavity.measuredFrequency()*1e-9);
            else ImGui::TextUnformatted("Probe resonance appears after two rising zero crossings.");
        }
        if(cavity.settings().excitation==CavityController::Excitation::SceneSources)
            for(const auto& drive : cavity.sources())
                ImGui::Text("Source %u: E%c | %.3f GHz | A=%.3g | %zu driven, %zu PEC/wall excluded",
                    drive.id, "xyz"[drive.axis],drive.frequencyHz*1e-9,drive.amplitude,drive.samples.size(),drive.excluded);
        drawFieldSlice(cavity, cavityPlane, cavitySlice, lengthUnit, &cavityComponent);
        }

	}
	ImGui::End();
	if (!cavityOpen)
		cavity.setRunning(false);
}

void GUI::buildTheoryWindow()
{
	if (!theoryOpen)
		return;

	struct Topic
	{
		const char* title;
		const char* file;
	};
	static constexpr Topic topics[] = {
        {"Length units and grid coverage", "length_units.txt"},
        {"3D grid settings", "3d_grid_settings.txt"},
        {"3D slices and X pulse", "3d_slices_and_pulse.txt"},
        {"3D scene sources", "3d_scene_sources.txt"},
		{"Start a 1D run", "getting_started.txt"},
		{"Scene editor", "scene_editor.txt"},
		{"Simulation controls", "simulation_controls.txt"},
		{"Yee fields and boundaries", "yee_fields.txt"},
		{"3D cavity validation", "3d_vacuum_cavity.txt"},
		{"3D dielectric materials", "3d_dielectrics.txt"},
		{"3D perfect conductors", "3d_conductors.txt"},
		{"Sources, probes, and display", "sources_and_display.txt"},
		{"1D vacuum timing and playback", "1d_vacuum_timing.txt"},
	};

	ImGui::SetNextWindowSize(ImVec2(820, 500), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Theory of Operation", &theoryOpen))
	{
		int selected = theoryTopic < 0 ? 0 : theoryTopic;
		ImGui::BeginChild("Topics", ImVec2(220, 0), ImGuiChildFlags_Border);
		ImGui::SeparatorText("Topics");
		for (int i = 0; i < IM_ARRAYSIZE(topics); ++i)
		{
			if (ImGui::Selectable(topics[i].title, selected == i))
				selected = i;
		}
		ImGui::EndChild();
		ImGui::SameLine();

		const bool changed = selected != theoryTopic;
		if (changed)
		{
			theoryTopic = selected;
			const std::string path = std::string("res/theory_of_operation/") + topics[selected].file;
			std::ifstream file(path);
			if (file)
			{
				std::ostringstream content;
				content << file.rdbuf();
				theoryText = content.str();
			}
			else
			{
				theoryText = "Could not open " + path +
					". Launch the app from the directory containing res.";
			}
			ImGui::SetNextWindowScroll(ImVec2(0, 0));
		}
		ImGui::BeginChild("Article");
		ImGui::PushTextWrapPos(0.0f);
		for (std::size_t start = 0; start < theoryText.size();)
		{
			const std::size_t newline = theoryText.find('\n', start);
			const std::size_t end = newline == std::string::npos ? theoryText.size() : newline;
			const char* line = theoryText.data() + start;
			if (end == start)
				ImGui::Spacing();
			else if (end - start >= 2 && line[0] == '*' && line[1] == ' ')
			{
				ImGui::Bullet();
				ImGui::SameLine();
				const ImVec2 pos = ImGui::GetCursorScreenPos();
				const float width = ImGui::GetContentRegionAvail().x;
				ImGui::TextUnformatted(line + 2, theoryText.data() + end);
				// Add a second stroke using the same font to emphasize definitions.
				ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
					ImVec2(pos.x + 0.6f, pos.y), ImGui::GetColorU32(ImGuiCol_Text),
					line + 2, theoryText.data() + end, width);
			}
			else
				ImGui::TextUnformatted(line, theoryText.data() + end);
			start = end + 1;
		}
		ImGui::PopTextWrapPos();
		ImGui::EndChild();
	}
	ImGui::End();
}

void GUI::init()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	customFont = io.Fonts->AddFontFromFileTTF(__fontFile, 12.0f); // Adjust the size as needed
	io.FontDefault = customFont;
	io.Fonts->Build();

	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsLight();

	// Shared defaults for every window, including future tools and help panels.
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(1, 1, 1, 1);
	colors[ImGuiCol_ChildBg] = ImVec4(0.976f, 0.976f, 0.976f, 1);
	colors[ImGuiCol_PopupBg] = ImVec4(1, 1, 1, 1);
	colors[ImGuiCol_TitleBg] = colors[ImGuiCol_TitleBgActive] =
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.93f, 0.93f, 0.93f, 1);
	colors[ImGuiCol_FrameBg] = colors[ImGuiCol_FrameBgHovered] =
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.96f, 0.96f, 0.96f, 1);
	colors[ImGuiCol_Button] = ImVec4(0.96f, 0.96f, 0.96f, 1);
	colors[ImGuiCol_ButtonHovered] = colors[ImGuiCol_ButtonActive] =
		ImVec4(0.93f, 0.93f, 0.93f, 1);
	colors[ImGuiCol_Header] = ImVec4(0.91f, 0.91f, 0.91f, 1);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.96f, 0.96f, 0.96f, 1);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.89f, 0.89f, 0.89f, 1);
	colors[ImGuiCol_SliderGrab] = colors[ImGuiCol_SliderGrabActive] =
		ImVec4(0.85f, 0.85f, 0.85f, 1);
	colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.55f, 0.55f, 1);
}

void GUI::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::newFrame()
{
	ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.81f, 0.81f, 0.81f, 1.0f)); // Background color of selected text

	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	buildMainMenuPanel();
	buildMenuUpperPanel();
	buildMenuLowerPanel();
	buildLeftPanel();
	buildLowerPanel();
	buildRightPanels();
	buildTheoryWindow();
	buildCavityWindow();

	ImGui::PopStyleColor();
}
