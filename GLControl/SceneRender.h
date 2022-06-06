#pragma once
#include "SceneRenderBase.h"
#include "Render.h"
#include "lib.h"
#include "Vocabulary.h"
#include "CConfig.h"

#include <memory>
#include <vector>

namespace GL {

	class SceneRender;
	using SceneRenderPtr = std::shared_ptr<SceneRender>;

	class SceneRender : public lib::CConfig
	{
		static const char* ViewAngle() { return "ViewAngle"; }

		GLFWwindow* m_pWindow = nullptr;

		float m_fViewAspect = 1.0f;
		float m_fViewAngle = 45.0f;
		float m_fRotate = 0.0f;

		bool m_bScrolled = false;

		std::vector<GL::RenderPtr> m_vElementRendr;

		//  Состояние клавиш
		std::vector<bool> m_vKeyPress;

		lib::fPoint2D m_cursorStartPos;
		lib::fPoint2D m_cursorCurrentPos;
		lib::fPoint2D m_cursorMove;

	public:
		SceneRender();
		~SceneRender();

		static SceneRenderPtr Create() { return std::make_shared<SceneRender>(); }

	private:
		//virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
		//virtual void mouseMoveCallback(GLFWwindow* window, double fPosX, double fPosY) override;
		//virtual void mouseScrollCallback(GLFWwindow* window, double fPosX, double fPosY) override;
		//virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;

	public:
		bool init();

		bool WindowShouldClose();

		void SwapBuffers();

		void clearScreen();

		int GetVersion();

		void PollEvents();

	public:
		void addElement(GL::RenderPtr pRender_);

		void draw();

		void setViewAngle();

		void rotate(float fAngle_);

		void lookAt(lib::dPoint3D fCamPosition_, lib::dPoint3D vCamCenter_, lib::dPoint3D vCamUp_);

		void mouseScroll(float fZoom_);

	public:

		bool isKeyPress(EKeyPress nKey_);

		virtual bool keyPress(GL::EKeyPress nKey_);

		bool isCursoreMove();
		bool isScrolled();
		lib::fPoint2D getCursorMove();
		bool isInteraction();

	public:
		static bool read_error(bool check_error_, const char* szFileName = __FILE__, unsigned nLine = __LINE__, const char* szDateTime = __TIMESTAMP__);

	};
}

