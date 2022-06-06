#include "MainForm.h"

[System::STAThread]
int main()
{
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	System::Windows::Forms::Application::Run(gcnew ColdMars::MainForm());
	return 0;
}