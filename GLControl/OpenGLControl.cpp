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
		m_nOrbitCurrentIndex = this->trackBarOrbit->Value;

		m_pBridge->setFileRange(this->trackBarOrbit->Value, this->trackBarOrbit->Value + m_nOrbitQuantity);

		updateOrbitInfo();

		return System::Void();
	}

	System::Void OpenGLControl::updateOrbitInfo()
	{
		this->textBoxOrbitStart->Text = gcnew System::String(m_pBridge->getOrbit_by_index(m_nOrbitCurrentIndex).c_str());
		this->textBoxOrbitEnd->Text = gcnew System::String(m_pBridge->getOrbit_by_index(m_nOrbitCurrentIndex + m_nOrbitQuantity).c_str());

		this->labelJulianDate->Text = gcnew System::String(std::to_string(m_pBridge->getJulianDate()).c_str());
		this->labelLocalTime->Text = gcnew System::String(std::to_string(m_pBridge->getLocalTime()).c_str());
		this->labelUTC->Text = gcnew System::String(m_pBridge->getUTC().c_str());
		this->textBoxOrbitQuantity->Text = gcnew System::String(std::to_string(m_nOrbitQuantity).c_str());
		this->textBoxLS->Text = gcnew System::String(std::to_string(m_pBridge->getLS()).c_str());
		this->textBoxScale->Text = gcnew System::String(std::to_string(m_pBridge->getScale()).c_str());

		double fLS;
		if (!System::Double::TryParse(this->textBoxLS->Text, System::Globalization::NumberStyles::AllowDecimalPoint, System::Globalization::CultureInfo::CurrentCulture, fLS) &&
			!System::Double::TryParse(this->textBoxLS->Text, System::Globalization::NumberStyles::AllowDecimalPoint, System::Globalization::CultureInfo::InvariantCulture, fLS)
			)
			fLS = m_nLS;

		unsigned nLS = unsigned(fLS * 100);
		m_nLS = nLS;
	}

	System::Void OpenGLControl::textBoxOrbitQuantity_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		return System::Void();
	}

	System::Void OpenGLControl::buttonSetOrbit_Click(System::Object^ sender, System::EventArgs^ e)
	{
		unsigned nOrbitQuantity = System::Int32::Parse(this->textBoxOrbitQuantity->Text);

		double fLS;
		if (!System::Double::TryParse(this->textBoxLS->Text, System::Globalization::NumberStyles::AllowDecimalPoint, System::Globalization::CultureInfo::CurrentCulture, fLS) &&
			!System::Double::TryParse(this->textBoxLS->Text, System::Globalization::NumberStyles::AllowDecimalPoint, System::Globalization::CultureInfo::InvariantCulture, fLS)
			)
			fLS = m_nLS;

		unsigned nLS = unsigned(fLS * 100);

		double fScale;
		if (!System::Double::TryParse(this->textBoxScale->Text, System::Globalization::NumberStyles::AllowDecimalPoint, System::Globalization::CultureInfo::CurrentCulture, fScale) &&
			!System::Double::TryParse(this->textBoxScale->Text, System::Globalization::NumberStyles::AllowDecimalPoint, System::Globalization::CultureInfo::InvariantCulture, fScale)
			)
			fScale = m_nScale;

		unsigned nScale = unsigned(fScale * 100);

		if (m_nOrbitQuantity != nOrbitQuantity)
		{
			m_nOrbitQuantity = std::min(nOrbitQuantity, m_pBridge->getFileCount() - m_nOrbitCurrentIndex - 1);

		}
		else if (m_nLS != nLS)
		{
			m_nOrbitCurrentIndex = m_pBridge->getOrbit_by_LS(int(fLS * 100));

			m_nLS = nLS;
		}
		else if (m_nScale != nScale)
		{
			m_pBridge->setScale(1.0f * nScale / 100.0f);

			m_nScale = nScale;
		}
		else
		{
			int nOrbitStartNumber = System::Int32::Parse(this->textBoxOrbitStart->Text);
			int nOrbitEndNumber = System::Int32::Parse(this->textBoxOrbitEnd->Text);

			unsigned nOrbitCurrentIndex = m_pBridge->getOrbit_by_number(nOrbitStartNumber);

			if (nOrbitCurrentIndex != UINT_MAX)
			{
				m_nOrbitCurrentIndex = nOrbitCurrentIndex;
				m_nOrbitQuantity = m_pBridge->getOrbit_by_number(nOrbitEndNumber) - m_nOrbitCurrentIndex;
			}
		}

		m_pBridge->setFileRange(m_nOrbitCurrentIndex, m_nOrbitCurrentIndex + m_nOrbitQuantity);

		updateOrbitInfo();

		this->trackBarOrbit->Maximum = m_pBridge->getFileCount() - m_nOrbitQuantity - 1;
		this->trackBarOrbit->Value = m_nOrbitCurrentIndex;

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

	System::Void OpenGLControl::buttonRew_Click(System::Object^ sender, System::EventArgs^ e)
	{
		this->timer1->Stop();
		m_nTimerSpeed = 0;

		moveBackward();

		return System::Void();
	}

	System::Void OpenGLControl::buttonFwd_Click(System::Object^ sender, System::EventArgs^ e)
	{
		this->timer1->Stop();
		m_nTimerSpeed = 0;

		moveForward();

		return System::Void();
	}

	bool OpenGLControl::moveBackward()
	{
		if (m_nOrbitCurrentIndex > 0)
			--m_nOrbitCurrentIndex;
		else
			return false;

		m_pBridge->setFileRange(m_nOrbitCurrentIndex, m_nOrbitCurrentIndex + m_nOrbitQuantity);

		updateOrbitInfo();

		this->trackBarOrbit->Value = m_nOrbitCurrentIndex;

		return true;
	}

	bool OpenGLControl::moveForward()
	{
		if (m_nOrbitCurrentIndex < m_pBridge->getFileCount() - m_nOrbitQuantity - 1)
			++m_nOrbitCurrentIndex;
		else
			return false;

		m_pBridge->setFileRange(m_nOrbitCurrentIndex, m_nOrbitCurrentIndex + m_nOrbitQuantity);

		updateOrbitInfo();

		this->trackBarOrbit->Value = m_nOrbitCurrentIndex;

		return true;
	}

	System::Void OpenGLControl::buttonFwdPlay_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (m_nTimerSpeed == 0)
			m_nTimerSpeed = 1000;
		else if (m_nTimerSpeed < 0)
		{
			m_nTimerSpeed = 0;
			this->timer1->Stop();
			return;
		}
		else
			m_nTimerSpeed = m_nTimerSpeed < 200 ? m_nTimerSpeed : m_nTimerSpeed / 2;

		this->timer1->Interval = std::abs(m_nTimerSpeed);
		this->timer1->Start();

		return System::Void();
	}

	System::Void OpenGLControl::buttonRewPlay_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (m_nTimerSpeed == 0)
			m_nTimerSpeed = -1000;
		else if (m_nTimerSpeed > 0)
		{
			m_nTimerSpeed = 0;
			this->timer1->Stop();
			return;
		}
		else
			m_nTimerSpeed = m_nTimerSpeed > -200 ? m_nTimerSpeed : m_nTimerSpeed / 2;

		this->timer1->Interval = std::abs(m_nTimerSpeed);
		this->timer1->Start();

		return System::Void();
	}

	System::Void OpenGLControl::timer1_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		bool bResult = false;

		if (m_nTimerSpeed > 0)
			bResult = moveForward();
		else if (m_nTimerSpeed < 0)
			bResult = moveBackward();

		if (!bResult)
		{
			this->timer1->Stop();
			m_nTimerSpeed = 0;
		}

		return System::Void();
	}

	System::Void OpenGLControl::buttonScaleMinus_Click(System::Object^ sender, System::EventArgs^ e)
	{
		m_nScale = unsigned(1.0 * m_nScale / 1.2);
		m_pBridge->setScale(1.0f * m_nScale / 100.0f);

		updateOrbitInfo();

		return System::Void();
	}

	System::Void OpenGLControl::buttonScalePlus_Click(System::Object^ sender, System::EventArgs^ e)
	{
		m_nScale = unsigned(1.0 * m_nScale * 1.2);
		m_pBridge->setScale(1.0f * m_nScale / 100.0f);

		updateOrbitInfo();

		return System::Void();
	}

	void OpenGLControl::init()
	{
		if (m_bBridgeInit)
			return;

		this->trackBarOrbit->Maximum = m_pBridge->getFileCount() - m_nOrbitQuantity - 1;

		updateOrbitInfo();

		m_bBridgeInit = true;
	}

}
