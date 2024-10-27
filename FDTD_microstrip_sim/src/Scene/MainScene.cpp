#include "MainScene.h"

//extern bool _rebuildAllObjects;

MainScene::MainScene(GLFWwindow* w)
{
	window = w;
	shaderMap = new map<std::string, Shader*>();
	physicalObjectBuffer = new std::vector<PhysicalObject*>();
	viewMatrix = glm::mat4(1.0f);
	projMatrix = glm::mat4(1.0f);
	propertyWindow = new PropertyWindow();
	activeObject = nullptr;
	init();
}

void MainScene::init()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	projMatrix = glm::perspective(glm::radians(cam->getZoom()), aspect, 0.1f, 100.0f);
	viewMatrix = cam->lookAt();


	Shader* shader_csys = new Shader("res/shaders/coordinate_system.shader", 0);
	shaderMap->insert(std::make_pair("Csys", shader_csys));
	PhysicalObject* csys = new CoordinateSystem_PO(20.0f, 0.6f, shader_csys);
	physicalObjectBuffer->push_back(csys);

	testingLineShader = new Shader("res/shaders/testingLine.shader", 0);
	testingLine = new Curve_PO(testingLineShader);
}

void MainScene::render()
{
	if (_projMatrixChanged) {
		projMatrix = glm::perspective(glm::radians(cam->getZoom()), aspect, 0.1f, 100.0f);
		testingLineShader->bind();
		glUniformMatrix4fv(testingLineShader->getUniformLocation("proj"), 1, GL_FALSE, glm::value_ptr(projMatrix));
		testingLineShader->unbind();
		for (auto sh = shaderMap->begin(); sh != shaderMap->end(); sh++) {
			sh->second->bind();
			glUniformMatrix4fv(sh->second->getUniformLocation("proj"), 1, GL_FALSE, glm::value_ptr(projMatrix));
			sh->second->unbind();
		}
		_projMatrixChanged = false;
	}
	if (_viewMatrixChanged) {
		viewMatrix = cam->lookAt();
		testingLineShader->bind();
		glUniformMatrix4fv(testingLineShader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		testingLineShader->unbind();
		for (auto sh = shaderMap->begin(); sh != shaderMap->end(); sh++) {
			sh->second->bind();
			glUniformMatrix4fv(sh->second->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
			sh->second->unbind();
		}
		_viewMatrixChanged = false;
	}



	testingLine->draw();
	 
	for (PhysicalObject* obj : *physicalObjectBuffer) {
		if (obj->needsRebuilding())
		{
			obj->build();
			obj->setRebuiltExpected(false);
		}
		obj->draw();
	}



}

void MainScene::addCarrier(std::string& s)
{
	Shader* shader;
	if (shaderMap->find("Carrier") == shaderMap->end())
	{
		shader = new Shader("res/shaders/carrier.shader", 1);
		shaderMap->insert(std::make_pair("Carrier", shader));
	}
	else {
		shader = shaderMap->at("Carrier");
	}
	Carrier_PO* carrierPO = new Carrier_PO(glm::vec3(0.0f), 4.5f, 7.0f, 0.45f, 
		glm::vec3(1.0f, 1.0f, .66f), 1500, 600, shader);
	carrierPO->updatePropertyMap();
	physicalObjectBuffer->push_back(carrierPO);
	_viewMatrixChanged = true;
	_projMatrixChanged = true;
}

void MainScene::addSubstrate(std::string& s)
{
	Shader* shader;
	if (shaderMap->find("Substrate") == shaderMap->end())
	{
		shader = new Shader("res/shaders/substrate.shader", 1);
		shaderMap->insert(std::make_pair("Substrate", shader));
	}
	else {
		shader = shaderMap->at("Substrate");
	}
	Substrate_PO* substratePO = new Substrate_PO(glm::vec3(0.0f, .45f, 3.0f), 4.5f, 1.0f, 0.6f,
		glm::vec3(.4f, .8f, 1.0f), .00001f, 3.05f, shader);
	substratePO->updatePropertyMap();
	physicalObjectBuffer->push_back(substratePO);
	_viewMatrixChanged = true;
	_projMatrixChanged = true;
}

void MainScene::addTrace(std::string& s)
{
	Shader* shader;
	if (shaderMap->find("Trace") == shaderMap->end())
	{
		shader = new Shader("res/shaders/trace.shader", 1);
		shaderMap->insert(std::make_pair("Trace", shader));
	}
	else {
		shader = shaderMap->at("Trace");
	}
	Trace_PO* tracePO = new Trace_PO(glm::vec3(0.0f, 1.05f, 3.42f), 4.5f, .15f, 0.01f,
		glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410, shader);
	tracePO->updatePropertyMap();
	physicalObjectBuffer->push_back(tracePO);
	_viewMatrixChanged = true;
	_projMatrixChanged = true;
}

void MainScene::addHousing(std::string& s)
{
	Shader* shader;
	if (shaderMap->find("Housing") == shaderMap->end())
	{
		shader = new Shader("res/shaders/housing.shader", 1);
		shaderMap->insert(std::make_pair("Housing", shader));
	}
	else {
		shader = shaderMap->at("Housing");
	}
	Housing_PO* housingPO = new Housing_PO(glm::vec3(0.0f, 0.0f, -.2f), 9.0f, .2f, 2.0f,
		glm::vec3(.88f, .88f, .88f), 10.0f, 50, shader);
	housingPO->updatePropertyMap();
	physicalObjectBuffer->push_back(housingPO);
	_viewMatrixChanged = true;
	_projMatrixChanged = true;
}

void MainScene::deleteActiveObject()
{



	delete activeObject;
	activeObject = nullptr;
}

void MainScene::deleteAllObjects()
{
	for (auto it = physicalObjectBuffer->begin(); it != physicalObjectBuffer->end();)
	{
		if ((*it)->interactable()) {
			std::string shaderName = (*it)->
			delete *it;
			it = physicalObjectBuffer->erase(it);
		}
		else {
			++it;
		}
	}
	physicalObjectBuffer->clear();
	shaderMap->clear();

	//NEED contain shader names in physicalobjects and emptry shadermap here



}

void MainScene::generateRay(glm::vec3 pos, glm::vec3 dir)
{
	testingLine->terminateLine();
	testingLine->addPoint(pos);
	testingLine->addPoint(pos + dir * 20.0f);
}

// hardcoding just the carriers for now
// TODO inefficient ray picking
void MainScene::selectObject(glm::vec3 pos, glm::vec3 dir)
{
	glm::vec3 rayPos = pos;
	float count = 0.0f;
	while (count < SELECTING_OBJECT_RANGE) {
		for (PhysicalObject* obj : *physicalObjectBuffer) {
			if (obj->intersectionCheck(rayPos)) {
				std::cout << "object selected!" << std::endl;
				propertyWindow->addPropertyMap(obj->getPropertyMap());
				propertyWindow->updateActiveObject(obj);
				activeObject = obj;
				extern bool _propertyWindowOn;
				_propertyWindowOn = true;
				return;
			}
		}
		rayPos += dir * SELECTING_OBJECT_PRECISION;
		count += SELECTING_OBJECT_PRECISION;
	}
	propertyWindow->setDefaultPropertyMap();
	propertyWindow->updateActiveObject(nullptr);
	std::cout << "no objects found!" << std::endl;
}
