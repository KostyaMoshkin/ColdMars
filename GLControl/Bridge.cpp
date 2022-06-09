#include "pch.h"
#include "Bridge.h"
#include "ContextContainer.h"
#include "Screen.h"

#include <GLEW/glew.h>

namespace GL
{
	Bridge::Bridge()
	{
		DataContextEngine* pDataContextEngine = dynamic_cast<DataContextEngine*>(this);
		m_pScreen = gcnew Screen(System::IntPtr(pDataContextEngine));
		m_pContextContainer = gcnew ContextContainer(m_pScreen);

		m_pControlContext = ControlContext::Create();

		m_pSceneRender = SceneRender::Create();
		
		m_pXMLconfig = lib::XMLreader::Create("ColdMars.config");

		m_pSceneRender->setConfig(m_pXMLconfig->getRoot());
		m_pSceneRender->init();

		m_vCamPosition3D = lib::Vector3(0, 0, -3.5);
		m_vCamRight3D = lib::Vector3(1, 0, 0);

	}

	Bridge::~Bridge()
	{
	}

	bool Bridge::init()
	{
		bool bFirsfInit = !m_pRenderMegdr && !m_pRenderOrbitTemperature;

		if (!bFirsfInit)
			return true;

		m_pControlContext->begin_draw(m_pScreen->ViewControl->Size.Width, m_pScreen->ViewControl->Size.Height);

		//--------------------------------------------------------------------------

		if (!m_pRenderMegdr)
		{
			m_pRenderMegdr = RenderMegdr::Create();

			m_pRenderMegdr->setConfig(m_pXMLconfig->getRoot());

			int nVersionFull = m_pSceneRender->GetVersion();
			m_pRenderMegdr->setVersionGl(nVersionFull);

			if (!m_pRenderMegdr->init())
			{
				m_pSceneRender.reset();
				m_bInit = false;
				//toLog("OpenGL RenderMegdr init ERROR");
				return false;
			}
			m_pRenderMegdr->setVisible(true);
			m_pSceneRender->addElement(m_pRenderMegdr);
		}

		//--------------------------------------------------------------------------

		if (!m_pRenderOrbitTemperature)
		{
			m_pRenderOrbitTemperature = RenderOrbitTemperature::Create();

			m_pRenderOrbitTemperature->setConfig(m_pXMLconfig->getRoot());

			int nVersionFull = m_pSceneRender->GetVersion();
			m_pRenderOrbitTemperature->setVersionGl(nVersionFull);

			if (!m_pRenderOrbitTemperature->init())
			{
				m_pSceneRender.reset();
				m_bInit = false;
				//toLog("OpenGL RenderOrbitTemperature init ERROR");
				return false;
			}
			m_pRenderOrbitTemperature->setVisible(true);
			m_pSceneRender->addElement(m_pRenderOrbitTemperature);
		}

		//--------------------------------------------------------------------------

		lib::Vector3 vCamPosition3D(0, 0, -3.5);
		lib::Vector3 vCamRight3D(1, 0, 0);

		m_pSceneRender->lookAt(vCamPosition3D, lib::Vector3(0, 0, 0), lib::Vector3(0, 1, 0));
		m_pSceneRender->rotate(0);
		m_pSceneRender->translate(0, 0);
		m_pSceneRender->setViewAngle(0.0f);

		//--------------------------------------------------------------------------

		m_pControlContext->end_draw();

		//--------------------------------------------------------------------------

		std::string sOrbitDir;
		if (!lib::XMLreader::getSting(lib::XMLreader::getNode(m_pXMLconfig->getRoot(), OrbitDir()), sOrbitDir))
			return false;

		m_vFileList = lib::create_file_list(sOrbitDir.c_str());

		//--------------------------------------------------------------------------

		return true;
	}

	void Bridge::draw()
	{
		if (!m_bInit)
			return;

		m_pControlContext->begin_draw(m_pScreen->ViewControl->Size.Width, m_pScreen->ViewControl->Size.Height);

		m_pSceneRender->draw();

		m_pControlContext->end_draw();
	}

	void Bridge::on_handle_created()
	{
		m_hWnd = m_pContextContainer->hWnd;

		m_pControlContext->initContext((HWND)m_hWnd.ToPointer());

		init();

		m_bInit = true;
	}

	void Bridge::on_handle_changed()
	{
		m_hWnd = m_pContextContainer->hWnd;

		if (m_hWnd == System::IntPtr::Zero)
			return;

		if (!m_pControlContext->initContext((HWND)m_hWnd.ToPointer()))
			return;
	}

