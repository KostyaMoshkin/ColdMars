#pragma once

#include "DataContextEngine.h"
#include "ContextContainer.h"
#include "ContextInterface.h"
#include "ControlContext.h"

#include <vcclr.h>
#include <memory>

#include "SceneRender.h"

#include "RenderMegdr.h"

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

		lib::XMLreaderPtr m_pXMLconfig;
		RenderMegdrPtr m_pRenderMegdr;

		System::IntPtr m_hWnd;

		bool m_bInit = false;
	
	public:
		Bridge();
		~Bridge();

		static BridgePtr Create() { return std::make_shared<Bridge>(); }

	public:
		void draw(bool bForce_ = false) override;

		void on_handle_created() override;
		void on_handle_changed() override;
		void on_handle_destroyed() override;

		bool isInit();

	public:
		ContextInterface^ getScreen();

		ControlContextPtr getContext();

	};

}
