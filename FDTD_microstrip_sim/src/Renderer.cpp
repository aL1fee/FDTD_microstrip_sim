#include "Renderer.h"

int testExecTimeInterval = 150;
long testExecTimeCount = 0;

float _aspectRatio = INITIAL_ASPECT_RATIO;
backgroundColor _activeColor = GREY;

bool _testingLinePressed = false;
bool _testingLineExpected = false;
bool _selectingObjectExpected = false;
bool _selectingObjectPressed = false;
bool _mouseRightButtonPressed = false;
bool _cameraTranslationalMotionOn = false;
bool _mouseMiddleButtonPressed = false;
bool _cameraRotationAboutPointOn = false;

bool _rayExpected = false;

bool _windowS1On = false;
bool _windowS2On = false;

bool _projMatrixChanged = true;
bool _viewMatrixChanged = true;

bool _resizing = false;

int _preserveLeftClickNum = 0;
bool _acceptingLeftClickBufferInput = false;


int _initialWindowWidth = 0;
int _initialWindowHeight = 0;
int _currentWindowWidth = 0;
int _currentWindowHeight = 0;

bool _propertyWindowOn = false;
bool _rebuildAllObjects = false;

unsigned int physicalObjectNextId = 0;
unsigned int physicalObjectNextIdMax = 0xFFFFFFFF;

std::mutex mtx;

//MainScene* _scene_main = nullptr;
MainScene* _scene_main;

//std::vector<glm::vec3> activeInputBuffer 


//Camera* cam = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

Camera* cam = new Camera(glm::vec3(3.0f, 2.0f, 10.0f), glm::vec3(3.0f, 0.0f, 0.0f));

float _deltaTime = 0.0f;
float _lastFrame = 0.0f;

//testing performance
auto testExecutionRenderLoopTime0 = std::chrono::high_resolution_clock::now();
auto testExecutionRenderLoopTime1 = std::chrono::high_resolution_clock::now();
double savedTime = 0;
double updateTimer = 0;
const double updateInterval = 1.0 / 60.0f;
int frameNum = 0;
GLuint query;

Input* input_test;

void end_resize(GLFWwindow* window) {
	_resizing = false;
	glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);

}

void Renderer::updateDeltaTime() const
{
	float currentFrame = (float) glfwGetTime();
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

void Renderer::testPerformanceStart() const {
	glFinish();
	testExecutionRenderLoopTime0 = std::chrono::high_resolution_clock::now();
	glGenQueries(1, &query);
	glBeginQuery(GL_TIME_ELAPSED, query);
	frameNum++;
	double currentTime = glfwGetTime();
	double elapseTime = currentTime - savedTime;
	savedTime = currentTime;
	updateTimer += elapseTime;
	updateTimer = 0;
}

void Renderer::testPerformanceStop() const 
{
	glEndQuery(GL_TIME_ELAPSED);
	GLuint64 elapsed_time;
	glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);
	testExecTimeCount++;
	if (testExecTimeCount % testExecTimeInterval == 0) {
		glFinish();
		testExecutionRenderLoopTime1 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds> (testExecutionRenderLoopTime1 - testExecutionRenderLoopTime0);
		std::cout << "The render loop execution time is: " << duration.count() << std::endl;
		std::cout << "Time elapsed glQuery: " << elapsed_time / 1000000.0 << " ms" << std::endl;
	}
}


std::atomic<bool> threadSwitch(false);


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (!threadSwitch.load()) {
		//int width, height;
		//glfwGetWindowSize(window, &width, &height);
		//std::cout << "HEYWIDTH: " << width << std::endl;
		//std::cout << "HEYHEIGHT: " << height << std::endl;
		//glViewport(10, 10, 10, 10);

		//_projMatrixChanged = true;

		//glViewport(0, 0, width, height);


		glfwGetWindowSize(window, &_currentWindowWidth, &_currentWindowHeight);
		_resizing = true;
		threadSwitch.store(true);
	}

}
	
