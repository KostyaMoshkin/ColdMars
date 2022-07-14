#pragma once

#include "DataContextEngine.h"
#include "ContextContainer.h"
#include "ContextInterface.h"
#include "ControlContext.h"

#include <vcclr.h>

#include "SceneRender.h"

#include "RenderMegdr.h"
#include "RenderOrbitTemperature.h"

#include "CConfig.h"

namespace GL
{
	class Bridge;
	using BridgePtr = std::shared_ptr<Bridge>;

	class Bridge : public DataContextEngine
	{
		gcroot<ContextContainer^> m_pContextContainer;
		gcroot<ContextInterface^> m_pScreen;

		ControlContextPtr m_pControlContext = nullptr;

		SceneRenderPtr m_pSceneRender = nullptr;

		lib::XMLreaderPtr m_pXMLconfig = nullptr;

		RenderMegdrPtr m_pRenderMegdr = nullptr;
		RenderOrbitTemperaturePtr m_pRenderOrbitTemperature = nullptr;

		System::IntPtr m_hWnd;

		bool m_bInit = false;

		float m_fRoteteAngle = 0.005f;
		float m_fInclineAngle = 0.25f;

		float m_fViewAngle = 0.0f;
		float m_fRotate = 3.14159f;
		float m_fMoveX = 0.0f;
		float m_fMoveY = 0.0f;

		lib::Vector3 m_vCamPosition3D;
		lib::Vector3 m_vCamRight3D;

	public:
		Bridge();
		~Bridge();

		void resetView();

		static BridgePtr Create() { return std::make_shared<Bridge>(); }

	private:
		bool init();

	public:
		void draw() const override;

		void sizeChanged(int nWidth_, int nHeight_) const override;

		void on_handle_created() override;
		void on_handle_changed() override;
		void on_handle_destroyed() override;

		void on_mouse_scroll(float fScroll_) override;
		void on_mouse_left_btn_move(int nMoveX_, int nMoveY_) override;
		void on_mouse_right_btn_move(int nMoveX_, int nMoveY_) override;
		void on_mouse_double_click() override;
		lib::fPoint2D on_mouse_click(int nPosX_, int nPosY_) override;

		bool isInit();

		unsigned getOrbitCount();

		void setFileArray(const std::vector<unsigned>& vOrbit_);

		void setFileRange(unsigned nFirstFile_, unsigned nLasetFile_);

		unsigned getOrbit_by_index(unsigned nIndex_);
		unsigned getOrbit_by_LS(unsigned nIndex_);
		unsigned getOrbitIndex_by_OrbitNumber(unsigned nIndex_);

	public:
		ContextInterface^ getScreen();

		ControlContextPtr getContext();

		std::vector<unsigned> getOrbitListByCoord(float fLatitude_, float fLongitude_) const;

	public:
		unsigned getSpectrumNumb();
		float getLS();
		float getJulianDate();
		float getLocalTime();
		std::string getUTC();
		int getOrbitAltitudeMax();

		void getPalette(std::vector<lib::iPoint3D>& vPalette_, int& nPaletteMin_, int& nPaletteMax_);

		float getScale();
		void setScale(float fScale_);

		void setIncludeAtmosphere(bool bInclude_);

		void setLocalTimeFilter(double fLocalTimeStart_, double fLocalTimeEnd_);
	};

}
