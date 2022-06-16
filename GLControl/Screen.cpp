#include "pch.h"
#include "Screen.h"

namespace GL
{

    Screen::Screen(System::IntPtr pEngine_)
    {
        m_pDataContextEngine = reinterpret_cast<DataContextEngine*>(pEngine_.ToPointer());
        InitializeComponent();
    }
    void Screen::InitializeComponent(void)
    {
        this->SuspendLayout();
        // 
        // SceneViewControl
        // 
        this->Name = L"SceneViewControl";
        this->Size = System::Drawing::Size(200, 200);
        this->Dock = System::Windows::Forms::DockStyle::Fill;
        this->BackColor = System::Drawing::Color::Red;
        this->HandleCreated += gcnew System::EventHandler(this, &Screen::OnHandleCreated);
        this->HandleDestroyed += gcnew System::EventHandler(this, &Screen::OnHandleDestroyed);
        this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Screen::Screen_Paint);
        this->SizeChanged += gcnew System::EventHandler(this, &Screen::Screen_OnSizeChanged);
        this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Screen::ScreenMouseDown);
        this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Screen::ScreenMouseMove);
        this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Screen::ScreenMouseUp);
        this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Screen::ScreenMouseWheel);
        this->DoubleClick += gcnew System::EventHandler(this, &Screen::MouseDoubleClick);
        this->ResumeLayout(false);

    }

    System::Void Screen::OnHandleCreated(System::Object^ /*sender*/, System::EventArgs^ e)
    {
        m_hWnd = (HWND)Handle.ToPointer();

        if (!!m_pDataContextEngine)
        {
            m_handle_destroyed = false;
            m_pDataContextEngine->on_handle_created();
        }

        if (!!this->ParentForm && !m_parent_open)
        {
            m_parent_open = true;
            this->ParentForm->Closing += gcnew System::ComponentModel::CancelEventHandler(this, &Screen::ParentForm_Closing);
        }
    }

    System::Void Screen::OnHandleDestroyed(System::Object^ /*sender*/, System::EventArgs^ e)
    {
        if (!m_pDataContextEngine)
            return;

        m_handle_destroyed = true;
        m_pDataContextEngine->on_handle_destroyed();
    }

    System::Void Screen::Screen_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
    {
        if (!m_pDataContextEngine)
            return;

        m_pDataContextEngine->draw();
    }

    System::Void Screen::Screen_OnSizeChanged(System::Object^ /*sender*/, System::EventArgs^ e)
    {
        if (!m_pDataContextEngine)
            return;

        bool stateChanged = false;
        if (System::Windows::Forms::Form^ parentForm = this->ParentForm)
        {
            System::Windows::Forms::FormWindowState wndState = this->ParentForm->WindowState;

            stateChanged =
                (m_WndState == System::Windows::Forms::FormWindowState::Minimized && wndState != System::Windows::Forms::FormWindowState::Minimized) ||
                (m_WndState != System::Windows::Forms::FormWindowState::Minimized && wndState == System::Windows::Forms::FormWindowState::Minimized);

            m_WndState = wndState;
        }

        if (stateChanged)
            m_pDataContextEngine->on_handle_changed();

        m_pDataContextEngine->draw();
    }

    System::Void Screen::ParentForm_Closing(System::Object^ /*sender*/, System::ComponentModel::CancelEventArgs^ /*e*/)
    {
        if (!!m_pDataContextEngine)
        {
            m_handle_destroyed = true;
            m_pDataContextEngine->on_handle_destroyed();
        }

        if (!!this->ParentForm)
        {
            m_parent_open = false;
            this->ParentForm->Closing -= gcnew System::ComponentModel::CancelEventHandler(this, &Screen::ParentForm_Closing);
        }
    }

    System::Void Screen::ScreenMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        m_pDataContextEngine->on_mouse_scroll(1.0f * e->Delta / 60);

        return System::Void();
    }

    System::Void Screen::MouseDoubleClick(System::Object^ sender, System::EventArgs^ e)
    {
        m_pDataContextEngine->on_mouse_double_click();

        return System::Void();
    }

    System::IntPtr Screen::get_hWnd()
    {
        if (m_WndState == System::Windows::Forms::FormWindowState::Minimized)
            return System::IntPtr::Zero;

        return System::IntPtr(m_hWnd);
    }

    System::Void Screen::ScreenMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        if (e->Button == System::Windows::Forms::MouseButtons::Left)
        {
            m_bMouseLeftDown = true;
            m_bMouseRightDown = false;

            m_nMousePositionX = e->X;
            m_nMousePositionY = e->Y;
        }
        else if (e->Button == System::Windows::Forms::MouseButtons::Right)
        {
            m_bMouseLeftDown = false;
            m_bMouseRightDown = true;
            
            m_nMousePositionX = e->X;
            m_nMousePositionY = e->Y;
        }

        return System::Void();
    }

    System::Void Screen::ScreenMouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        m_bMouseLeftDown = false;
        m_bMouseRightDown = false;

        return System::Void();
    }

    System::Void Screen::ScreenMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
    {
        if (m_bMouseLeftDown)
            m_pDataContextEngine->on_mouse_left_btn_move(m_nMousePositionX - e->X, m_nMousePositionY - e->Y);
        else if (m_bMouseRightDown)
            m_pDataContextEngine->on_mouse_right_btn_move(m_nMousePositionX - e->X, m_nMousePositionY - e->Y);
        else
            return System::Void();

        m_nMousePositionX = e->X;
        m_nMousePositionY = e->Y;
        
        return System::Void();
    }

    System::Windows::Forms::Control^ Screen::ViewControl::get()
    {
        return this;
    }

}