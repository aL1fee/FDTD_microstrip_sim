#include "GUI.h"

extern MainScene* _scene_main;
extern bool _propertyWindowOn;

extern bool _windowS1On;
extern bool _windowS2On;

GUI::GUI(GLFWwindow* w)
{
	window = w;
	statusWindow = new StatusWindow("...");
	firstRightPanelMinimized = true;
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
			if (ImGui::MenuItem("Theory of operation")) { /* Undo action */ }
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
			if (ImGui::MenuItem("Testing line"))
			{
				extern bool _acceptingLeftClickBufferInput;
				extern bool _testingLineExpected;
				_testingLineExpected = true;

			}
			if (ImGui::MenuItem("Wire"))
			{
				//extern bool _acceptingLeftClickBufferInput;
				//extern bool _testingLineExpected;
				//_testingLineExpected = true;
				extern bool _wireInputExpected;
				_wireInputExpected = true;

				s = "> Adding a wire";
				_scene_main->addWire(s, glm::vec3(0.0f), 0.0f, 0.0f, 0.0f,
					glm::vec3(1.0f, .843f, 0.0f), 25, 560);


			}
			if (ImGui::MenuItem("Carrier"))
			{
				extern bool _acceptingLeftClickBufferInput;
				_acceptingLeftClickBufferInput = true;

				s = "> Adding a carrier";
				_scene_main->addCarrier(s, glm::vec3(0.0f), 4.5f, 7.0f, 0.45f,
					glm::vec3(1.0f, 1.0f, .66f), 1500, 600);
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
			if (ImGui::MenuItem("A simple system"))
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
			if (ImGui::MenuItem("Another system"))
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
			if (ImGui::MenuItem("Translate an object", "T"))
			{
				PhysicalObject* obj = _scene_main->getActiveObject();
				// redundant check
				if (obj == nullptr) {
					string s = "> No object selected!\n";
					statusWindow->setMessage(s);
				}
				else {
					_scene_main->buildModifyingVectors(obj, Translation);
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
					_scene_main->buildModifyingVectors(obj, Scaling);
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
		if (ImGui::BeginMenu("1"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Open action */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Save action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("2"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Undo action */ }
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Redo action */ }
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
					ImGui::Text("%s", it->first.c_str());
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-1); // Make the input field take the full width of the column
					float* propertyValue = it->second;
					if (ImGui::InputFloat(("##value" + it->first).c_str(), propertyValue, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue)) {
						// TODO only rebuild the object which property had been updated
						if (it->first == "Length" || it->first == "Width" ||
							it->first == "Height" || it->first == "Radius") {
							if (*propertyValue < MIN_OBJECT_SIZE) {
								*propertyValue = MIN_OBJECT_SIZE;
							}
							if (*propertyValue > MAX_OBJECT_SIZE) {
								*propertyValue = MAX_OBJECT_SIZE;
							}
						}
						if (it->first == "X pad separation" || it->first == "Z pad separation" ||
							it->first == "X pad size" || it->first == "Z pad size") {
							if (*propertyValue < MIN_PAD_SEPARATION_SIZE) {
								*propertyValue = MIN_PAD_SEPARATION_SIZE;
							}
							if (*propertyValue > MAX_PAD_SEPARATION_SIZE) {
								*propertyValue = MAX_PAD_SEPARATION_SIZE;
							}
						}
						if (it->first == "Diameter") {
							if (*propertyValue < WIRE_MIN_DIAMETER) {
								*propertyValue = WIRE_MIN_DIAMETER;
							}
							if (*propertyValue > WIRE_MAX_DIAMETER) {
								*propertyValue = WIRE_MAX_DIAMETER;
							}
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
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);


	ImGui::Text("Status window:");
	ImGui::Text(statusWindow->getMessage().c_str());

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





void GUI::init()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	customFont = io.Fonts->AddFontFromFileTTF("res/fonts/Helvetica.ttf", 12.0f); // Adjust the size as needed
	io.Fonts->Build();

	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsLight();
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
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	buildMainMenuPanel();
	buildMenuUpperPanel();
	buildMenuLowerPanel();
	buildLeftPanel();
	buildLowerPanel();
	buildRightPanels();
}
