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

		for (int i = 0; i < 10; ++i)
		{
			m_vLabel[i] = (gcnew System::Windows::Forms::Label());
			this->panelLabels->Controls->Add(m_vLabel[i]);
		}

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

		this->pictureBox1->Refresh();
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

	System::Void OpenGLControl::pictureBox1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		std::vector<lib::iPoint3D> vPalette;

		int nPaletteMin;
		int nPaletteMax;

		m_pBridge->getPalette(vPalette, nPaletteMin, nPaletteMax);

		for (int y = 0; y < this->panelLabels->Size.Height; ++y)
		{
			int nPaletteIndex = int(1.0 * y * vPalette.size() / this->panelLabels->Size.Height);
			lib::iPoint3D vColor = vPalette[nPaletteIndex];
			System::Drawing::Color penColor = System::Drawing::Color::FromArgb((int)vColor.r, (int)vColor.g, (int)vColor.b);
			e->Graphics->DrawLine(gcnew System::Drawing::Pen(penColor, 1.0f), System::Drawing::Point(0, y), System::Drawing::Point(this->panelLabels->Size.Width, y));
		}

		//---------------------------------------------------------------------------

		const unsigned nLabelCount = std::min(this->panelLabels->Size.Height / 30, 10);

		double fFactor = 1.0 * this->panelLabels->Size.Height / (nLabelCount - 1);

		for (unsigned i = 0; i < nLabelCount; i += 1)
		{
			int nYpos = int(i * fFactor) - 6;
			if (i == 0)
				nYpos += 6;
			else if (i == nLabelCount - 1)
				nYpos -= 7;

			m_vLabel[i]->Location = System::Drawing::Point(0, nYpos);
			m_vLabel[i]->AutoSize = true;
			m_vLabel[i]->Size = System::Drawing::Size(25, 13);
			m_vLabel[i]->TabIndex = i;
			m_vLabel[i]->Text = gcnew System::String(std::to_string(nPaletteMin + i * (nPaletteMax - nPaletteMin) / (nLabelCount - 1)).c_str());
		}

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
