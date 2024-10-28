#include "MainScene.h"

//extern bool _rebuildAllObjects;

MainScene::MainScene(GLFWwindow* w)
{
	window = w;
	shaderMap = new map<std::string, Shader*>();
	physicalObjectBuffer = new std::map<unsigned int, PhysicalObject*>();
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


	Shader* shader_csys = new Shader("res/shaders/coordinate_system.shader", 0, "Csys");
	shaderMap->insert(std::make_pair("Csys", shader_csys));
	shader_csys->incrNumObjectsServed();
	PhysicalObject* csys = new CoordinateSystem_PO(20.0f, 0.6f, shader_csys);
	physicalObjectBuffer->insert(std::make_pair(csys->getId(), csys));
	//physicalObjectBuffer->push_back(csys);

	testingLineShader = new Shader("res/shaders/testingLine.shader", 0, "Curve");
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
	 
	for (auto& pair : *physicalObjectBuffer) {
		PhysicalObject* obj = pair.second;
		if (obj->needsRebuilding())
		{
			obj->build();
			obj->setRebuiltExpected(false);
		}
		obj->draw();
	}
}

void MainScene::addCarrier(std::string& s, glm::vec3 o, float l, float w,
	float h, glm::vec3 col, float perm, float cond)
{
	Shader* shader;
	std::string name = "Carrier";
	if (shaderMap->find(name) == shaderMap->end())
	{
		shader = new Shader("res/shaders/carrier.shader", 1, name);
		shaderMap->insert(std::make_pair(name, shader));
	}
	else {
		shader = shaderMap->at(name);
	}
	shader->incrNumObjectsServed();
	/*Carrier_PO* carrierPO = new Carrier_PO(glm::vec3(0.0f), 4.5f, 7.0f, 0.45f, 
		glm::vec3(1.0f, 1.0f, .66f), 1500, 600, shader);*/
	Carrier_PO* carrierPO = new Carrier_PO(o, l, w, h, col, perm, cond, shader);
	carrierPO->updatePropertyMap();
	physicalObjectBuffer->insert(std::make_pair(carrierPO->getId(), carrierPO));
	//physicalObjectBuffer->push_back(carrierPO);
	_viewMatrixChanged = true;
	_projMatrixChanged = true;
}

void MainScene::addSubstrate(std::string& s, glm::vec3 o, float l, float w,
	float h, glm::vec3 col, float perm, float cond)
{
	Shader* shader;
	std::string name = "Substrate";
	if (shaderMap->find(name) == shaderMap->end())
	{
		shader = new Shader("res/shaders/substrate.shader", 1, name);
		shaderMap->insert(std::make_pair(name, shader));
	}
	else {
		shader = shaderMap->at(name);
	}
	shader->incrNumObjectsServed();
	//Substrate_PO* substratePO = new Substrate_PO(glm::vec3(0.0f, .45f, 3.0f), 4.5f, 1.0f, 0.6f,
		//glm::vec3(.4f, .8f, 1.0f), .00001f, 3.05f, shader);
	Substrate_PO* substratePO = new Substrate_PO(o, l, w, h, col, perm, cond, shader);
	substratePO->updatePropertyMap();
	physicalObjectBuffer->insert(std::make_pair(substratePO->getId(), substratePO));
	//physicalObjectBuffer->push_back(substratePO);
	_viewMatrixChanged = true;
	_projMatrixChanged = true;
}

void MainScene::addTrace(std::string& s, glm::vec3 o, float l, float w,
	float h, glm::vec3 col, float perm, float cond)
{
	Shader* shader;
	std::string name = "Trace";
	if (shaderMap->find(name) == shaderMap->end())
	{
		shader = new Shader("res/shaders/trace.shader", 1, name);
		shaderMap->insert(std::make_pair(name, shader));
	}
	else {
		shader = shaderMap->at(name);
	}
	shader->incrNumObjectsServed();
	//Trace_PO* tracePO = new Trace_PO(glm::vec3(0.0f, 1.05f, 3.42f), 4.5f, .15f, 0.01f,
	//	glm::vec3(1.0f, .843f, 0.0f), 5.2f, 410, shader);
	Trace_PO* tracePO = new Trace_PO(o, l, w, h, col, perm, cond, shader);
	tracePO->updatePropertyMap();
	physicalObjectBuffer->insert(std::make_pair(tracePO->getId(), tracePO));
	//physicalObjectBuffer->push_back(tracePO);
	_viewMatrixChanged = true;
	_projMatrixChanged = true;
}

void MainScene::addHousing(std::string& s, glm::vec3 o, float l, float w,
	float h, glm::vec3 col, float perm, float cond)
{
	Shader* shader;
	std::string name = "Housing";
	if (shaderMap->find(name) == shaderMap->end())
	{
		shader = new Shader("res/shaders/housing.shader", 1, name);
		shaderMap->insert(std::make_pair(name, shader));
	}
	else {
		shader = shaderMap->at(name);
	}
	shader->incrNumObjectsServed();
	//Housing_PO* housingPO = new Housing_PO(glm::vec3(0.0f, 0.0f, -.2f), 9.0f, .2f, 2.0f,
	//	glm::vec3(.88f, .88f, .88f), 10.0f, 50, shader);
	Housing_PO* housingPO = new Housing_PO(o, l, w, h, col, perm, cond, shader);
	housingPO->updatePropertyMap();
	physicalObjectBuffer->insert(std::make_pair(housingPO->getId(), housingPO));
	//physicalObjectBuffer->push_back(housingPO);
	_viewMatrixChanged = true;
	_projMatrixChanged = true;
}

void MainScene::eraseShaderMapOneInstance(std::string name)
{
	std::cout << "objs served: " << shaderMap->at(name)->getNumObjectsServed() << std::endl;
	
	shaderMap->at(name)->decrNumObjectsServed();
	if (shaderMap->at(name)->getNumObjectsServed() == 0) {
		std::cout << "erasing the shader" << std::endl;
		shaderMap->erase(name);
	}
}

void MainScene::deleteActiveObject()
{
	if (activeObject != nullptr)
	{
		if (activeObject->interactable())
		{
			physicalObjectBuffer->erase(activeObject->getId());
			eraseShaderMapOneInstance(activeObject->getShaderName());
			delete activeObject;
			activeObject = nullptr;
			propertyWindow->nullify();
			return;
		}
	}
}

void MainScene::deleteAllObjects()
{
	for (auto it = physicalObjectBuffer->begin(); 
		it != physicalObjectBuffer->end();) {
		PhysicalObject* obj = it->second;
		std::cout << obj->getShaderName() << std::endl;
		if (obj->interactable()) {
			eraseShaderMapOneInstance(obj->getShaderName());
			delete obj;
			it = physicalObjectBuffer->erase(it); // Erase returns the next iterator
		}
		else {
			++it;
		}
	}
	activeObject = nullptr;
	propertyWindow->nullify();
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
		for (auto &pair : *physicalObjectBuffer) {
			PhysicalObject* obj = pair.second;
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
