#pragma once

#include "Bridge.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace GLControl {

	/// <summary>
	/// —водка дл€ MyUserControl
	/// </summary>
	public ref class OpenGLControl : public System::Windows::Forms::UserControl
	{
		GL::Bridge* m_pBridge;

	public:
		OpenGLControl()
		{
			InitializeComponent();

			m_pBridge = new GL::Bridge();

			this->Controls->Add(m_pBridge->getScreen()->ViewControl);
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~OpenGLControl()
		{
			if (components)
			{
				delete components;
			}

			delete m_pBridge;
		}

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// OpenGLControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Name = L"OpenGLControl";
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &OpenGLControl::OpenGLControl_Paint);
			this->Resize += gcnew System::EventHandler(this, &OpenGLControl::OpenGLControl_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion

	public: event EventHandler^ DrawGL;

	private: 

		System::Void runRender()
		{
			if (!m_pBridge->isInit())
				return System::Void();

			m_pBridge->draw();

			  //m_pBridge->getContext()->begin_draw(this->Size.Width, this->Size.Height);

			  //DrawGL(this, EventArgs::Empty);
			  //m_pBridge->getContext()->end_draw();

			  return System::Void();
		}

		System::Void OpenGLControl_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) 
		{
			runRender();
		}

		System::Void OpenGLControl_Resize(System::Object^ sender, System::EventArgs^ e) 
		{
			runRender();
		}

	};
}
