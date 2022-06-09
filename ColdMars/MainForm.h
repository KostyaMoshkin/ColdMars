#pragma once

namespace ColdMars {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void);

	protected:
		~MainForm();

	private: GLControl::OpenGLControl^ openGLControl1;

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
			//this->openGLControl1 = (gcnew GLControl::OpenGLControl());
			this->SuspendLayout();
			// 
			// openGLControl1
			// 
			this->openGLControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->openGLControl1->Location = System::Drawing::Point(0, 0);
			this->openGLControl1->Name = L"openGLControl1";
			this->openGLControl1->Size = System::Drawing::Size(1256, 923);
			this->openGLControl1->TabIndex = 2;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1381, 608);
			this->Controls->Add(this->openGLControl1);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
