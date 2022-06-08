#pragma once

#include "DataContextEngine.h"
#include "ContextContainer.h"
#include "ContextInterface.h"
#include "ControlContext.h"

#include <vcclr.h>
#include <memory>

#include "SceneRender.h"

#include "RenderMegdr.h"
#include "RenderOrbitTemperature.h"

#include "lib.h"

#include "CConfig.h"

namespace GL
{
	class Bridge;
	using BridgePtr = std::shared_ptr<Bridge>;

	class Bridge : public DataContextEngine
	{
		static const char* OrbitDir() { return "OrbitDir"; }

		gcroot<ContextContainer^> m_pContextContainer;
		gcroot<ContextInterface^> m_pScreen;

		ControlContextPtr m_pControlContext = nullptr;

		SceneRenderPtr m_pSceneRender = nullptr;

		lib::XMLreaderPtr m_pXMLconfig = nullptr;

		RenderMegdrPtr m_pRenderMegdr = nullptr;
		RenderOrbitTemperaturePtr m_pRenderOrbitTemperature = nullptr;

		System::IntPtr m_hWnd;

		bool m_bInit = false;
		bool m_bMouseFolow = false;

		float m_fScroll = 0;
		float m_fRoteteAngle = 0.02f;
		float m_fInclineAngle = 0.25f;

		lib::Vector3 m_vCamPosition3D;
		lib::Vector3 m_vCamRight3D;

		std::vector<std::string> m_vFileList;
			
	public:
		Bridge();
		~Bridge();

		static BridgePtr Create() { return std::make_shared<Bridge>(); }

	private:
		bool init();

	public:
		void draw() override;

		void Scroll(float fValue_);

		void on_handle_created() override;
		void on_handle_changed() override;
		void on_handle_destroyed() override;

		void on_mouse_scroll(float fScroll_) override;
		void on_mouse_down() override;
		void on_mouse_up() override;
		void on_mouse_move(int nMoveX_, int nMoveY_) override;

		bool isInit();

		int getFileCount();

		void setFileRange(int nFirstFile_, int nLasetFile_);

		std::string getOrbit(unsigned nIndex_);

	public:
		ContextInterface^ getScreen();

		ControlContextPtr getContext();

	public:
		unsigned getSpectrumNumb();
		unsigned getInterferogramID();
		float getJulianDate();
		float getLocalTime();
		std::string getUTC();

	};

}