void window_size_callback(GLFWwindow* window, int width, int height) {
	//if (threadSwitch.load()) {
	//	threadSwitch.store(false);
	//	std::cout << "debug 3" << std::endl;
	//}

	/*std::cout << "Hrere" << std::endl;*/



}

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//
//}

void resetKeyStates(GLFWwindow* window) {
	for (int i = 0; i < GLFW_KEY_LAST; ++i) {
		glfwGetKey(window, i); // Poll each key to reset its state
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	}
}

void window_refresh_callback(GLFWwindow* window) {
	//resetKeyStates(window);
	std::cout << "Hrere" << std::endl;

}

struct InputState {
	bool keys[GLFW_KEY_LAST] = { false };
};


InputState inputState;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key >= 0 && key < GLFW_KEY_LAST) {
		std::cout << "Key " << key << " action: " << action << std::endl;
	}

}

void Renderer::run() const 
{
	enableDepthTest();
	enablePolygonOffset();




	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	glfwSwapInterval(1);

	gui->init();
	//_scene_main = new MainScene(window, gui, input);
	//_scene_main = new MainScene(window, input);
	_scene_main = new MainScene(window);

	glfwMakeContextCurrent(nullptr); // Release context from main thread
	std::thread renderThread(&Renderer::render, this);


	while (!glfwWindowShouldClose(window)) {
		if (!threadSwitch.load()) {
			glfwPollEvents();
			_resizing = false;
			threadSwitch.store(true);
		}
		//
		////if (!threadSwitch) {
		////}
		////threadSwitch = true;
		////std::lock_guard<std::mutex> lock(mtx);
		////glfwPollEvents();
		////std::lock_guard<std::mutex> unlock(mtx);

	}
	renderThread.join();

}

void Renderer::centeredViewportResize() const {
	int width, height;
	int lowerLeftCornerOfViewportX = 0, lowerLeftCornerOfViewportY = 0;
	glfwGetWindowSize(window, &width, &height);
	int newWidth = width;
	int newHeight = height;
	if (width > height * _aspectRatio) {
		newWidth = static_cast<int>(height * _aspectRatio);
		lowerLeftCornerOfViewportX = static_cast<int>((width - newWidth) / 2.0f);
	}
	if (height > width / _aspectRatio) {
		newHeight = static_cast<int>(width / _aspectRatio);
		lowerLeftCornerOfViewportY = static_cast<int>((height - newHeight) / 2.0f);
	}
	glViewport(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, newWidth, newHeight);
}

void Renderer::enablePolygonOffset() const
{
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);
}

void Renderer::disablePolygonOffset() const
{
	glDisable(GL_POLYGON_OFFSET_LINE);
}

void Renderer::disableLineSmoothing() const
{
	glDisable(GL_LINE_SMOOTH);
}

void Renderer::setLineWidth(float value) const
{
	glLineWidth(value);
}

void Renderer::enableDepthTest() const
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Renderer::disableDepthTest() const
{
	glDisable(GL_DEPTH_TEST);
}

void Renderer::render() const
{
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window)) {
		if (threadSwitch.load()) {
			if (_resizing) {
				//centeredViewportResize();
				//_projMatrixChanged = true;
				//std::cout << "currentWidth: " << _currentWindowWidth << std::endl;
				//std::cout << "currentHeight: " << _currentWindowHeight << std::endl;
				//glViewport(0, 0, _currentWindowWidth, _currentWindowHeight);

			}
			//testPerformanceStart();

			updateDeltaTime();
			updateFrameBackgroundColor();
			clearDepthBuffer();

			_scene_main->render();

			input->processInput();

			gui->newFrame();
			gui->render();

			glfwSwapBuffers(window);

			//testPerformanceStop();

			double currentTime = glfwGetTime();
			nbFrames++;
			if (currentTime - lastTime >= 1.0) {
				std::cout << "# of frames: " << nbFrames << std::endl;
				nbFrames = 0;
				lastTime += 1.0;
			}
			threadSwitch.store(false);
		}
	}

	glfwMakeContextCurrent(nullptr);
}
