#include "pch.h"
#include "Bridge.h"
#include "ContextContainer.h"
#include "Screen.h"

namespace GL
{
	Bridge::Bridge()
	{
		DataContextEngine* pDataContextEngine = dynamic_cast<DataContextEngine*>(this);
		m_pScreen = gcnew Screen(System::IntPtr(pDataContextEngine));
		m_pContextContainer = gcnew ContextContainer(m_pScreen);

		m_pControlContext = ControlContext::Create();

		m_pSceneRender = SceneRender::Create();
		
		m_pXMLconfig = lib::XMLreader::Create("D:\\VisualStudioProjects\\ColdMars\\x64\\Debug\\ColdMars.config");


	}

	Bridge::~Bridge()
	{
	}

	void Bridge::draw(bool bForce_)
	{
		if (!m_bInit)
			return;

		if (!m_pRenderMegdr)
		{
			m_pRenderMegdr = RenderMegdr::Create();

			m_pRenderMegdr->setConfig(m_pXMLconfig->getRoot());

			int nVersionFull = m_pSceneRender->GetVersion();
			m_pRenderMegdr->setVersionGl(nVersionFull);

			if (!m_pRenderMegdr->init())
			{
				m_pSceneRender.reset();
				//toLog("OpenGL RenderMegdr init ERROR");
				return;
			}
			m_pRenderMegdr->setVisible(true);
			m_pSceneRender->addElement(m_pRenderMegdr);

			lib::Vector3 vCamPosition3D(0, 0, -6.5);
			lib::Vector3 vCamRight3D(1, 0, 0);

			m_pSceneRender->lookAt(vCamPosition3D, lib::Vector3(0, 0, 0), lib::Vector3(0, 1, 0));
			m_pSceneRender->rotate(0);
			m_pSceneRender->mouseScroll(0.0f);

		}

		m_pControlContext->begin_draw(m_pContextContainer->Size.Width, m_pContextContainer->Size.Height);

		m_pSceneRender->draw();

		m_pControlContext->end_draw();
	}

	void Bridge::on_handle_created()
	{
		m_hWnd = m_pContextContainer->hWnd;

		m_pControlContext->initContext((HWND)m_hWnd.ToPointer());

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

	bool Bridge::isInit()
	{
		return m_bInit;
	}

	ContextInterface^ Bridge::getScreen()
	{
		return m_pScreen;
	}

	ControlContextPtr Bridge::getContext()
	{
		return m_pControlContext;
	}
}