	void Bridge::on_handle_destroyed()
	{
		m_pControlContext->releaseContext();
		m_hWnd = System::IntPtr::Zero;
	}

	void Bridge::on_mouse_scroll(float fScroll_)
	{
		m_fViewAngle += fScroll_;

		m_pControlContext->begin_draw();

		m_pSceneRender->setViewAngle(m_fViewAngle);
		m_pSceneRender->draw();

		m_pControlContext->end_draw();
	}

	void Bridge::on_mouse_left_btn_move(int nMoveX_, int nMoveY_)
	{
		lib::Quat qIncline = lib::makeQuat(-m_fInclineAngle * nMoveY_, m_vCamRight3D);
		m_vCamPosition3D = qIncline * m_vCamPosition3D;
		lib::Vector3 vCamUp = glm::cross(-m_vCamPosition3D, m_vCamRight3D);

		m_fRotate += -m_fRoteteAngle * nMoveX_;

		//------------------------------------------------------------------------------

		m_pControlContext->begin_draw();

		m_pSceneRender->lookAt(m_vCamPosition3D, lib::Vector3(0, 0, 0), vCamUp);
		m_pSceneRender->rotate(m_fRotate);

		m_pSceneRender->draw();

		m_pControlContext->end_draw();
	}

	void Bridge::on_mouse_right_btn_move(int nMoveX_, int nMoveY_)
	{
		m_fMoveX -= 1.0f * nMoveX_ / m_pScreen->ViewControl->Size.Width;
		m_fMoveY += 1.0f * nMoveY_ / m_pScreen->ViewControl->Size.Width;

		m_pControlContext->begin_draw();

		m_pSceneRender->translate(m_fMoveX, m_fMoveY);

		m_pSceneRender->draw();

		m_pControlContext->end_draw();
	}

	void Bridge::on_mouse_double_click()
	{
		lib::Vector3 vCamPosition3D(0, 0, -3.5);
		lib::Vector3 vCamRight3D(1, 0, 0);
		m_vCamPosition3D = lib::Vector3(0, 0, -3.5);

		m_fRotate = 0;
		m_fMoveX = 0.0f;
		m_fMoveY = 0.0f;
		m_fViewAngle = 0.0f;


		m_pControlContext->begin_draw();

		m_pSceneRender->lookAt(vCamPosition3D, lib::Vector3(0, 0, 0), lib::Vector3(0, 1, 0));
		m_pSceneRender->rotate(0);
		m_pSceneRender->translate(0, 0);
		m_pSceneRender->setViewAngle(0.0f);

		m_pSceneRender->draw();

		m_pControlContext->end_draw();
	}

	bool Bridge::isInit()
	{
		return m_bInit;
	}

	int Bridge::getFileCount()
	{
		return (int)m_vFileList.size();
	}

	void Bridge::setFileRange(int nFirstFile_, int nLasetFile_)
	{
		m_pRenderOrbitTemperature->setFileRange(nFirstFile_, nLasetFile_);

		draw();
	}

	std::string Bridge::getOrbit(unsigned nIndex_)
	{
		if (nIndex_ >= m_vFileList.size())
			return std::string();

		std::string sFileName = m_vFileList[nIndex_].c_str();

		size_t nPointPos = sFileName.find(".");
		size_t nNamePos = sFileName.find_last_of("\\") + 2;

		std::string  sNumber = sFileName.substr(nNamePos, nPointPos - nNamePos);

		return sNumber;
	}

	ContextInterface^ Bridge::getScreen()
	{
		return m_pScreen;
	}

	ControlContextPtr Bridge::getContext()
	{
		return m_pControlContext;
	}

	unsigned Bridge::getSpectrumNumb()
	{
		return m_pRenderOrbitTemperature->getReader()->getSpectrumNumb();
	}

	unsigned Bridge::getInterferogramID()
	{
		return m_pRenderOrbitTemperature->getReader()->getInterferogramID();
	}

	float Bridge::getJulianDate()
	{
		return m_pRenderOrbitTemperature->getReader()->getJulianDate();
	}

	float Bridge::getLocalTime()
	{
		return m_pRenderOrbitTemperature->getReader()->getLocalTime();
	}

	std::string Bridge::getUTC()
	{
		return m_pRenderOrbitTemperature->getReader()->getUTC();
	}

	void Bridge::getPalette(std::vector<lib::iPoint3D>& vPalette_, int& nPaletteMin_, int& nPaletteMax_)
	{
		m_pRenderOrbitTemperature->getPalette(vPalette_, nPaletteMin_, nPaletteMax_);
	}
}
