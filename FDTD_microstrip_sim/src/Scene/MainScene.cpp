#include "MainScene.h"

//extern bool _rebuildAllObjects;
extern unsigned int physicalObjectNextIdMax;
extern bool _modifyingVectorsActivated;
extern glm::vec3 _modifyingVectorsDirection;

MainScene::MainScene(GLFWwindow* w)
{
	window = w;
	shaderMap = new map<std::string, Shader*>();
	physicalObjectBuffer = new std::map<unsigned int, PhysicalObject*>();
	viewMatrix = glm::mat4(1.0f);
	projMatrix = glm::mat4(1.0f);
	propertyWindow = new PropertyWindow();
	activeObject = nullptr;
	nearPlaneValue = INITIAL_NEAR_PLANE_VALUE;
	farPlaneValue = INITIAL_FAR_PLANE_VALUE;
	modifyingVectors = nullptr;
	activeWire = nullptr;
	highestClickedObjPoint = glm::vec3(0.0f);
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
		projMatrix = glm::perspective(glm::radians(cam->getZoom()), aspect, nearPlaneValue, farPlaneValue);
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
		if (obj->needsBuilding())
		{
			obj->build();
			obj->setBuiltExpected(false);
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
	postObjectInsertionSetup();
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
	postObjectInsertionSetup();
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
	postObjectInsertionSetup();
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
	postObjectInsertionSetup();
}

void MainScene::addPowerSource(std::string& s, glm::vec3 o, glm::vec3 dir, float p,
	float r, float l, glm::vec3 col, float perm, float cond)
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
	PowerSource_PO* powerSourcePO = new PowerSource_PO(o, dir, p, r, l, col, perm, cond, shader);
	powerSourcePO->updatePropertyMap();
	physicalObjectBuffer->insert(std::make_pair(powerSourcePO->getId(), powerSourcePO));
	postObjectInsertionSetup();
}

void MainScene::addPowerDetector(std::string& s, glm::vec3 o, glm::vec3 dir, float sens,
	float r, float l, glm::vec3 col, float perm, float cond)
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
	PowerDetector_PO* powerDetector_PO = new PowerDetector_PO(o, dir, sens, r, l, col, perm, cond, shader);
	powerDetector_PO->updatePropertyMap();
	physicalObjectBuffer->insert(std::make_pair(powerDetector_PO->getId(), powerDetector_PO));
	postObjectInsertionSetup();
}

void MainScene::addTuningPadArray(std::string& s, glm::vec3 o, int npX, int npZ,
	float padSepX, float padSepZ, float sX, float sZ, glm::vec3 col, float perm, float cond)
{
	Shader* shader;
	std::string name = "Cuboid Instanced Array";
	if (shaderMap->find(name) == shaderMap->end())
	{
		shader = new Shader("res/shaders/cuboid_instanced_array.shader", 1, name);
		shaderMap->insert(std::make_pair(name, shader));
	}
	else {
		shader = shaderMap->at(name);
	}
	shader->incrNumObjectsServed();
	TuningPadArray* tuningArrayPaw_PO = new TuningPadArray(o, npX, npZ, padSepX, padSepZ, sX, sZ,
		col, perm, cond, shader);
	tuningArrayPaw_PO->updatePropertyMap();
	physicalObjectBuffer->insert(std::make_pair(tuningArrayPaw_PO->getId(), tuningArrayPaw_PO));
	postObjectInsertionSetup();
}

void MainScene::addWire(std::string& s, glm::vec3 o, float l, float w, 
	float h, glm::vec3 col, float perm, float cond)
{
	Shader* shader;
	std::string name = "Wire";
	if (shaderMap->find(name) == shaderMap->end())
	{
		shader = new Shader("res/shaders/testingline.shader", 1, name);
		shaderMap->insert(std::make_pair(name, shader));
	}
	else {
		shader = shaderMap->at(name);
	}
	shader->incrNumObjectsServed();
	Wire_PO* wire = new Wire_PO(o, l, w, h, col, perm, cond, shader);
	wire->updatePropertyMap();
	physicalObjectBuffer->insert(std::make_pair(wire->getId(), wire));
	postObjectInsertionSetup();



	activeWire = wire;
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
			deleteModifyingVectors();
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
	modifyingVectors = nullptr;
	activeObject = nullptr;
	propertyWindow->nullify();
}

void MainScene::generateRay(glm::vec3 pos, glm::vec3 dir)
{
	testingLine->terminateLine();
	testingLine->addPoint(pos);
	testingLine->addPoint(pos + dir * RAY_LENGTH);
	testingLine->terminateLineWithBuiltIndexIncr();
}

