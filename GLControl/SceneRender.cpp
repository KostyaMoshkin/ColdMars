#include "pch.h"

#include "SceneRender.h"

#include <string>

#include <LOG/logger.h>

namespace GL {

	SceneRender::SceneRender()
	{
		//glfwInit();

		m_vKeyPress.resize(120);

		m_vElementRendr.clear();
	}

	SceneRender::~SceneRender()
	{
		//glfwDestroyWindow(m_pWindow);
		//glfwTerminate();
	}

	bool SceneRender::init()
	{
//#ifdef __DEBUG
//		m_pWindow = glfwCreateWindow(1024, 768, "OpenGL", nullptr, nullptr);
//#else
//		m_pWindow = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
//			glfwGetVideoMode(glfwGetPrimaryMonitor())->height,
//			"OpenGL", glfwGetPrimaryMonitor(), nullptr);
//#endif // __DEBUG
//
//		if (m_pWindow == nullptr)
//		{
//			toLog("no window");
//			return false;
//		}
//
//		glfwMakeContextCurrent(m_pWindow);
//
//		glfwSetKeyCallback(m_pWindow, SceneRenderBase::keyCallbackDispatch);
//		glfwSetCursorPosCallback(m_pWindow, SceneRenderBase::mouseMoveCallbackDispatch);
//		glfwSetScrollCallback(m_pWindow, SceneRenderBase::mouseScrollCallbackDispatch);
//		glfwSetMouseButtonCallback(m_pWindow, SceneRenderBase::mouseButtonCallbackDispatch);
//
//		glewExperimental = true; // Флаг необходим в Core-режиме OpenGL
//
//		if( glewInit() != GLEW_OK )
//		{
//			toLog("glewInit error");
//			return false;
//		}
//
//		int nWidth, nHeight;
//		glfwGetFramebufferSize(m_pWindow, &nWidth, &nHeight);
//
//		m_fViewAspect = 1.0f * nWidth / nHeight;
//
//		m_cursorCurrentPos = lib::fPoint2D(float(nWidth / 2), float(nHeight / 2));
//		glfwSetCursorPos(m_pWindow, m_cursorCurrentPos.x, m_cursorCurrentPos.y);
//
//		glViewport(0, 0, nWidth, nHeight);
//
//		glEnable(GL_DEPTH_TEST);
//
//		glPointSize(3);

		unsigned nViewAngle = 45;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), ViewAngle()), nViewAngle))
			nViewAngle = 45;

		m_fViewAngle = float(nViewAngle);


		return true;
	}

	bool SceneRender::WindowShouldClose()
	{
		return true; // glfwWindowShouldClose(m_pWindow);
	}

	void SceneRender::SwapBuffers()
	{
		//glfwSwapBuffers(m_pWindow);
	}

	void SceneRender::clearScreen()
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	int SceneRender::GetVersion()
	{
		int nVersionFull = -1;

		const GLubyte* pVersion = glGetString(GL_VERSION);
		if (!pVersion)
			return nVersionFull;

		std::string sVersion = (const char*)pVersion;
		if (sVersion.length() < 3)
			return nVersionFull;

		int nVersionMain = sVersion[0] - '0';
		int nVersionSub = sVersion[2] - '0';

		nVersionFull = nVersionMain * 10 + nVersionSub;

		return nVersionFull;
	}
	
	void SceneRender::PollEvents()
	{
		//glfwPollEvents();
	}

	bool SceneRender::isKeyPress(EKeyPress nKey_)
	{
		return m_vKeyPress[(int)nKey_];
	}

	bool SceneRender::keyPress(GL::EKeyPress nKey_)
	{
		bool bError = false;

		for (RenderPtr pElement : m_vElementRendr)
			bError |= !pElement->keyPress(nKey_);

		return !bError;
	}

	void SceneRender::addElement(RenderPtr pRender_)
	{
		m_vElementRendr.emplace_back(pRender_);
	}

	lib::fPoint2D SceneRender::getCursorMove()
	{
		return std::exchange(m_cursorMove, { 0, 0 });
	}

	void SceneRender::draw()
	{
		clearScreen();

		for (RenderPtr pElement : m_vElementRendr)
			pElement->draw();

		m_bScrolled = false;

		//SwapBuffers();
	}

	void SceneRender::setViewAngle()
	{
		lib::limit(m_fViewAngle, 1.0f, 150.0f);

		lib::Matrix4 mPerspective = glm::perspective(glm::radians(m_fViewAngle), m_fViewAspect, 0.001f, 50.0f);

		for (RenderPtr pElement : m_vElementRendr)
			pElement->setViewAngle(mPerspective);
	}

	void SceneRender::rotate(float fAngle_)
	{
		m_fRotate += fAngle_;

		for (RenderPtr pElement : m_vElementRendr)
			pElement->rotate(m_fRotate);
	}

	void SceneRender::lookAt(lib::dPoint3D fCamPosition_, lib::dPoint3D vCamCenter_, lib::dPoint3D vCamUp_)
	{
		lib::Matrix4 mView = glm::lookAt(fCamPosition_, glm::normalize(fCamPosition_ - vCamCenter_), glm::normalize(vCamUp_));  //  eye, center, up

		for (RenderPtr pElement : m_vElementRendr)
			pElement->lookAt(mView);
	}

	bool SceneRender::isCursoreMove()
	{
		return !(m_cursorMove.x == 0 && m_cursorMove.y == 0);
	}

	bool SceneRender::isScrolled()
	{
		return m_bScrolled;
	}

	//void SceneRender::keyCallback(GLFWwindow* pWindow_, int nKey_, int nScancode_, int nAction_, int nMods_)
	//{
	//	bool bPress;

	//	if (nAction_ == GLFW_PRESS)
	//		bPress = true;
	//	else if (nAction_ == GLFW_RELEASE)
	//		bPress = false;
	//	else
	//		return;

	//	if ( nKey_ == GLFW_KEY_ESCAPE )
	//		m_vKeyPress[(int)EKeyPress::esk] = bPress;
	//	else if ( nKey_ == GLFW_KEY_1 )
	//		m_vKeyPress[(int)EKeyPress::key_1] = bPress;
	//	else if ( nKey_ == GLFW_KEY_2 )
	//		m_vKeyPress[(int)EKeyPress::key_2] = bPress;
	//	else if ( nKey_ == GLFW_KEY_3 )
	//		m_vKeyPress[(int)EKeyPress::key_3] = bPress;
	//	else if (nKey_ == GLFW_KEY_4)
	//		m_vKeyPress[(int)EKeyPress::key_4] = bPress;
	//	else if (nKey_ == GLFW_KEY_5)
	//		m_vKeyPress[(int)EKeyPress::key_5] = bPress;
	//	else if (nKey_ == GLFW_KEY_6)
	//		m_vKeyPress[(int)EKeyPress::key_6] = bPress;
	//	else if (nKey_ == GLFW_KEY_7)
	//		m_vKeyPress[(int)EKeyPress::key_7] = bPress;
	//	else if (nKey_ == GLFW_KEY_8)
	//		m_vKeyPress[(int)EKeyPress::key_8] = bPress;
	//	else if (nKey_ == GLFW_KEY_9)
	//		m_vKeyPress[(int)EKeyPress::key_9] = bPress;
	//	else if (nKey_ == GLFW_KEY_0)
	//		m_vKeyPress[(int)EKeyPress::key_0] = bPress;
	//	else if (nKey_ == GLFW_KEY_LEFT)
	//		m_vKeyPress[(int)EKeyPress::key_left] = bPress;
	//	else if (nKey_ == GLFW_KEY_RIGHT)
	//		m_vKeyPress[(int)EKeyPress::key_right] = bPress;
	//	else if (nKey_ == GLFW_KEY_UP)
	//		m_vKeyPress[(int)EKeyPress::key_up] = bPress;
	//	else if (nKey_ == GLFW_KEY_DOWN)
	//		m_vKeyPress[(int)EKeyPress::key_down] = bPress;
	//	else if (nKey_ == GLFW_KEY_DELETE)
	//		m_vKeyPress[(int)EKeyPress::key_delete] = bPress;
	//	else if (nKey_ == GLFW_KEY_PAGE_DOWN)
	//		m_vKeyPress[(int)EKeyPress::key_pagedown] = bPress;
	//	else if (nKey_ == GLFW_KEY_HOME)
	//		m_vKeyPress[(int)EKeyPress::key_home] = bPress;
	//	else if (nKey_ == GLFW_KEY_END)
	//		m_vKeyPress[(int)EKeyPress::key_end] = bPress;
	//	else if (nKey_ == GLFW_KEY_MINUS)
	//		m_vKeyPress[(int)EKeyPress::key_minus] = bPress;
	//	else if (nKey_ == GLFW_KEY_EQUAL)
	//		m_vKeyPress[(int)EKeyPress::key_equal] = bPress;
	//}

	bool SceneRender::isInteraction()
	{
		if (m_vKeyPress[(int)EKeyPress::esk])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_1])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_2])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_3])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_4])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_5])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_6])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_7])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_8])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_9])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_0])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_left])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_right])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_up])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_down])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_minus])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_equal])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_home])
			return true;
		else if (m_vKeyPress[(int)EKeyPress::key_end])
			return true;
		else if (isCursoreMove())
			return true;
		else if (isScrolled())
			return true;


		return false;
	}

	//void SceneRender::mouseMoveCallback(GLFWwindow* window_, double fPosX_, double fPosY_)
	//{
	//	if (!m_vKeyPress[(int)EKeyPress::mouse_left])
	//		return;

	//	m_cursorMove = { m_cursorCurrentPos.x - float(fPosX_), m_cursorCurrentPos.y - float(fPosY_) };

	//	m_cursorCurrentPos = { float(fPosX_), float(fPosY_) };
	//}

	void SceneRender::mouseScroll(float fZoom_)
	{
		m_fViewAngle += fZoom_;

		setViewAngle();
	}

	bool SceneRender::read_error(bool check_error_, const char* szFileName /*= __FILE__*/, unsigned nLine /*= __LINE__*/, const char* szDateTime /*= __TIMESTAMP__*/)
	{
		bool result = true;

		GLenum err;
		int nCount = 0;
		while (((err = glGetError()) != GL_NO_ERROR) && ++nCount < 1000)
		{
			if (check_error_)
				toLog("OpenGL error: " + std::to_string(err));

			result = false;
		}

		return result;
	}

	//void SceneRender::mouseScrollCallback(GLFWwindow* window, double fPosX, double fPosY)
	//{
	//	m_bScrolled = true;

	//	m_fViewAngle += fPosY;

	//	setViewAngle();
	//}

	//void SceneRender::mouseButtonCallback(GLFWwindow* window, int nKey_, int nAction_, int mods)
	//{
	//	bool bPress;

	//	if (nAction_ == GLFW_PRESS)
	//		bPress = true;
	//	else if (nAction_ == GLFW_RELEASE)
	//		bPress = false;
	//	else
	//		return;

	//	double fPosX, fPosY;
	//	glfwGetCursorPos(m_pWindow, &fPosX, &fPosY);
	//	m_cursorCurrentPos = { (float)fPosX, (float)fPosY };

	//	if (nKey_ == GLFW_MOUSE_BUTTON_LEFT)
	//		m_vKeyPress[(int)EKeyPress::mouse_left] = bPress;
	//	else if (nKey_ == GLFW_MOUSE_BUTTON_RIGHT)
	//		m_vKeyPress[(int)EKeyPress::mouse_right] = bPress;
	//}

}
