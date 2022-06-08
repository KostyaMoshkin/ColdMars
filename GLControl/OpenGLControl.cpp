#include "pch.h"
#include "OpenGLControl.h"

namespace GLControl {

	OpenGLControl::OpenGLControl()
	{
		m_pBridge = new GL::Bridge();

		InitializeComponent();

		m_pBridge->getScreen()->ViewControl->Dock = System::Windows::Forms::DockStyle::Fill;
		this->panelGL->Controls->Add(m_pBridge->getScreen()->ViewControl);

		this->textBoxOrbitQuantity->Text = gcnew System::String(std::to_string(m_nOrbitQuantity).c_str());
	}

	OpenGLControl::~OpenGLControl()
	{
		if (components)
		{
			delete components;
		}

		delete m_pBridge;
	}

	System::Void OpenGLControl::runRender()
	{
		if (!m_pBridge->isInit())
			return System::Void();

		m_pBridge->draw();

		return System::Void();
	}

	System::Void OpenGLControl::OpenGLControl_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		init();

		runRender();
	}

	System::Void OpenGLControl::OpenGLControl_Resize(System::Object^ sender, System::EventArgs^ e)
	{
		runRender();
	}

	System::Void OpenGLControl::trackBarOrbit_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		m_pBridge->setFileRange(this->trackBarOrbit->Value, this->trackBarOrbit->Value + m_nOrbitQuantity);

		this->textBoxOrbitStart->Text = gcnew System::String(m_pBridge->getOrbit(this->trackBarOrbit->Value).c_str());
		this->textBoxOrbitEnd->Text = gcnew System::String(m_pBridge->getOrbit(this->trackBarOrbit->Value + m_nOrbitQuantity).c_str());

		this->labelSpectrumNumb->Text = gcnew System::String(std::to_string(m_pBridge->getSpectrumNumb()).c_str());
		this->labelIterferogramID->Text = gcnew System::String(std::to_string(m_pBridge->getInterferogramID()).c_str());
		this->labelJulianDate->Text = gcnew System::String(std::to_string(m_pBridge->getJulianDate()).c_str());
		this->labelLocalTime->Text = gcnew System::String(std::to_string(m_pBridge->getLocalTime()).c_str());
		this->labelUTC->Text = gcnew System::String(m_pBridge->getUTC().c_str());

		return System::Void();
	}

	System::Void OpenGLControl::textBoxOrbitQuantity_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		m_nOrbitQuantity = System::Int32::Parse(this->textBoxOrbitQuantity->Text);
		this->trackBarOrbit->Maximum = m_pBridge->getFileCount() - m_nOrbitQuantity - 1;

		return System::Void();
	}

	System::Void OpenGLControl::buttonSetOrbit_Click(System::Object^ sender, System::EventArgs^ e)
	{
		m_pBridge->setFileRange(this->trackBarOrbit->Value, this->trackBarOrbit->Value + m_nOrbitQuantity);

		return System::Void();
	}

	void OpenGLControl::init()
	{
		if (m_bBridgeInit)
			return;

		this->trackBarOrbit->Maximum = m_pBridge->getFileCount() - m_nOrbitQuantity - 1;

		m_bBridgeInit = true;
	}

}
