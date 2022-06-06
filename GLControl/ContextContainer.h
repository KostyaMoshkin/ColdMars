#pragma once

#include "ContextInterface.h"

namespace GL
{
	ref class ContextContainer : public System::Windows::Forms::UserControl
	{
		ContextInterface^ m_pContextInterface;

	private: System::Windows::Forms::Panel^ panel;

	public:
		ContextContainer(ContextInterface^ pContextInterface_);

		void InitializeComponent(void);

	public:
		property System::IntPtr hWnd
		{
			System::IntPtr get();
		}

	};

}
