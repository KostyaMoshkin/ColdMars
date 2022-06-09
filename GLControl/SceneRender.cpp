#include "pch.h"

#include "SceneRender.h"

#include <string>

#include <LOG/logger.h>

namespace GL {

	SceneRender::SceneRender()
	{
		m_vElementRendr.clear();
	}

	SceneRender::~SceneRender()
	{
	}

	bool SceneRender::init()
	{
		unsigned nViewAngle = 45;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), ViewAngle()), nViewAngle))
			nViewAngle = 45;

		m_fViewAngle = float(nViewAngle);

		return true;
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

	void SceneRender::addElement(RenderPtr pRender_)
	{
		m_vElementRendr.emplace_back(pRender_);
	}

	void SceneRender::draw()
	{
		clearScreen();

		for (RenderPtr pElement : m_vElementRendr)
			pElement->draw();
	}

	void SceneRender::setViewAngle(float fZoom_)
	{
		lib::limit(m_fViewAngle, 1.0f, 150.0f);

		lib::Matrix4 mPerspective = glm::perspective(glm::radians(m_fViewAngle + fZoom_), m_fViewAspect, 0.1f, 50.0f);

		for (RenderPtr pElement : m_vElementRendr)
			pElement->setViewAngle(mPerspective);
	}

	void SceneRender::rotate(float fAngle_)
	{
		lib::Matrix4 rotate = lib::Matrix4(1.0f);
		rotate = glm::rotate(rotate, 0.2f, glm::vec3(0, 0, 1));
		rotate = glm::rotate(rotate, fAngle_, glm::vec3(0, 1, 0));

		for (RenderPtr pElement : m_vElementRendr)
			pElement->rotate(rotate);
	}

	void SceneRender::translate(float fMoveV_, float fMoveY_)
	{
		lib::Matrix4 mTranslate = lib::Matrix4(1.0f);
		mTranslate = glm::translate(mTranslate, lib::Vector3(fMoveV_, 0, 0));
		mTranslate = glm::translate(mTranslate, lib::Vector3(0, fMoveY_, 0));

		for (RenderPtr pElement : m_vElementRendr)
			pElement->translate(mTranslate);
	}

	void SceneRender::lookAt(lib::dPoint3D fCamPosition_, lib::dPoint3D vCamCenter_, lib::dPoint3D vCamUp_)
	{
		lib::Matrix4 mView = glm::lookAt(fCamPosition_, glm::normalize(fCamPosition_ - vCamCenter_), glm::normalize(vCamUp_));  //  eye, center, up

		for (RenderPtr pElement : m_vElementRendr)
			pElement->lookAt(mView);
	}

	//void SceneRender::mouseScroll(float fZoom_)
	//{
	//	m_fViewAngle += fZoom_;

	//	setViewAngle();
	//}

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
}
