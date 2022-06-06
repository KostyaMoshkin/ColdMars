#pragma once

#include "DataContextEngine.h"
#include "ContextInterface.h"

#include <Windows.h>

namespace GL
{
	ref class Screen : public System::Windows::Forms::UserControl, ContextInterface
	{
		DataContextEngine* m_pDataContextEngine;

		HWND m_hWnd = nullptr;
		System::Windows::Forms::FormWindowState m_WndState = System::Windows::Forms::FormWindowState::Normal;

		bool m_handle_destroyed = false;
		bool m_parent_open = false;

    public:
		Screen(System::IntPtr pEngine_);

	public:
		void InitializeComponent(void);

	protected: System::Void OnHandleCreated(System::Object^ /*sender*/, System::EventArgs^ e);
	protected: System::Void OnHandleDestroyed(System::Object^ /*sender*/, System::EventArgs^ e);

	protected: System::Void Screen_OnSizeChanged(System::Object^, System::EventArgs^ e);

	protected: System::Void ParentForm_Closing(System::Object^, System::ComponentModel::CancelEventArgs^);

	public:
		virtual System::IntPtr get_hWnd();

		virtual property System::Windows::Forms::Control^ ViewControl
		{
			System::Windows::Forms::Control^ get();
		}

	};

}
