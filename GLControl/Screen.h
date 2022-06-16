#pragma once

#include "DataContextEngine.h"
#include "ContextInterface.h"

namespace GL
{
	ref class Screen : public System::Windows::Forms::UserControl, ContextInterface
	{
		DataContextEngine* m_pDataContextEngine;

		HWND m_hWnd = nullptr;
		System::Windows::Forms::FormWindowState m_WndState = System::Windows::Forms::FormWindowState::Normal;

		bool m_handle_destroyed = false;
		bool m_parent_open = false;

		int m_nMousePositionX = 0;
		int m_nMousePositionY = 0;

		bool m_bMouseLeftDown = false;
		bool m_bMouseRightDown = false;

    public:
		Screen(System::IntPtr pEngine_);

	public:
		void InitializeComponent(void);

		virtual System::IntPtr get_hWnd();

		virtual property System::Windows::Forms::Control^ ViewControl
		{
			System::Windows::Forms::Control^ get();
		}

	protected: 
		System::Void OnHandleCreated(System::Object^ /*sender*/, System::EventArgs^ e);
		System::Void OnHandleDestroyed(System::Object^ /*sender*/, System::EventArgs^ e);
		System::Void Screen_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
		System::Void Screen_OnSizeChanged(System::Object^, System::EventArgs^ e);
		System::Void ParentForm_Closing(System::Object^, System::ComponentModel::CancelEventArgs^);

	private: 
		System::Void ScreenMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void ScreenMouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void ScreenMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void ScreenMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void MouseDoubleClick(System::Object^ sender, System::EventArgs^ e);

	};

}
