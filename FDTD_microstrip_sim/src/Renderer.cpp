#include "Renderer.h"
#include "Scene/ObservationPlaneRenderer.h"
#include "Scene/SceneViewport.h"
#include "Scene/SceneRenderTarget.h"

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
bool _cameraVerticalRotationAboutPointOn = false;

bool _rayExpected = false;
bool _rayRendered = false;

bool _wireInputExpected = false;
bool _wireInputPressed = false;
bool _wireFirstXZPlanePoint = true;
glm::vec3 _wireFirstPoint = glm::vec3(0.0f);
bool _wireXYplane = false;
bool _wireZYplane = false;


bool _ribbonInputExpected = false;
bool _ribbonInputPressed = false;
//bool _ribbonFirstXZPlanePoint = true;
//glm::vec3 _ribbonFirstPoint = glm::vec3(0.0f);


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

bool _modifyingVectorsActivated = false;
glm::vec3 _modifyingVectorsDirection = glm::vec3(0.0f);

std::mutex mtx;

//MainScene* _scene_main = nullptr;
MainScene* _scene_main;
SimulationSpace* _simulation_space;


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


// GLFW callbacks and OpenGL now share the main/UI thread.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glfwGetWindowSize(window, &_currentWindowWidth, &_currentWindowHeight);
    glViewport(0,0,width,height);
    _resizing = true;
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
	_simulation_space = new SimulationSpace(window, _scene_main);

    // Input, ImGui, scene editing and GL remain on one thread. Only numerical
    // stepping runs in AsyncCavity; no event/render flag spin loops are needed.
    render();

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
    ObservationPlaneRenderer observationPlane;
    SceneRenderTarget target;
    target.resize(1,1);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if(glfwWindowShouldClose(window)) break;
        updateDeltaTime();
        gui->newFrame(target.texture);
        const auto viewport=sceneViewport();
        if(viewport.visible) {
            const auto scale=ImGui::GetIO().DisplayFramebufferScale;
            const int w=std::max(1,static_cast<int>(viewport.width*scale.x));
            const int h=std::max(1,static_cast<int>(viewport.height*scale.y));
            target.resize(w,h);
            _scene_main->setViewportAspect(viewport.width/viewport.height);
            input->processInput();
            glBindFramebuffer(GL_FRAMEBUFFER,target.framebuffer);
            glViewport(0,0,w,h);
            glDisable(GL_SCISSOR_TEST); glDepthMask(GL_TRUE);
            updateFrameBackgroundColor(); clearDepthBuffer();
            _scene_main->render();
            _simulation_space->render();
            observationPlane.draw(gui->observationPlane(),_scene_main->getViewMatrix(),_scene_main->getProjMatrix(),&gui->fieldSimulation());
        }
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        int w,h; glfwGetFramebufferSize(window,&w,&h); glViewport(0,0,w,h);
        glDisable(GL_SCISSOR_TEST); glClearColor(.94f,.94f,.94f,1); glClear(GL_COLOR_BUFFER_BIT);
        gui->render();
        glfwSwapBuffers(window);
        _resizing=false;
    }
    target.release(); observationPlane.release();
    gui->terminate();
}
