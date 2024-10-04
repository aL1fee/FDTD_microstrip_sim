#include "GUI.h"

GUI::GUI(GLFWwindow* w)
{
	window = w;
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
			if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Open action */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Save action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Undo action */ }
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Redo action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("view")) { /* Undo action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("tools")) { /* Undo action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("help")) { /* Undo action */ }
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
		if (ImGui::BeginMenu("x"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Open action */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Save action */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("y"))
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
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

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

	ImGui::Begin("Property window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove);

	if (ImGui::BeginTable("PropertiesTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
		// Setup columns
		ImGui::TableSetupColumn("Property");
		ImGui::TableSetupColumn("Value");
		ImGui::TableHeadersRow();

		// Example properties
		const char* properties[] = { "Width", "Height", "Depth" };
		static float values[] = { 100.0f, 200.0f, 300.0f };

		for (int row = 0; row < IM_ARRAYSIZE(properties); row++) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", properties[row]);
			ImGui::TableSetColumnIndex(1);
			ImGui::SetNextItemWidth(-1); // Make the input field take the full width of the column
			ImGui::InputFloat(("##value" + std::to_string(row)).c_str(), &values[row], 0.0f, 0.0f, "%.2f");
		}

		ImGui::EndTable();
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

	ImGui::Text("Debug window");
	ImGui::Text("> Loading..");
	ImGui::Text("> Loading..");
	ImGui::Text("> Loading..");

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

	firstRightPanelMinimized = ImGui::IsWindowCollapsed();

	ImGui::End();
	ImGui::PopStyleColor(4);

	int secondRightPanelY = 17;
	if (!firstRightPanelMinimized) {
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
