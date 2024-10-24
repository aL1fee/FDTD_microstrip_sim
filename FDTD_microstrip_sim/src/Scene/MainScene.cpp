#include "MainScene.h"

extern bool _rebuildObjects;

void MainScene::init()
{
	csys = CoordinateSystem(20.0f, 0.6f);
	csys.run();

	viewCamLoc = shader_csys.getUniformLocation("view");
	projCamLoc = shader_csys.getUniformLocation("proj");
	colCsysLoc = shader_csys.getUniformLocation("color");

	mainGridColor = csys.getMainColor();
	helperGridColor = csys.getHelperColor();

	viewMatrix = cam->lookAt();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	projMatrix = glm::perspective(glm::radians(cam->getZoom()), aspect, 0.1f, 100.0f);

	testingLine = new TestingLine();

	tempCarrierBuffer = new std::vector<Carrier_PO*>();

	propertyWindow = new PropertyWindow();
}

void MainScene::render()
{
	if (_projMatrixChanged) {
		//int w, h;
		//glfwGetFramebufferSize(window, &w, &h);
		//aspect = (float)w / (float)h;
		projMatrix = glm::perspective(glm::radians(cam->getZoom()), aspect, 0.1f, 100.0f);
		shader_csys.bind();
		glUniformMatrix4fv(projCamLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));
		shader_csys.unbind();
		shader_carrier.bind();
		glUniformMatrix4fv(shader_carrier.getUniformLocation("proj"), 1, GL_FALSE, glm::value_ptr(projMatrix));
		shader_carrier.unbind();
		_projMatrixChanged = false;
	}
	if (_viewMatrixChanged) {
		viewMatrix = cam->lookAt();
		shader_csys.bind();
		glUniformMatrix4fv(viewCamLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		shader_csys.unbind();
		shader_carrier.bind();
		glUniformMatrix4fv(shader_carrier.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		shader_carrier.unbind();
		_viewMatrixChanged = false;
	}

	shader_csys.bind();
	shader_csys.setUniform3f("color", 0.0f, 0.0f, 1.0f);
	testingLine->build();
	testingLine->draw();

	shader_csys.setUniform3f("color", mainGridColor.x, mainGridColor.y, mainGridColor.z);
	csys.drawMain();
	shader_csys.setUniform3f("color", helperGridColor.x, helperGridColor.y, helperGridColor.z);
	csys.drawHelper();
	shader_csys.unbind();


	shader_carrier.bind();
	for (Carrier_PO* obj : *tempCarrierBuffer) {
		if (_rebuildObjects) {
			obj->build();
		}
		obj->draw();
	}
	shader_carrier.unbind();




}

void MainScene::addCarrier(std::string& s)
{
	int i = 0;
	s = "Please choose point 0";
	//Carrier_PO* carrierPO = new Carrier_PO();

	Carrier_PO* carrierPO = new Carrier_PO(glm::vec3(0.0f), 4.0f, 1.0f, 0.6f, glm::vec3(.85f, .85f, .85f), 2.0f);
	//physicalObjectBuffer->push_back(carrierPO);
	tempCarrierBuffer->push_back(carrierPO);
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
		for (Carrier_PO* obj : *tempCarrierBuffer) {
			if (obj->intersectionCheck(rayPos)) {
				std::cout << "object selected!" << std::endl;
				propertyWindow->addPropertyMap(obj->getPropertyMap());
				extern bool _propertyWindowOn;
				_propertyWindowOn = true;
				return;
			}
		}
		rayPos += dir * SELECTING_OBJECT_PRECISION;
		count += SELECTING_OBJECT_PRECISION;
	}
	std::cout << "no objects found!" << std::endl;
}
