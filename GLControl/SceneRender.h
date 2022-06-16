#pragma once
#include "Render.h"
#include "Vocabulary.h"
#include "CConfig.h"

namespace GL {

	class SceneRender;
	using SceneRenderPtr = std::shared_ptr<SceneRender>;

	class SceneRender : public lib::CConfig
	{
		static const char* ViewAngle() { return "ViewAngle"; }

		float m_fViewAspect = 1.0f;
		float m_fViewAngle = 45.0f;

		std::vector<GL::RenderPtr> m_vElementRendr;

	public:
		SceneRender();
		~SceneRender();

		static SceneRenderPtr Create() { return std::make_shared<SceneRender>(); }

	public:
		bool init();

		void clearScreen();

		int GetVersion();

	public:
		void addElement(GL::RenderPtr pRender_);

		void draw();

		void setViewAngle(float fZoom_);

		void rotate(float fAngle_);

		void translate(float fMoveV_, float fMoveY_);

		void lookAt(lib::dPoint3D fCamPosition_, lib::dPoint3D vCamCenter_, lib::dPoint3D vCamUp_);

		//void mouseScroll(float fZoom_);

	public:
		static bool read_error(bool check_error_, const char* szFileName = __FILE__, unsigned nLine = __LINE__, const char* szDateTime = __TIMESTAMP__);

	};
}

