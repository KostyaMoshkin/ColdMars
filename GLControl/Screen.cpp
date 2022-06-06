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
        this->SizeChanged += gcnew System::EventHandler(this, &Screen::Screen_OnSizeChanged);
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

        //System::Windows::Forms::UserControl::OnHandleCreated(e);
    }

    System::Void Screen::OnHandleDestroyed(System::Object^ /*sender*/, System::EventArgs^ e)
    {
        if (!!m_pDataContextEngine)
        {
            m_handle_destroyed = true;
            m_pDataContextEngine->on_handle_destroyed();
        }

        //System::Windows::Forms::UserControl::OnHandleDestroyed(e);
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
        {
            m_pDataContextEngine->on_handle_changed();
            m_pDataContextEngine->draw();
        }
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

    System::IntPtr Screen::get_hWnd()
    {
        if (m_WndState == System::Windows::Forms::FormWindowState::Minimized)
            return System::IntPtr::Zero;

        return System::IntPtr(m_hWnd);
    }

    System::Windows::Forms::Control^ Screen::ViewControl::get()
    {
        return this;
    }

}