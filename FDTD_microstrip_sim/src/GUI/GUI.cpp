#include "GUI.h"
#include "../Simulation/SceneCavitySetup.h"

#include <algorithm>
#include <fstream>
#include <sstream>

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
			if (ImGui::MenuItem("Propetry menu")) { /* Undo action */ }
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

				float simulationDimensions[3] = { currDims->x, currDims->y, currDims->z };

				if (ImGui::SliderFloat3("Dimensions (x, y, z)", simulationDimensions, 0.0f, 20.0f, "%.2f")) {
					currDims->x = simulationDimensions[0];
					currDims->y = simulationDimensions[1];
					currDims->z = simulationDimensions[2];
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
				if (ImGui::SliderFloat("Cell size", cellSize, 0.02f, .5f, "%.2f"))
				{
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
					ImGui::Text("%s", it->first.c_str());
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-1); // Make the input field take the full width of the column
					float* propertyValue = it->second;
					if (ImGui::InputFloat(("##value" + it->first).c_str(), propertyValue, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue)) {
						// TODO only rebuild the object which property had been updated
						if (it->first == "X position" || it->first == "Y position" ||
							it->first == "Z position")
						{
							activeObj->setModelMatrixRegenExpected(true);
						}					
						if (it->first == "Length" || it->first == "Width" ||
							it->first == "Height" || it->first == "Radius") {
							if (*propertyValue < MIN_OBJECT_SIZE) {
								*propertyValue = MIN_OBJECT_SIZE;
							}
							if (*propertyValue > MAX_OBJECT_SIZE) {
								*propertyValue = MAX_OBJECT_SIZE;
							}
							activeObj->setModelMatrixRegenExpected(true);
						}
						if (it->first == "X pad separation" || it->first == "Z pad separation" ||
							it->first == "X pad size" || it->first == "Z pad size") {
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
						if (it->first == "Diameter") {
							if (*propertyValue < WIRE_MIN_DIAMETER) {
								*propertyValue = static_cast<float>(WIRE_MIN_DIAMETER);
							}
							if (*propertyValue > WIRE_MAX_DIAMETER) {
								*propertyValue = static_cast<float>(WIRE_MAX_DIAMETER);
							}
						}
						if (it->first == "Rotation angle")
						{
							if (*propertyValue < 0.0f) {
								*propertyValue = std::fmod(*propertyValue, 360.0f);
								*propertyValue += 360.0f;
							}
							if (*propertyValue > 360.0f) {
								*propertyValue = std::fmod(*propertyValue, 360.0f);
							}
						}
						if (it->first == "Curve length" || it->first == "Tail length" ||
							it->first == "Curve height" || it->first == "Thickness")
						{
							activeObj->setModelVertsRegenExpected(true);

							if (it->first == "Curve length")
							{
								activeObj->setScaleL(*propertyValue);
							}
							else if (it->first == "Curve height")
							{
								if (*propertyValue < RIBBON_MIN_CURVE_HEIGHT) {
									*propertyValue = static_cast<float>(RIBBON_MIN_CURVE_HEIGHT);
								}
								if (*propertyValue > RIBBON_MAX_CURVE_HEIGHT) {
									*propertyValue = static_cast<float>(RIBBON_MAX_CURVE_HEIGHT);
								}
							}
							else if (it->first == "Tail length")
							{
								if (*propertyValue < RIBBON_MIN_CURVE_TAIL) {
									*propertyValue = static_cast<float>(RIBBON_MIN_CURVE_TAIL);
								}
								if (*propertyValue > RIBBON_MAX_CURVE_TAIL) {
									*propertyValue = static_cast<float>(RIBBON_MAX_CURVE_TAIL);
								}
							}
							else if (it->first == "Thickness")
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
					if (ImGui::InputScalar(("##value" + it->first).c_str(), ImGuiDataType_S32, propertyValue, 
						nullptr, nullptr, "%d", ImGuiInputTextFlags_EnterReturnsTrue)) {
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
	ImGui::Text("Cell size (mm):");

	ImGui::SetCursorPos(ImVec2(126, 50));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushItemWidth(50);
	//ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));

	ImGui::BeginDisabled(oldSimulationRunning);
	if (ImGui::InputFloat("##number", cellSize, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
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
	ImGui::Text("Sim space X (mm):");

	ImGui::SetCursorPos(ImVec2(126, 74));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushItemWidth(50);

	ImGui::BeginDisabled(oldSimulationRunning);
	if (ImGui::InputFloat("##simSpaceX", &finalSimSpaceX, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
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
		ImGui::Text("Sim space Z (mm):");

		ImGui::SetCursorPos(ImVec2(126, 98));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
		ImGui::PushItemWidth(50);

		ImGui::BeginDisabled(oldSimulationRunning);
		if (ImGui::InputFloat("##simSpaceZ", &finalSimSpaceZ, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
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
		ImGui::Text("Sim space Y (mm):");

		ImGui::SetCursorPos(ImVec2(126, 122));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
		ImGui::PushItemWidth(50);

		ImGui::BeginDisabled(oldSimulationRunning);
		if (ImGui::InputFloat("##simSpaceY", &finalSimSpaceY, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
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

	float wavelength = (float) SPEED_OF_LIGHT / (*freq * GHZ_TO_HZ) * 1000;

	ImGui::SetCursorPos(ImVec2(190, 54));
	ImGui::Text("Wavelength (mm):");

	ImGui::SetCursorPos(ImVec2(310, 50));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushItemWidth(100);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.96f, 0.96f, 0.96f, 1.0f));

	ImGui::SliderFloat("##wavelength slider", &wavelength, 5.0, 600.0);

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
	const auto cavityCells = CavityController::cells();
	const double cavitySpacing = CavityController::spacing();
	auto reset = [&] {
		resetSceneCavity(cavity, *_scene_main,
			{static_cast<CavityController::Material>(cavityMaterialMode), cavityPermittivity});
	};
	if (!cavity.ready())
		reset();
	const double now = glfwGetTime();
	cavity.update(now - cavityLastFrame);
	cavityLastFrame = now;

	ImGui::SetNextWindowSize(ImVec2(540, 470), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("3D vacuum cavity", &cavityOpen))
	{
		ImGui::TextWrapped("Solver validation example. A standing wave oscillates inside a closed metal box. "
			"We compare its measured frequency with the calculated value to check the 3D solver.");
		ImGui::TextWrapped("Red and blue show opposite electric-field directions. White is zero. "
			"The initial mode is uniform along Z. Spatially varying materials can change its pattern.");
		if (cavityMaterialMode < 2)
			ImGui::Text("Analytical resonance %.3f GHz", cavity.analyticalFrequency() * 1e-9);
		else
			ImGui::TextWrapped("Scene substrates are sampled in mm inside the fixed box from (0,0,0). "
				"Reset applies geometry edits. Traces and carriers become perfect conductors. Conductivity losses are excluded.");
		ImGui::BeginDisabled(cavity.isRunning());
		if (ImGui::Combo("Material", &cavityMaterialMode, "Vacuum\0Uniform dielectric\0Scene materials\0"))
			reset();
		if (cavityMaterialMode == 1 &&
			ImGui::SliderFloat("Relative permittivity", &cavityPermittivity, 1.0f, 16.0f))
			reset();
		ImGui::EndDisabled();
		if (!cavity.error().empty())
			ImGui::TextWrapped("%s", cavity.error().c_str());
		ImGui::TextUnformatted("12 x 10 x 8 mm | Conducting walls | 0.5 mm cells");
        if (cavityMaterialMode == 2)
            ImGui::Text("PEC electric samples: %zu (thin features may miss this grid)", cavity.conductorSamples());
		ImGui::BeginDisabled(!cavity.error().empty());
		if (ImGui::Button(cavity.isRunning() ? "Stop" : "Run"))
		{
			cavity.setRunning(!cavity.isRunning());
		}
		ImGui::SameLine();
		if (ImGui::Button("Step Once"))
		{
			cavity.stepOnce();
		}
		ImGui::EndDisabled();
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
			reset();

		ImGui::SliderInt("Z slice", &cavitySlice, 0, static_cast<int>(cavityCells[2]) - 1);
		ImGui::Text("Step %d | %.2f ps | Z = %.2f mm", cavity.stepCount(),
			cavity.time() * 1e12, (cavitySlice + 0.5) * cavitySpacing * 1e3);
		if (cavity.measuredFrequency() > 0)
			ImGui::Text("Probe crossing estimate %.3f GHz", cavity.measuredFrequency() * 1e-9);
		else
			ImGui::TextUnformatted("Probe resonance appears after two rising zero crossings.");
		ImGui::TextUnformatted("XY slice of normalized Ez | Blue negative | White zero | Red positive");

		const ImVec2 available = ImGui::GetContentRegionAvail();
		const float pitch = std::min(available.x / (cavityCells[0] + 1),
			available.y / (cavityCells[1] + 1));
		if (pitch > 0)
		{
			const ImVec2 origin = ImGui::GetCursorScreenPos();
			auto* draw = ImGui::GetWindowDrawList();
			for (std::size_t y = 0; y <= cavityCells[1]; ++y)
				for (std::size_t x = 0; x <= cavityCells[0]; ++x)
				{
					const float sample = cavity.sample(
						{x, y, static_cast<std::size_t>(cavitySlice)});
					const float value = std::max(-1.0f, std::min(sample, 1.0f));
					const float fade = 1.0f - std::abs(value);
					const ImU32 color = ImGui::GetColorU32(ImVec4(
						value < 0 ? fade : 1.0f, fade, value > 0 ? fade : 1.0f, 1.0f));
					const ImVec2 top(origin.x + pitch * x, origin.y + pitch * (cavityCells[1] - y));
					draw->AddRectFilled(top, ImVec2(top.x + pitch, top.y + pitch), color);
				}
			ImGui::Dummy(ImVec2(pitch * (cavityCells[0] + 1), pitch * (cavityCells[1] + 1)));
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
		{"Start a 1D run", "getting_started.txt"},
		{"Scene editor", "scene_editor.txt"},
		{"Simulation controls", "simulation_controls.txt"},
		{"Yee fields and boundaries", "yee_fields.txt"},
		{"3D cavity validation", "3d_vacuum_cavity.txt"},
		{"3D dielectric materials", "3d_dielectrics.txt"},
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
