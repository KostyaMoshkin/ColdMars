#include "MainForm.h"


namespace ColdMars {

	MainForm::MainForm(void)
	{
		this->openGLControl1 = (gcnew GLControl::OpenGLControl());

		InitializeComponent();

		// 
		// openGLControl1
		// 
		//this->openGLControl1->Dock = System::Windows::Forms::DockStyle::Fill;
		//this->openGLControl1->Location = System::Drawing::Point(0, 0);
		//this->openGLControl1->Name = L"openGLControl1";
		//this->openGLControl1->Size = System::Drawing::Size(1256, 923);
		//this->openGLControl1->TabIndex = 2;

		this->ClientSize = System::Drawing::Size(1381, 908);
		//this->Controls->Add(this->openGLControl1);
	}

	MainForm::~MainForm()
	{
		if (components)
		{
			delete components;
		}
	}


}