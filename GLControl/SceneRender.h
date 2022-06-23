#pragma once
#include "Render.h"
#include "CConfig.h"

namespace GL {

	class SceneRender;
	using SceneRenderPtr = std::shared_ptr<SceneRender>;

	class SceneRender : public lib::CConfig
	{
		float m_fViewAngle = 45.0f;

		std::vector<GL::RenderPtr> m_vElementRendr;

		int m_nScreenWidth = 640;
		int m_nScreenHeight = 480;

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

		void sizeChanged(int nWidth_, int nHeight_);

		void setViewAngle(float fZoom_);

		void rotate(float fAngle_);

		void translate(float fMoveV_, float fMoveY_);

		void lookAt(lib::dPoint3D fCamPosition_, lib::dPoint3D vCamCenter_, lib::dPoint3D vCamUp_);

	public:
		static bool read_error(bool check_error_, const char* szFileName = __FILE__, unsigned nLine = __LINE__, const char* szDateTime = __TIMESTAMP__);

	};
}