// TODO inefficient ray picking
void MainScene::selectObject(glm::vec3 pos, glm::vec3 dir)
{
	glm::vec3 rayPos = pos;
	float count = 0.0f;
	// checking high priority objects first
	if (modifyingVectors != nullptr)
	{
		while (count < SELECTING_OBJECT_RANGE)
		{
			glm::vec3 v = modifyingVectors->intersectionDirection(rayPos);
			if (v != glm::vec3(0.0f))
			{
				_modifyingVectorsActivated = true;
				_modifyingVectorsDirection = v;
				return;
			}
			rayPos += dir * SELECTING_OBJECT_PRECISION;
			count += SELECTING_OBJECT_PRECISION;
		}
	}
	_modifyingVectorsActivated = false;
	rayPos = pos;
	count = 0.0f;
	// checking for the rest of the objects
	while (count < SELECTING_OBJECT_RANGE) {
		for (auto &pair : *physicalObjectBuffer) {
			PhysicalObject* obj = pair.second;
			// make sure this check works
			if (obj == modifyingVectors) {
				continue;
			}
			if (obj->intersectionCheck(rayPos)) {
				std::cout << "object selected!" << std::endl;
				highestClickedObjPoint = *obj->getOrigin() +
					glm::vec3(0.0f, *obj->getHeight(), 0.0f);
				propertyWindow->addPropertyMap(obj->getPropertyMap());
				propertyWindow->addPropertyMapInt(obj->getPropertyMapInt());
				propertyWindow->updateActiveObject(obj);
				activeObject = obj;
				extern bool _propertyWindowOn;
				_propertyWindowOn = true;
				if (modifyingVectors != nullptr)
				{
					modifyingVectors->setOrigin(activeObject->getCenterLocation());
					modifyingVectors->setRebuiltExpected(true);
				}
				return;
			}
		}
		rayPos += dir * SELECTING_OBJECT_PRECISION;
		count += SELECTING_OBJECT_PRECISION;
	}
	activeObject = nullptr;
	propertyWindow->setDefaultPropertyMap();
	propertyWindow->updateActiveObject(nullptr);
	deleteModifyingVectors();
	highestClickedObjPoint = glm::vec3(0.0f);
	std::cout << "no objects found!" << std::endl;
}

void MainScene::buildModifyingVectors(PhysicalObject* obj, ModyfingVectorType type)
{
	if (modifyingVectors != nullptr || obj == nullptr)
	{
		return;
	}
	ModifyingVectors_PO* ret = nullptr;

	Shader* shader;
	std::string name = "Generic shader";
	if (shaderMap->find(name) == shaderMap->end())
	{
		shader = new Shader("res/shaders/generic.shader", 1, name);
		shaderMap->insert(std::make_pair(name, shader));
	}
	else {
		shader = shaderMap->at(name);
	}

	switch (type) {
	case Translation:
		ret = new ModifyingVectors_PO(MODIFYING_VECTORS_INITIAL_SIZE,
			type, obj->getCenterLocation(), shader);
		ret->setId(physicalObjectNextIdMax--);
		//ret->setInteractable(false);
		break;
	case Scaling:
		ret = new ModifyingVectors_PO(MODIFYING_VECTORS_INITIAL_SIZE,
			type, obj->getCenterLocation(), shader);
		ret->setId(physicalObjectNextIdMax--);
		break;
	case Rotation:

		break;
	}
	if (ret != nullptr)
	{
		modifyingVectors = ret;
		//increment twice not to recreate the same shader each time
		shader->incrNumObjectsServed();
		shader->incrNumObjectsServed();
		physicalObjectBuffer->insert(std::make_pair(ret->getId(), ret));
		shaderMap->insert(std::make_pair(shader->getName(), shader));
		_viewMatrixChanged = true;
		_projMatrixChanged = true;
	}
}

void MainScene::deleteModifyingVectors()
{
	if (modifyingVectors != nullptr)
	{
		physicalObjectBuffer->erase(modifyingVectors->getId());
		eraseShaderMapOneInstance(modifyingVectors->getShaderName());
		modifyingVectors = nullptr;
		delete modifyingVectors;
	}
}

void MainScene::preObjectInsertionSetup() 
{

}

void MainScene::postObjectInsertionSetup() 
{
	_viewMatrixChanged = true;
	_projMatrixChanged = true;
}

void MainScene::clearLines()
{
	testingLine->clear();
}

DimensionalCurve_POT* MainScene::getActiveWire()
{
	if (activeWire != nullptr)
	{
		return activeWire;
	}
	return nullptr;
}

glm::vec3 MainScene::getHighestClickedObjPoint()
{
	return highestClickedObjPoint;
}

void MainScene::updateHighestClickedObjPoint(glm::vec3 pos, glm::vec3 dir)
{
	glm::vec3 rayPos = pos;
	float count = 0.0f;
	if (modifyingVectors != nullptr)
	{
		while (count < SELECTING_OBJECT_RANGE)
		{
			glm::vec3 v = modifyingVectors->intersectionDirection(rayPos);
			if (v != glm::vec3(0.0f))
			{
				_modifyingVectorsActivated = true;
				_modifyingVectorsDirection = v;
				return;
			}
			rayPos += dir * SELECTING_OBJECT_PRECISION;
			count += SELECTING_OBJECT_PRECISION;
		}
	}
	_modifyingVectorsActivated = false;
	rayPos = pos;
	count = 0.0f;
	// checking for the rest of the objects
	while (count < SELECTING_OBJECT_RANGE) {
		for (auto& pair : *physicalObjectBuffer) {
			PhysicalObject* obj = pair.second;
			// make sure this check works
			if (obj == modifyingVectors || obj->getObjectType() == CURVE) {
				continue;
			}
			if (obj->intersectionCheck(rayPos)) {
				highestClickedObjPoint = *obj->getOrigin() +
					glm::vec3(0.0f, *obj->getHeight(), 0.0f);
				return;
			}
		}
		rayPos += dir * SELECTING_OBJECT_PRECISION;
		count += SELECTING_OBJECT_PRECISION;
	}
}

// ignores wires
PhysicalObject* MainScene::higherPointObject(glm::vec3 v, glm::vec3 maxV)
{
	for (auto& pair : *physicalObjectBuffer) {
		PhysicalObject* obj = pair.second;
		if (obj == modifyingVectors || obj->getObjectType() == CURVE) {
			continue;
		}
		if (obj->intersectionCheck(v)) {
			float objYLoc = *obj->getOriginY() + *obj->getHeight();
			if (objYLoc >= v.y &&
				objYLoc < maxV.y)
			{
				return obj;
			}
		}
	}
	return nullptr;
}

