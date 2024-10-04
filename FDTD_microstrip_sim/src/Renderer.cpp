#include "Renderer.h"

int testExecTimeInterval = 150;
long testExecTimeCount = 0;

float _aspectRatio = INITIAL_ASPECT_RATIO;
backgroundColor _activeColor = GREY;

bool _mouseRightButtonPressed = false;
bool _cameraTranslationalMotionOn = false;

bool _projMatrixChanged = true;
bool _viewMatrixChanged = true;

//Camera* cam = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

Camera* cam = new Camera(glm::vec3(3.0f, 2.0f, 10.0f), glm::vec3(3.0f, 0.0f, 0.0f));

float _deltaTime = 0.0f;
float _lastFrame = 0.0f;

glm::mat4 pMat;



void Renderer::updateDeltaTime() const
{
	float currentFrame = glfwGetTime();
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
}

Renderer::Renderer(GLFWwindow* w, GUI* g)
{
	window = w;
	gui = g;
	input = new Input(window);
}

void Renderer::clearColorBuffer() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clearDepthBuffer() const
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::updateFrameBackgroundColor() const
{
	switch (_activeColor) {
	case GREY:
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.95f, 1.0f, 1.0f, 1.0f);
		break;
	case LIGHT:
		glClearColor(0.94f, 0.94f, 0.94f, 1.0f);
		break;
	case DARK:
		glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
		break;
	}
	clearColorBuffer();
}

