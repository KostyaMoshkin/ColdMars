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
		bool m_bBridgeInit = false;

		unsigned m_nOrbitCurrentIndex = 0;
		unsigned m_nOrbitQuantity = 2;

		double m_fLocalTimeStart = 0.0f;
		double m_fLocalTimeEnd   = 24.0f;

		int m_nOrbitEndIndex;

		unsigned m_nScale = 500;

		int m_nTimerSpeed = 0;

		bool m_bMouseMove = false;

		static cli::array<System::Windows::Forms::Label^>^ m_vLabel = gcnew cli::array<System::Windows::Forms::Label^>(11);

	private: System::Windows::Forms::Button^ buttonSetOrbit;
	private: System::Windows::Forms::Panel^ panelInfo;

	private: System::Windows::Forms::Label^ labelJulianDate;

	private: System::Windows::Forms::Label^ label6;

	private: System::Windows::Forms::Label^ labelUTC;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Panel^ panelPalette;

	private: System::Windows::Forms::TextBox^ textBoxLsEnd;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Panel^ panelOrbiMovie;
	private: System::Windows::Forms::Button^ buttonFwdPlay;
	private: System::Windows::Forms::Button^ buttonFwd;
	private: System::Windows::Forms::Button^ buttonRew;
	private: System::Windows::Forms::Button^ buttonRewPlay;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::Button^ buttonScalePlus;
	private: System::Windows::Forms::Button^ buttonScaleMinus;
	private: System::Windows::Forms::TextBox^ textBoxScale;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::CheckBox^ checkBoxAtmosphere;
	private: System::Windows::Forms::Panel^ panelObitList;
	private: System::Windows::Forms::Panel^ panelLabels;

	private: System::Windows::Forms::Button^ buttonSearchOrbits;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TextBox^ textLatitude;
	private: System::Windows::Forms::TextBox^ textLongitude;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::CheckedListBox^ checkedListOrbit;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::RadioButton^ radioButtonIce;
	private: System::Windows::Forms::RadioButton^ radioButtonDust;
	private: System::Windows::Forms::RadioButton^ radioButtonTemperature;
	private: System::Windows::Forms::Button^ buttonFindOrbitByLS;
	private: System::Windows::Forms::Label^ labelLS;
	private: System::Windows::Forms::Label^ label13;
		   //private: System::Windows::Forms::Button^ buttonAddOrbit;
	private: System::Windows::Forms::TextBox^ textBoxLocalTimeStart;	
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::RadioButton^ radioButtonLocalTime;
	private: System::Windows::Forms::RadioButton^ radioButtonAngle;
		   //private: System::Windows::Forms::TextBox^ textBox2;
	//private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::TextBox^ textBoxLocalTimeEnd;
	private: System::Windows::Forms::Label^ labelAtmosphereLimit;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::TextBox^ textBoxLsStart;
	private: System::Windows::Forms::Label^ label12;

	public:
		OpenGLControl();

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~OpenGLControl();

	private: System::Windows::Forms::Panel^ panelControl;
	private: System::Windows::Forms::Panel^ panelSeparator;
	private: System::Windows::Forms::Panel^ panelGL;
	private: System::Windows::Forms::Panel^ panelOrbit;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ textBoxOrbitQuantity;
	private: System::Windows::Forms::TextBox^ textBoxOrbitEnd;
	private: System::Windows::Forms::TextBox^ textBoxOrbitStart;
	private: System::Windows::Forms::Label^ labelOrbit;
	private: System::Windows::Forms::TrackBar^ trackBarOrbit;
	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->panelControl = (gcnew System::Windows::Forms::Panel());
			this->panelObitList = (gcnew System::Windows::Forms::Panel());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->buttonSearchOrbits = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->textLatitude = (gcnew System::Windows::Forms::TextBox());
			this->textLongitude = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->checkedListOrbit = (gcnew System::Windows::Forms::CheckedListBox());
			this->panelInfo = (gcnew System::Windows::Forms::Panel());
			this->labelLS = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->labelAtmosphereLimit = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->labelUTC = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->labelJulianDate = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->panelOrbit = (gcnew System::Windows::Forms::Panel());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonLocalTime = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonAngle = (gcnew System::Windows::Forms::RadioButton());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->textBoxLsStart = (gcnew System::Windows::Forms::TextBox());
			this->textBoxLocalTimeEnd = (gcnew System::Windows::Forms::TextBox());
			this->textBoxLocalTimeStart = (gcnew System::Windows::Forms::TextBox());
			this->buttonFindOrbitByLS = (gcnew System::Windows::Forms::Button());
			this->textBoxLsEnd = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonIce = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonDust = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonTemperature = (gcnew System::Windows::Forms::RadioButton());
			this->checkBoxAtmosphere = (gcnew System::Windows::Forms::CheckBox());
			this->buttonScalePlus = (gcnew System::Windows::Forms::Button());
			this->buttonScaleMinus = (gcnew System::Windows::Forms::Button());
			this->textBoxScale = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->panelOrbiMovie = (gcnew System::Windows::Forms::Panel());
			this->buttonFwdPlay = (gcnew System::Windows::Forms::Button());
			this->buttonFwd = (gcnew System::Windows::Forms::Button());
			this->buttonRew = (gcnew System::Windows::Forms::Button());
			this->buttonRewPlay = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->buttonSetOrbit = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxOrbitQuantity = (gcnew System::Windows::Forms::TextBox());
			this->textBoxOrbitEnd = (gcnew System::Windows::Forms::TextBox());
			this->textBoxOrbitStart = (gcnew System::Windows::Forms::TextBox());
			this->labelOrbit = (gcnew System::Windows::Forms::Label());
			this->trackBarOrbit = (gcnew System::Windows::Forms::TrackBar());
			this->panelSeparator = (gcnew System::Windows::Forms::Panel());
			this->panelGL = (gcnew System::Windows::Forms::Panel());
			this->panelPalette = (gcnew System::Windows::Forms::Panel());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->panelLabels = (gcnew System::Windows::Forms::Panel());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panelControl->SuspendLayout();
			this->panelObitList->SuspendLayout();
			this->panelInfo->SuspendLayout();
			this->panelOrbit->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->panelOrbiMovie->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarOrbit))->BeginInit();
			this->panelPalette->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// panelControl
			// 
			this->panelControl->Controls->Add(this->panelObitList);
			this->panelControl->Controls->Add(this->panelInfo);
			this->panelControl->Controls->Add(this->panelOrbit);
			this->panelControl->Controls->Add(this->trackBarOrbit);
			this->panelControl->Dock = System::Windows::Forms::DockStyle::Right;
			this->panelControl->Location = System::Drawing::Point(607, 0);
			this->panelControl->Name = L"panelControl";
			this->panelControl->Size = System::Drawing::Size(342, 692);
			this->panelControl->TabIndex = 0;
			// 
			// panelObitList
			// 
			this->panelObitList->Controls->Add(this->label10);
			this->panelObitList->Controls->Add(this->label9);
			this->panelObitList->Controls->Add(this->buttonSearchOrbits);
			this->panelObitList->Controls->Add(this->label7);
			this->panelObitList->Controls->Add(this->textLatitude);
			this->panelObitList->Controls->Add(this->textLongitude);
			this->panelObitList->Controls->Add(this->label4);
			this->panelObitList->Controls->Add(this->checkedListOrbit);
			this->panelObitList->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelObitList->Location = System::Drawing::Point(0, 457);
			this->panelObitList->Name = L"panelObitList";
			this->panelObitList->Size = System::Drawing::Size(297, 235);
			this->panelObitList->TabIndex = 3;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label10->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->label10->Location = System::Drawing::Point(202, 28);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(45, 13);
			this->label10->TabIndex = 7;
			this->label10->Text = L"Latitude";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label9->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->label9->Location = System::Drawing::Point(133, 28);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(54, 13);
			this->label9->TabIndex = 6;
			this->label9->Text = L"Longitude";
			// 
			// buttonSearchOrbits
			// 
			this->buttonSearchOrbits->Location = System::Drawing::Point(136, 70);
			this->buttonSearchOrbits->Name = L"buttonSearchOrbits";
			this->buttonSearchOrbits->Size = System::Drawing::Size(124, 20);
			this->buttonSearchOrbits->TabIndex = 5;
			this->buttonSearchOrbits->Text = L"Search";
			this->buttonSearchOrbits->UseVisualStyleBackColor = true;
			this->buttonSearchOrbits->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonSearchOrbits_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(194, 44);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(10, 13);
			this->label7->TabIndex = 4;
			this->label7->Text = L":";
			// 
			// textLatitude
			// 
			this->textLatitude->Location = System::Drawing::Point(205, 44);
			this->textLatitude->Name = L"textLatitude";
			this->textLatitude->Size = System::Drawing::Size(55, 20);
			this->textLatitude->TabIndex = 3;
			this->textLatitude->Text = L"20";
			// 
			// textLongitude
			// 
			this->textLongitude->Location = System::Drawing::Point(136, 44);
			this->textLongitude->Name = L"textLongitude";
			this->textLongitude->Size = System::Drawing::Size(55, 20);
			this->textLongitude->TabIndex = 2;
			this->textLongitude->Text = L"-132";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(133, 12);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(134, 13);
			this->label4->TabIndex = 1;
			this->label4->Text = L"Coordinates on the surface";
			// 
			// checkedListOrbit
			// 
			this->checkedListOrbit->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkedListOrbit->CheckOnClick = true;
			this->checkedListOrbit->FormattingEnabled = true;
			this->checkedListOrbit->Location = System::Drawing::Point(16, 12);
			this->checkedListOrbit->Name = L"checkedListOrbit";
			this->checkedListOrbit->Size = System::Drawing::Size(104, 184);
			this->checkedListOrbit->TabIndex = 0;
			this->checkedListOrbit->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &OpenGLControl::checkedListOrbit_CheckedChanged);
			// 
			// panelInfo
			// 
			this->panelInfo->Controls->Add(this->labelLS);
			this->panelInfo->Controls->Add(this->label13);
			this->panelInfo->Controls->Add(this->labelAtmosphereLimit);
			this->panelInfo->Controls->Add(this->label8);
			this->panelInfo->Controls->Add(this->labelUTC);
			this->panelInfo->Controls->Add(this->label3);
			this->panelInfo->Controls->Add(this->labelJulianDate);
			this->panelInfo->Controls->Add(this->label6);
			this->panelInfo->Dock = System::Windows::Forms::DockStyle::Top;
			this->panelInfo->Location = System::Drawing::Point(0, 358);
			this->panelInfo->Name = L"panelInfo";
			this->panelInfo->Size = System::Drawing::Size(297, 99);
			this->panelInfo->TabIndex = 2;
			// 
			// labelLS
			// 
			this->labelLS->AutoSize = true;
			this->labelLS->Location = System::Drawing::Point(101, 73);
			this->labelLS->Name = L"labelLS";
			this->labelLS->Size = System::Drawing::Size(41, 13);
			this->labelLS->TabIndex = 13;
			this->labelLS->Text = L"label14";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(73, 73);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(20, 13);
			this->label13->TabIndex = 12;
			this->label13->Text = L"LS";
			// 
			// labelAtmosphereLimit
			// 
			this->labelAtmosphereLimit->AutoSize = true;
			this->labelAtmosphereLimit->Location = System::Drawing::Point(141, 10);
			this->labelAtmosphereLimit->Name = L"labelAtmosphereLimit";
			this->labelAtmosphereLimit->Size = System::Drawing::Size(41, 13);
			this->labelAtmosphereLimit->TabIndex = 11;
			this->labelAtmosphereLimit->Text = L"label11";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(35, 10);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(103, 13);
			this->label8->TabIndex = 10;
			this->label8->Text = L"Atmosphere limit, km";
			// 
			// labelUTC
			// 
			this->labelUTC->AutoSize = true;
			this->labelUTC->Location = System::Drawing::Point(101, 49);
			this->labelUTC->Name = L"labelUTC";
			this->labelUTC->Size = System::Drawing::Size(35, 13);
			this->labelUTC->TabIndex = 9;
			this->labelUTC->Text = L"label5";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(64, 49);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(29, 13);
			this->label3->TabIndex = 8;
			this->label3->Text = L"UTC";
			// 
			// labelJulianDate
			// 
			this->labelJulianDate->AutoSize = true;
			this->labelJulianDate->Location = System::Drawing::Point(101, 27);
			this->labelJulianDate->Name = L"labelJulianDate";
			this->labelJulianDate->Size = System::Drawing::Size(35, 13);
			this->labelJulianDate->TabIndex = 5;
			this->labelJulianDate->Text = L"label7";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(35, 27);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(58, 13);
			this->label6->TabIndex = 4;
			this->label6->Text = L"Julian date";
			// 
			// panelOrbit
			// 
			this->panelOrbit->Controls->Add(this->groupBox2);
			this->panelOrbit->Controls->Add(this->label12);
			this->panelOrbit->Controls->Add(this->label11);
			this->panelOrbit->Controls->Add(this->textBoxLsStart);
			this->panelOrbit->Controls->Add(this->textBoxLocalTimeEnd);
			this->panelOrbit->Controls->Add(this->textBoxLocalTimeStart);
			this->panelOrbit->Controls->Add(this->buttonFindOrbitByLS);
			this->panelOrbit->Controls->Add(this->textBoxLsEnd);
			this->panelOrbit->Controls->Add(this->groupBox1);
			this->panelOrbit->Controls->Add(this->checkBoxAtmosphere);
			this->panelOrbit->Controls->Add(this->buttonScalePlus);
			this->panelOrbit->Controls->Add(this->buttonScaleMinus);
			this->panelOrbit->Controls->Add(this->textBoxScale);
			this->panelOrbit->Controls->Add(this->label2);
			this->panelOrbit->Controls->Add(this->panelOrbiMovie);
			this->panelOrbit->Controls->Add(this->label5);
			this->panelOrbit->Controls->Add(this->buttonSetOrbit);
			this->panelOrbit->Controls->Add(this->label1);
			this->panelOrbit->Controls->Add(this->textBoxOrbitQuantity);
			this->panelOrbit->Controls->Add(this->textBoxOrbitEnd);
			this->panelOrbit->Controls->Add(this->textBoxOrbitStart);
			this->panelOrbit->Controls->Add(this->labelOrbit);
			this->panelOrbit->Dock = System::Windows::Forms::DockStyle::Top;
			this->panelOrbit->Location = System::Drawing::Point(0, 0);
			this->panelOrbit->Name = L"panelOrbit";
			this->panelOrbit->Size = System::Drawing::Size(297, 358);
			this->panelOrbit->TabIndex = 1;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->radioButtonLocalTime);
			this->groupBox2->Controls->Add(this->radioButtonAngle);
			this->groupBox2->Location = System::Drawing::Point(110, 314);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(170, 38);
			this->groupBox2->TabIndex = 23;
			this->groupBox2->TabStop = false;
			// 
			// radioButtonLocalTime
			// 
			this->radioButtonLocalTime->AutoSize = true;
			this->radioButtonLocalTime->Location = System::Drawing::Point(83, 15);
			this->radioButtonLocalTime->Name = L"radioButtonLocalTime";
			this->radioButtonLocalTime->Size = System::Drawing::Size(74, 17);
			this->radioButtonLocalTime->TabIndex = 1;
			this->radioButtonLocalTime->Text = L"LocalTime";
			this->radioButtonLocalTime->UseVisualStyleBackColor = true;
			this->radioButtonLocalTime->CheckedChanged += gcnew System::EventHandler(this, &OpenGLControl::radioButtonLocalTime_CheckedChanged);
			// 
			// radioButtonAngle
			// 
			this->radioButtonAngle->AutoSize = true;
			this->radioButtonAngle->Checked = true;
			this->radioButtonAngle->Location = System::Drawing::Point(6, 15);
			this->radioButtonAngle->Name = L"radioButtonAngle";
			this->radioButtonAngle->Size = System::Drawing::Size(72, 17);
			this->radioButtonAngle->TabIndex = 0;
			this->radioButtonAngle->TabStop = true;
			this->radioButtonAngle->Text = L"Longitude";
			this->radioButtonAngle->UseVisualStyleBackColor = true;
			this->radioButtonAngle->CheckedChanged += gcnew System::EventHandler(this, &OpenGLControl::radioButtonAngle_CheckedChanged);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(224, 151);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(10, 13);
			this->label12->TabIndex = 22;
			this->label12->Text = L"-";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(179, 132);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(88, 13);
			this->label11->TabIndex = 21;
			this->label11->Text = L"Local time range:";
			// 
			// textBoxLsStart
			// 
			this->textBoxLsStart->Location = System::Drawing::Point(181, 25);
			this->textBoxLsStart->Name = L"textBoxLsStart";
			this->textBoxLsStart->Size = System::Drawing::Size(100, 20);
			this->textBoxLsStart->TabIndex = 20;
			// 
			// textBoxLocalTimeEnd
			// 
			this->textBoxLocalTimeEnd->Location = System::Drawing::Point(236, 148);
			this->textBoxLocalTimeEnd->Name = L"textBoxLocalTimeEnd";
			this->textBoxLocalTimeEnd->Size = System::Drawing::Size(42, 20);
			this->textBoxLocalTimeEnd->TabIndex = 19;
			// 
			// textBoxLocalTimeStart
			// 
			this->textBoxLocalTimeStart->Location = System::Drawing::Point(182, 148);
			this->textBoxLocalTimeStart->Name = L"textBoxLocalTimeStart";
			this->textBoxLocalTimeStart->Size = System::Drawing::Size(40, 20);
			this->textBoxLocalTimeStart->TabIndex = 18;
			// 
			// buttonFindOrbitByLS
			// 
			this->buttonFindOrbitByLS->Location = System::Drawing::Point(182, 77);
			this->buttonFindOrbitByLS->Name = L"buttonFindOrbitByLS";
			this->buttonFindOrbitByLS->Size = System::Drawing::Size(99, 21);
			this->buttonFindOrbitByLS->TabIndex = 17;
			this->buttonFindOrbitByLS->Text = L"<< find orbits";
			this->buttonFindOrbitByLS->UseVisualStyleBackColor = true;
			this->buttonFindOrbitByLS->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonFindOrbitByLS_Click);
			// 
			// textBoxLsEnd
			// 
			this->textBoxLsEnd->Location = System::Drawing::Point(181, 51);
			this->textBoxLsEnd->Name = L"textBoxLsEnd";
			this->textBoxLsEnd->Size = System::Drawing::Size(100, 20);
			this->textBoxLsEnd->TabIndex = 16;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radioButtonIce);
			this->groupBox1->Controls->Add(this->radioButtonDust);
			this->groupBox1->Controls->Add(this->radioButtonTemperature);
			this->groupBox1->Location = System::Drawing::Point(10, 275);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(270, 38);
			this->groupBox1->TabIndex = 15;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Display";
			// 
			// radioButtonIce
			// 
			this->radioButtonIce->AutoSize = true;
			this->radioButtonIce->Location = System::Drawing::Point(196, 15);
			this->radioButtonIce->Name = L"radioButtonIce";
			this->radioButtonIce->Size = System::Drawing::Size(40, 17);
			this->radioButtonIce->TabIndex = 2;
			this->radioButtonIce->Text = L"Ice";
			this->radioButtonIce->UseVisualStyleBackColor = true;
			this->radioButtonIce->CheckedChanged += gcnew System::EventHandler(this, &OpenGLControl::radioButtonIce_CheckedChanged);
			// 
			// radioButtonDust
			// 
			this->radioButtonDust->AutoSize = true;
			this->radioButtonDust->Location = System::Drawing::Point(124, 15);
			this->radioButtonDust->Name = L"radioButtonDust";
			this->radioButtonDust->Size = System::Drawing::Size(47, 17);
			this->radioButtonDust->TabIndex = 1;
			this->radioButtonDust->Text = L"Dust";
			this->radioButtonDust->UseVisualStyleBackColor = true;
			this->radioButtonDust->CheckedChanged += gcnew System::EventHandler(this, &OpenGLControl::radioButtonDust_CheckedChanged);
			// 
			// radioButtonTemperature
			// 
			this->radioButtonTemperature->AutoSize = true;
			this->radioButtonTemperature->Checked = true;
			this->radioButtonTemperature->Location = System::Drawing::Point(12, 15);
			this->radioButtonTemperature->Name = L"radioButtonTemperature";
			this->radioButtonTemperature->Size = System::Drawing::Size(85, 17);
			this->radioButtonTemperature->TabIndex = 0;
			this->radioButtonTemperature->TabStop = true;
			this->radioButtonTemperature->Text = L"Temperature";
			this->radioButtonTemperature->UseVisualStyleBackColor = true;
			this->radioButtonTemperature->CheckedChanged += gcnew System::EventHandler(this, &OpenGLControl::radioButtonTemperature_CheckedChanged);
			// 
			// checkBoxAtmosphere
			// 
			this->checkBoxAtmosphere->AutoSize = true;
			this->checkBoxAtmosphere->Checked = true;
			this->checkBoxAtmosphere->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBoxAtmosphere->Location = System::Drawing::Point(22, 326);
			this->checkBoxAtmosphere->Name = L"checkBoxAtmosphere";
			this->checkBoxAtmosphere->Size = System::Drawing::Size(82, 17);
			this->checkBoxAtmosphere->TabIndex = 13;
			this->checkBoxAtmosphere->Text = L"Atmosphere";
			this->checkBoxAtmosphere->UseVisualStyleBackColor = true;
			this->checkBoxAtmosphere->CheckedChanged += gcnew System::EventHandler(this, &OpenGLControl::checkBoxAtmosphere_CheckedChanged);
			// 
			// buttonScalePlus
			// 
			this->buttonScalePlus->Location = System::Drawing::Point(202, 196);
			this->buttonScalePlus->Name = L"buttonScalePlus";
			this->buttonScalePlus->Size = System::Drawing::Size(20, 20);
			this->buttonScalePlus->TabIndex = 12;
			this->buttonScalePlus->Text = L"+";
			this->buttonScalePlus->UseVisualStyleBackColor = true;
			this->buttonScalePlus->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonScalePlus_Click);
			// 
			// buttonScaleMinus
			// 
			this->buttonScaleMinus->Location = System::Drawing::Point(180, 196);
			this->buttonScaleMinus->Name = L"buttonScaleMinus";
			this->buttonScaleMinus->Size = System::Drawing::Size(20, 20);
			this->buttonScaleMinus->TabIndex = 11;
			this->buttonScaleMinus->Text = L"-";
			this->buttonScaleMinus->UseVisualStyleBackColor = true;
			this->buttonScaleMinus->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonScaleMinus_Click);
			// 
			// textBoxScale
			// 
			this->textBoxScale->Location = System::Drawing::Point(59, 196);
			this->textBoxScale->Name = L"textBoxScale";
			this->textBoxScale->Size = System::Drawing::Size(100, 20);
			this->textBoxScale->TabIndex = 10;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(18, 203);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(34, 13);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Scale";
			// 
			// panelOrbiMovie
			// 
			this->panelOrbiMovie->Controls->Add(this->buttonFwdPlay);
			this->panelOrbiMovie->Controls->Add(this->buttonFwd);
			this->panelOrbiMovie->Controls->Add(this->buttonRew);
			this->panelOrbiMovie->Controls->Add(this->buttonRewPlay);
			this->panelOrbiMovie->Location = System::Drawing::Point(13, 132);
			this->panelOrbiMovie->Name = L"panelOrbiMovie";
			this->panelOrbiMovie->Size = System::Drawing::Size(160, 50);
			this->panelOrbiMovie->TabIndex = 8;
			// 
			// buttonFwdPlay
			// 
			this->buttonFwdPlay->Location = System::Drawing::Point(120, 0);
			this->buttonFwdPlay->Name = L"buttonFwdPlay";
			this->buttonFwdPlay->Size = System::Drawing::Size(40, 50);
			this->buttonFwdPlay->TabIndex = 3;
			this->buttonFwdPlay->Text = L">>";
			this->buttonFwdPlay->UseVisualStyleBackColor = true;
			this->buttonFwdPlay->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonFwdPlay_Click);
			// 
			// buttonFwd
			// 
			this->buttonFwd->Location = System::Drawing::Point(80, 0);
			this->buttonFwd->Name = L"buttonFwd";
			this->buttonFwd->Size = System::Drawing::Size(40, 50);
			this->buttonFwd->TabIndex = 2;
			this->buttonFwd->Text = L">";
			this->buttonFwd->UseVisualStyleBackColor = true;
			this->buttonFwd->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonFwd_Click);
			// 
			// buttonRew
			// 
			this->buttonRew->Location = System::Drawing::Point(40, 0);
			this->buttonRew->Name = L"buttonRew";
			this->buttonRew->Size = System::Drawing::Size(40, 50);
			this->buttonRew->TabIndex = 1;
			this->buttonRew->Text = L"<";
			this->buttonRew->UseVisualStyleBackColor = true;
			this->buttonRew->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonRew_Click);
			// 
			// buttonRewPlay
			// 
			this->buttonRewPlay->Location = System::Drawing::Point(0, 0);
			this->buttonRewPlay->Name = L"buttonRewPlay";
			this->buttonRewPlay->Size = System::Drawing::Size(40, 50);
			this->buttonRewPlay->TabIndex = 0;
			this->buttonRewPlay->Text = L"<<";
			this->buttonRewPlay->UseVisualStyleBackColor = true;
			this->buttonRewPlay->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonRewPlay_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(179, 7);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(45, 13);
			this->label5->TabIndex = 6;
			this->label5->Text = L"Orbit LS";
			// 
			// buttonSetOrbit
			// 
			this->buttonSetOrbit->Location = System::Drawing::Point(178, 233);
			this->buttonSetOrbit->Name = L"buttonSetOrbit";
			this->buttonSetOrbit->Size = System::Drawing::Size(100, 23);
			this->buttonSetOrbit->TabIndex = 5;
			this->buttonSetOrbit->Text = L"OK";
			this->buttonSetOrbit->UseVisualStyleBackColor = true;
			this->buttonSetOrbit->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonSetOrbit_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(21, 77);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(74, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Orbits quantity";
			// 
			// textBoxOrbitQuantity
			// 
			this->textBoxOrbitQuantity->Location = System::Drawing::Point(24, 93);
			this->textBoxOrbitQuantity->Name = L"textBoxOrbitQuantity";
			this->textBoxOrbitQuantity->Size = System::Drawing::Size(100, 20);
			this->textBoxOrbitQuantity->TabIndex = 3;
			this->textBoxOrbitQuantity->Text = L"3";
			// 
			// textBoxOrbitEnd
			// 
			this->textBoxOrbitEnd->Location = System::Drawing::Point(21, 51);
			this->textBoxOrbitEnd->Name = L"textBoxOrbitEnd";
			this->textBoxOrbitEnd->Size = System::Drawing::Size(100, 20);
			this->textBoxOrbitEnd->TabIndex = 2;
			// 
			// textBoxOrbitStart
			// 
			this->textBoxOrbitStart->Location = System::Drawing::Point(21, 25);
			this->textBoxOrbitStart->Name = L"textBoxOrbitStart";
			this->textBoxOrbitStart->Size = System::Drawing::Size(100, 20);
			this->textBoxOrbitStart->TabIndex = 1;
			// 
			// labelOrbit
			// 
			this->labelOrbit->AutoSize = true;
			this->labelOrbit->Location = System::Drawing::Point(18, 9);
			this->labelOrbit->Name = L"labelOrbit";
			this->labelOrbit->Size = System::Drawing::Size(78, 13);
			this->labelOrbit->TabIndex = 0;
			this->labelOrbit->Text = L"Orbit number(s)";
			// 
			// trackBarOrbit
			// 
			this->trackBarOrbit->Dock = System::Windows::Forms::DockStyle::Right;
			this->trackBarOrbit->Location = System::Drawing::Point(297, 0);
			this->trackBarOrbit->Name = L"trackBarOrbit";
			this->trackBarOrbit->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->trackBarOrbit->Size = System::Drawing::Size(45, 692);
			this->trackBarOrbit->TabIndex = 0;
			this->trackBarOrbit->TickStyle = System::Windows::Forms::TickStyle::TopLeft;
			this->trackBarOrbit->Scroll += gcnew System::EventHandler(this, &OpenGLControl::trackBarOrbit_Scroll);
			// 
			// panelSeparator
			// 
			this->panelSeparator->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->panelSeparator->Dock = System::Windows::Forms::DockStyle::Right;
			this->panelSeparator->Location = System::Drawing::Point(604, 0);
			this->panelSeparator->Name = L"panelSeparator";
			this->panelSeparator->Size = System::Drawing::Size(3, 692);
			this->panelSeparator->TabIndex = 1;
			// 
			// panelGL
			// 
			this->panelGL->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelGL->Location = System::Drawing::Point(0, 0);
			this->panelGL->Name = L"panelGL";
			this->panelGL->Size = System::Drawing::Size(529, 692);
			this->panelGL->TabIndex = 2;
			// 
			// panelPalette
			// 
			this->panelPalette->Controls->Add(this->pictureBox1);
			this->panelPalette->Controls->Add(this->panelLabels);
			this->panelPalette->Dock = System::Windows::Forms::DockStyle::Right;
			this->panelPalette->Location = System::Drawing::Point(529, 0);
			this->panelPalette->Name = L"panelPalette";
			this->panelPalette->Size = System::Drawing::Size(75, 692);
			this->panelPalette->TabIndex = 0;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Right;
			this->pictureBox1->Location = System::Drawing::Point(25, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(50, 692);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &OpenGLControl::temperatureScale_Paint);
			// 
			// panelLabels
			// 
			this->panelLabels->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelLabels->Location = System::Drawing::Point(0, 0);
			this->panelLabels->Name = L"panelLabels";
			this->panelLabels->Size = System::Drawing::Size(75, 692);
			this->panelLabels->TabIndex = 1;
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &OpenGLControl::timer1_Tick);
			// 
			// OpenGLControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->panelGL);
			this->Controls->Add(this->panelPalette);
			this->Controls->Add(this->panelSeparator);
			this->Controls->Add(this->panelControl);
			this->Location = System::Drawing::Point(181, 26);
			this->Name = L"OpenGLControl";
			this->Size = System::Drawing::Size(949, 692);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &OpenGLControl::OpenGLControl_Paint);
			this->Resize += gcnew System::EventHandler(this, &OpenGLControl::OpenGLControl_Resize);
			this->panelControl->ResumeLayout(false);
			this->panelControl->PerformLayout();
			this->panelObitList->ResumeLayout(false);
			this->panelObitList->PerformLayout();
			this->panelInfo->ResumeLayout(false);
			this->panelInfo->PerformLayout();
			this->panelOrbit->ResumeLayout(false);
			this->panelOrbit->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->panelOrbiMovie->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarOrbit))->EndInit();
			this->panelPalette->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: 

		System::Void OpenGLControl_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);

		System::Void OpenGLControl_Resize(System::Object^ sender, System::EventArgs^ e);

		System::Void trackBarOrbit_Scroll(System::Object^ sender, System::EventArgs^ e);

		System::Void updateOrbitInfo(unsigned nIndex_, bool bRebiuldList_);

		System::Void buttonSetOrbit_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void temperatureScale_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);

		System::Void buttonRewPlay_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void buttonRew_Click(System::Object^ sender, System::EventArgs^ e);
		bool moveBackward();
		System::Void buttonFwd_Click(System::Object^ sender, System::EventArgs^ e);
		bool moveForward();
		System::Void buttonFwdPlay_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e);

		System::Void buttonScaleMinus_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void buttonScalePlus_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void checkBoxAtmosphere_CheckedChanged(System::Object^ sender, System::EventArgs^ e);

		System::Void buttonAddOrbit_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void buttonSearchOrbits_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void checkedListOrbit_CheckedChanged(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e);

		System::Void OpenGLControl::ScreenMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void OpenGLControl::ScreenMouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void OpenGLControl::ScreenMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

		System::Void buttonFindOrbitByLS_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void radioButtonTemperature_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void radioButtonDust_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void radioButtonIce_CheckedChanged(System::Object^ sender, System::EventArgs^ e);

		System::Void radioButtonAngle_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void radioButtonLocalTime_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
	private:
		void init();

		void setLocalTimeFilter();
};
}
