#include "MainScene.h"

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
}

void MainScene::render()
{
	shader_csys.bind();
	if (_projMatrixChanged) {
		//int w, h;
		//glfwGetFramebufferSize(window, &w, &h);
		//aspect = (float)w / (float)h;
		projMatrix = (glm::perspective(glm::radians(cam->getZoom()), aspect, 0.1f, 1000.0f));
		glUniformMatrix4fv(projCamLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));
		_projMatrixChanged = false;
	}
	if (_viewMatrixChanged) {
		viewMatrix = cam->lookAt();
		glUniformMatrix4fv(viewCamLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		_viewMatrixChanged = false;
	}

	shader_csys.setUniform3f("color", 0.0f, 0.0f, 1.0f);
	testingLine->build();
	testingLine->draw();

	shader_csys.setUniform3f("color", mainGridColor.x, mainGridColor.y, mainGridColor.z);
	csys.drawMain();
	shader_csys.setUniform3f("color", helperGridColor.x, helperGridColor.y, helperGridColor.z);
	csys.drawHelper();
	shader_csys.unbind();




}

void MainScene::addCarrier(std::string& s)
{
	int i = 0;
	s = "Please choose point 0";
	Carrier_PO* carrierPO = new Carrier_PO();
	physicalObjectBuffer->push_back(carrierPO);




}