void Renderer::run() const
{
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(window, window_reshape_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	gui->init();

	//glfwSetWindowRefreshCallback(window, window_refresh_callback);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	const int numVAOs = 1;
	const int numVBOs = 2;



	float cameraX, cameraY, cameraZ;
	float cubeLocX, cubeLocY, cubeLocZ, pyrLocX, pyrLocY, pyrLocZ;
	GLuint renderingProgram;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];

	GLuint mvLoc, projLoc, vLoc, tfLoc;
	int width, height;
	float aspect;
	glm::mat4 vMat, mMat, mvMat, tMat, rMat;


	float vertexPositions[108] = {
		 -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
		};

	float pyramidPositions[54] =
	{ -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
	 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
	 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
	 -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
	 -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
	 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base – right back
	};


	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

	//std::cout << "H0: " << vbo[0] << std::endl;
	//std::cout << "H1: " << vbo[1] << std::endl;

	Shader shader_cube("res/shaders/cube.shader", 0);

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 15.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective
	pyrLocX = 3.0f; pyrLocY = 4.0f; pyrLocZ = 5.0f;
	//cameraZ = 420.0f;

	auto testExecutionRenderLoopTime0 = std::chrono::high_resolution_clock::now();

	stack<glm::mat4> mvStack;

	mvLoc = shader_cube.getUniformLocation("mv_matrix");
	projLoc = shader_cube.getUniformLocation("proj_matrix");


	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	//pMat = glm::perspective(1.0472f, aspect, 0.1f, 100.0f); // 1.0472 radians = 60 degrees
	pMat = glm::perspective(glm::radians(cam->getZoom()), aspect, 0.1f, 100.0f);
	std::cout << "aspect: " << aspect << std::endl;





	Shader shader_csys("res/shaders/coordinate_system.shader", 1);
	CoordinateSystem csys = CoordinateSystem(20.0f, 0.6f);
	csys.run();

	GLuint viewCamLoc = shader_csys.getUniformLocation("view");
	GLuint projCamLoc = shader_csys.getUniformLocation("proj");
	GLuint colCsysLoc = shader_csys.getUniformLocation("color");

	glm::vec3 mainGridColor = csys.getMainColor();
	glm::vec3 helperGridColor = csys.getHelperColor();

	const float radius = 10.0f;
	float camX, camZ;
	glm::mat4 view;
	view = cam->lookAt();

	double savedTime = 0;
	double updateTimer = 0;
	const double updateInterval = 1.0 / 60.0f;

	int frameNum = 0;

	while (!glfwWindowShouldClose(window)) {
		std::cout << "EHEYE" << std::endl;
		std::cout << "asdasd" << std::endl;
		glFinish();
		auto testExecutionRenderLoopTime0 = std::chrono::high_resolution_clock::now();
		GLuint query;
		glGenQueries(1, &query);
		glBeginQuery(GL_TIME_ELAPSED, query);
		frameNum++;

		double currentTime = glfwGetTime();
		double elapseTime = currentTime - savedTime;
		savedTime = currentTime;
		updateTimer += elapseTime;
		if (updateTimer < updateInterval) {
			continue;
		}
		updateTimer = 0;

		updateDeltaTime();
		updateFrameBackgroundColor();
		clearDepthBuffer();

		gui->newFrame();
		input->processInput();


		//camX = sin(glfwGetTime()) * radius;
		//camZ = cos(glfwGetTime()) * radius;
		//cam->updatePos(glm::vec3(camX, camZ-camX, camZ));




		shader_csys.bind();
		if (_projMatrixChanged) {
			pMat = glm::perspective(glm::radians(cam->getZoom()), aspect, 0.1f, 100.0f);
			glUniformMatrix4fv(projCamLoc, 1, GL_FALSE, glm::value_ptr(pMat));
			_projMatrixChanged = false;
		}
		if (_viewMatrixChanged) {
			view = cam->lookAt();
			glUniformMatrix4fv(viewCamLoc, 1, GL_FALSE, glm::value_ptr(view));
			_viewMatrixChanged = false;
		}

		shader_csys.setUniform3f("color", mainGridColor.x, mainGridColor.y, mainGridColor.z);
		csys.drawMain();
		shader_csys.setUniform3f("color", helperGridColor.x, helperGridColor.y, helperGridColor.z);
		csys.drawHelper();
		shader_csys.unbind();





		//shader_cube.bind();
		//Utility::checkGLError("shader_cube.bind");
		//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
		//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(view));
		//Utility::checkGLError("vMat");
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		//Utility::checkGLError("pMat");
		//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//glBindVertexArray(vao[0]); // Bind VAO if you have one
		//Utility::checkGLError("bindbuf");
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//Utility::checkGLError("attribpointer");
		//glEnableVertexAttribArray(0);
		//glEnable(GL_DEPTH_TEST);
		//Utility::checkGLError("other");
		//glDrawArrays(GL_TRIANGLES, 0, 36); // draw the sun
		//shader_cube.unbind();
		//Utility::checkGLError("last");
		//auto testExecutionRenderLoopTime1 = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (testExecutionRenderLoopTime1 - testExecutionRenderLoopTime0);
		//float currentTime = (float)duration.count() / 2000;
		// 
		//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
		//mvStack.push(vMat);
		//
		//mvStack.push(mvStack.top());
		//mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // sun position
		//mvStack.push(mvStack.top());
		//mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
		//
		//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//glEnableVertexAttribArray(0);
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_LEQUAL);
		//glDrawArrays(GL_TRIANGLES, 0, 18); // draw the sun
		//mvStack.pop(); // remove the sun’s axial rotation from the stack
		//
		////----------------------- cube == planet ---------------------------------------------
		//mvStack.push(mvStack.top());
		//mvStack.top() *=
		//	glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
		//mvStack.push(mvStack.top());
		//mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
		//// planet rotation
		//
		//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		//
		//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//glEnableVertexAttribArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, 36); // draw the planet
		//mvStack.pop(); // remove the planet’s axial rotation from the stack
		//
		//
		//mvStack.push(mvStack.top());
		//mvStack.top() *=
		//	glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0,
		//		cos((float)currentTime) * 2.0));
		//mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));
		//// moon rotation
		//mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f)); // make the moon smaller
		//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		//
		//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//glEnableVertexAttribArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, 36); // draw the moon
		//// remove moon scale/rotation/position, planet position, sun position, and view matrices from stack
		//mvStack.pop(); mvStack.pop(); mvStack.pop(); mvStack.pop();




		gui->render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		glEndQuery(GL_TIME_ELAPSED);
		GLuint64 elapsed_time;
		glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);
		testExecTimeCount++;
		if (testExecTimeCount % testExecTimeInterval == 0) {
			glFinish();
			auto testExecutionRenderLoopTime1 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds> (testExecutionRenderLoopTime1 - testExecutionRenderLoopTime0);
			std::cout << "The render loop execution time is: " << duration.count() << std::endl;
			std::cout << "Time elapsed glQuery: " << elapsed_time / 1000000.0 << " ms" << std::endl;
		}
	}
}
