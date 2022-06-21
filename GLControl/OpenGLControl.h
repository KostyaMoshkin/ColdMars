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
		unsigned m_nLS = 0;

		unsigned m_nScale = 500;

		int m_nTimerSpeed = 0;

		static  cli::array<System::Windows::Forms::Label^>^ m_vLabel = gcnew cli::array<System::Windows::Forms::Label^>(10);

	private: System::Windows::Forms::Button^ buttonSetOrbit;
	private: System::Windows::Forms::Panel^ panelInfo;

	private: System::Windows::Forms::Label^ labelJulianDate;

	private: System::Windows::Forms::Label^ label6;

	private: System::Windows::Forms::Label^ labelLocalTime;
	private: System::Windows::Forms::Label^ labelUTC;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Panel^ panelPalette;
	private: System::Windows::Forms::Panel^ panelLabels;
	private: System::Windows::Forms::TextBox^ textBoxLS;
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
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::RadioButton^ radioButtonSouth;
	private: System::Windows::Forms::RadioButton^ radioButtonNorth;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::RadioButton^ radioButtonEast;
	private: System::Windows::Forms::RadioButton^ radioButtonWest;
	private: System::Windows::Forms::Button^ buttonSearchOrbits;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TextBox^ textLatitude;
	private: System::Windows::Forms::TextBox^ textLongitude;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::CheckedListBox^ checkedListOrbit;

	private: System::Windows::Forms::Button^ buttonAddOrbit;




	private: System::Windows::Forms::Label^ label8;

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
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonSouth = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonNorth = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonEast = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonWest = (gcnew System::Windows::Forms::RadioButton());
			this->buttonSearchOrbits = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->textLatitude = (gcnew System::Windows::Forms::TextBox());
			this->textLongitude = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->checkedListOrbit = (gcnew System::Windows::Forms::CheckedListBox());
			this->panelInfo = (gcnew System::Windows::Forms::Panel());
			this->labelUTC = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->labelLocalTime = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->labelJulianDate = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->panelOrbit = (gcnew System::Windows::Forms::Panel());
			this->buttonAddOrbit = (gcnew System::Windows::Forms::Button());
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
			this->textBoxLS = (gcnew System::Windows::Forms::TextBox());
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
			this->groupBox2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->panelInfo->SuspendLayout();
			this->panelOrbit->SuspendLayout();
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
			this->panelControl->Location = System::Drawing::Point(605, 0);
			this->panelControl->Name = L"panelControl";
			this->panelControl->Size = System::Drawing::Size(342, 492);
			this->panelControl->TabIndex = 0;
			// 
			// panelObitList
			// 
			this->panelObitList->Controls->Add(this->groupBox2);
			this->panelObitList->Controls->Add(this->groupBox1);
			this->panelObitList->Controls->Add(this->buttonSearchOrbits);
			this->panelObitList->Controls->Add(this->label7);
			this->panelObitList->Controls->Add(this->textLatitude);
			this->panelObitList->Controls->Add(this->textLongitude);
			this->panelObitList->Controls->Add(this->label4);
			this->panelObitList->Controls->Add(this->checkedListOrbit);
			this->panelObitList->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelObitList->Location = System::Drawing::Point(0, 317);
			this->panelObitList->Name = L"panelObitList";
			this->panelObitList->Size = System::Drawing::Size(297, 175);
			this->panelObitList->TabIndex = 3;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->radioButtonSouth);
			this->groupBox2->Controls->Add(this->radioButtonNorth);
			this->groupBox2->Location = System::Drawing::Point(126, 54);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(59, 54);
			this->groupBox2->TabIndex = 7;
			this->groupBox2->TabStop = false;
			// 
			// radioButtonSouth
			// 
			this->radioButtonSouth->AutoSize = true;
			this->radioButtonSouth->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->radioButtonSouth->Location = System::Drawing::Point(6, 33);
			this->radioButtonSouth->Name = L"radioButtonSouth";
			this->radioButtonSouth->Size = System::Drawing::Size(51, 17);
			this->radioButtonSouth->TabIndex = 1;
			this->radioButtonSouth->Text = L"south";
			this->radioButtonSouth->UseVisualStyleBackColor = true;
			// 
			// radioButtonNorth
			// 
			this->radioButtonNorth->AutoSize = true;
			this->radioButtonNorth->Checked = true;
			this->radioButtonNorth->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->radioButtonNorth->Location = System::Drawing::Point(6, 10);
			this->radioButtonNorth->Name = L"radioButtonNorth";
			this->radioButtonNorth->Size = System::Drawing::Size(49, 17);
			this->radioButtonNorth->TabIndex = 0;
			this->radioButtonNorth->TabStop = true;
			this->radioButtonNorth->Text = L"north";
			this->radioButtonNorth->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radioButtonEast);
			this->groupBox1->Controls->Add(this->radioButtonWest);
			this->groupBox1->Location = System::Drawing::Point(191, 54);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(59, 54);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			// 
			// radioButtonEast
			// 
			this->radioButtonEast->AutoSize = true;
			this->radioButtonEast->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->radioButtonEast->Location = System::Drawing::Point(5, 33);
			this->radioButtonEast->Name = L"radioButtonEast";
			this->radioButtonEast->Size = System::Drawing::Size(45, 17);
			this->radioButtonEast->TabIndex = 1;
			this->radioButtonEast->Text = L"east";
			this->radioButtonEast->UseVisualStyleBackColor = true;
			// 
			// radioButtonWest
			// 
			this->radioButtonWest->AutoSize = true;
			this->radioButtonWest->Checked = true;
			this->radioButtonWest->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->radioButtonWest->Location = System::Drawing::Point(5, 10);
			this->radioButtonWest->Name = L"radioButtonWest";
			this->radioButtonWest->Size = System::Drawing::Size(47, 17);
			this->radioButtonWest->TabIndex = 0;
			this->radioButtonWest->TabStop = true;
			this->radioButtonWest->Text = L"west";
			this->radioButtonWest->UseVisualStyleBackColor = true;
			// 
			// buttonSearchOrbits
			// 
			this->buttonSearchOrbits->Location = System::Drawing::Point(235, 28);
			this->buttonSearchOrbits->Name = L"buttonSearchOrbits";
			this->buttonSearchOrbits->Size = System::Drawing::Size(52, 20);
			this->buttonSearchOrbits->TabIndex = 5;
			this->buttonSearchOrbits->Text = L"Search";
			this->buttonSearchOrbits->UseVisualStyleBackColor = true;
			this->buttonSearchOrbits->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonSearchOrbits_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(178, 28);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(10, 13);
			this->label7->TabIndex = 4;
			this->label7->Text = L":";
			// 
			// textLatitude
			// 
			this->textLatitude->Location = System::Drawing::Point(191, 28);
			this->textLatitude->Name = L"textLatitude";
			this->textLatitude->Size = System::Drawing::Size(40, 20);
			this->textLatitude->TabIndex = 3;
			this->textLatitude->Text = L"64";
			// 
			// textLongitude
			// 
			this->textLongitude->Location = System::Drawing::Point(136, 28);
			this->textLongitude->Name = L"textLongitude";
			this->textLongitude->Size = System::Drawing::Size(40, 20);
			this->textLongitude->TabIndex = 2;
			this->textLongitude->Text = L"119";
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
			this->checkedListOrbit->Size = System::Drawing::Size(104, 154);
			this->checkedListOrbit->TabIndex = 0;
			// 
			// panelInfo
			// 
			this->panelInfo->Controls->Add(this->labelUTC);
			this->panelInfo->Controls->Add(this->label3);
			this->panelInfo->Controls->Add(this->labelLocalTime);
			this->panelInfo->Controls->Add(this->label8);
			this->panelInfo->Controls->Add(this->labelJulianDate);
			this->panelInfo->Controls->Add(this->label6);
			this->panelInfo->Dock = System::Windows::Forms::DockStyle::Top;
			this->panelInfo->Location = System::Drawing::Point(0, 243);
			this->panelInfo->Name = L"panelInfo";
			this->panelInfo->Size = System::Drawing::Size(297, 74);
			this->panelInfo->TabIndex = 2;
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
			// labelLocalTime
			// 
			this->labelLocalTime->AutoSize = true;
			this->labelLocalTime->Location = System::Drawing::Point(101, 7);
			this->labelLocalTime->Name = L"labelLocalTime";
			this->labelLocalTime->Size = System::Drawing::Size(35, 13);
			this->labelLocalTime->TabIndex = 7;
			this->labelLocalTime->Text = L"label9";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(38, 7);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(55, 13);
			this->label8->TabIndex = 6;
			this->label8->Text = L"Local time";
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
			this->panelOrbit->Controls->Add(this->buttonAddOrbit);
			this->panelOrbit->Controls->Add(this->checkBoxAtmosphere);
			this->panelOrbit->Controls->Add(this->buttonScalePlus);
			this->panelOrbit->Controls->Add(this->buttonScaleMinus);
			this->panelOrbit->Controls->Add(this->textBoxScale);
			this->panelOrbit->Controls->Add(this->label2);
			this->panelOrbit->Controls->Add(this->panelOrbiMovie);
			this->panelOrbit->Controls->Add(this->textBoxLS);
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
			this->panelOrbit->Size = System::Drawing::Size(297, 243);
			this->panelOrbit->TabIndex = 1;
			// 
			// buttonAddOrbit
			// 
			this->buttonAddOrbit->Location = System::Drawing::Point(130, 25);
			this->buttonAddOrbit->Name = L"buttonAddOrbit";
			this->buttonAddOrbit->Size = System::Drawing::Size(31, 20);
			this->buttonAddOrbit->TabIndex = 14;
			this->buttonAddOrbit->Text = L"+";
			this->buttonAddOrbit->UseVisualStyleBackColor = true;
			this->buttonAddOrbit->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonAddOrbit_Click);
			// 
			// checkBoxAtmosphere
			// 
			this->checkBoxAtmosphere->AutoSize = true;
			this->checkBoxAtmosphere->Checked = true;
			this->checkBoxAtmosphere->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBoxAtmosphere->Location = System::Drawing::Point(24, 209);
			this->checkBoxAtmosphere->Name = L"checkBoxAtmosphere";
			this->checkBoxAtmosphere->Size = System::Drawing::Size(82, 17);
			this->checkBoxAtmosphere->TabIndex = 13;
			this->checkBoxAtmosphere->Text = L"Atmosphere";
			this->checkBoxAtmosphere->UseVisualStyleBackColor = true;
			this->checkBoxAtmosphere->CheckedChanged += gcnew System::EventHandler(this, &OpenGLControl::checkBoxAtmosphere_CheckedChanged);
			// 
			// buttonScalePlus
			// 
			this->buttonScalePlus->Location = System::Drawing::Point(205, 155);
			this->buttonScalePlus->Name = L"buttonScalePlus";
			this->buttonScalePlus->Size = System::Drawing::Size(20, 20);
			this->buttonScalePlus->TabIndex = 12;
			this->buttonScalePlus->Text = L"+";
			this->buttonScalePlus->UseVisualStyleBackColor = true;
			this->buttonScalePlus->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonScalePlus_Click);
			// 
			// buttonScaleMinus
			// 
			this->buttonScaleMinus->Location = System::Drawing::Point(183, 155);
			this->buttonScaleMinus->Name = L"buttonScaleMinus";
			this->buttonScaleMinus->Size = System::Drawing::Size(20, 20);
			this->buttonScaleMinus->TabIndex = 11;
			this->buttonScaleMinus->Text = L"-";
			this->buttonScaleMinus->UseVisualStyleBackColor = true;
			this->buttonScaleMinus->Click += gcnew System::EventHandler(this, &OpenGLControl::buttonScaleMinus_Click);
			// 
			// textBoxScale
			// 
			this->textBoxScale->Location = System::Drawing::Point(62, 155);
			this->textBoxScale->Name = L"textBoxScale";
			this->textBoxScale->Size = System::Drawing::Size(100, 20);
			this->textBoxScale->TabIndex = 10;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(21, 162);
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
			this->panelOrbiMovie->Location = System::Drawing::Point(16, 91);
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
			// textBoxLS
			// 
			this->textBoxLS->Location = System::Drawing::Point(183, 75);
			this->textBoxLS->Name = L"textBoxLS";
			this->textBoxLS->Size = System::Drawing::Size(100, 20);
			this->textBoxLS->TabIndex = 7;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(180, 58);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(45, 13);
			this->label5->TabIndex = 6;
			this->label5->Text = L"Orbit LS";
			// 
			// buttonSetOrbit
			// 
			this->buttonSetOrbit->Location = System::Drawing::Point(183, 205);
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
			this->label1->Location = System::Drawing::Point(180, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(74, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Orbits quantity";
			// 
			// textBoxOrbitQuantity
			// 
			this->textBoxOrbitQuantity->Location = System::Drawing::Point(183, 25);
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
			this->trackBarOrbit->Size = System::Drawing::Size(45, 492);
			this->trackBarOrbit->TabIndex = 0;
			this->trackBarOrbit->TickStyle = System::Windows::Forms::TickStyle::TopLeft;
			this->trackBarOrbit->Scroll += gcnew System::EventHandler(this, &OpenGLControl::trackBarOrbit_Scroll);
			// 
			// panelSeparator
			// 
			this->panelSeparator->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->panelSeparator->Dock = System::Windows::Forms::DockStyle::Right;
			this->panelSeparator->Location = System::Drawing::Point(602, 0);
			this->panelSeparator->Name = L"panelSeparator";
			this->panelSeparator->Size = System::Drawing::Size(3, 492);
			this->panelSeparator->TabIndex = 1;
			// 
			// panelGL
			// 
			this->panelGL->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelGL->Location = System::Drawing::Point(0, 0);
			this->panelGL->Name = L"panelGL";
			this->panelGL->Size = System::Drawing::Size(527, 492);
			this->panelGL->TabIndex = 2;
			// 
			// panelPalette
			// 
			this->panelPalette->Controls->Add(this->pictureBox1);
			this->panelPalette->Controls->Add(this->panelLabels);
			this->panelPalette->Dock = System::Windows::Forms::DockStyle::Right;
			this->panelPalette->Location = System::Drawing::Point(527, 0);
			this->panelPalette->Name = L"panelPalette";
			this->panelPalette->Size = System::Drawing::Size(75, 492);
			this->panelPalette->TabIndex = 0;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Right;
			this->pictureBox1->Location = System::Drawing::Point(25, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(50, 492);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &OpenGLControl::temperatureScale_Paint);
			// 
			// panelLabels
			// 
			this->panelLabels->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelLabels->Location = System::Drawing::Point(0, 0);
			this->panelLabels->Name = L"panelLabels";
			this->panelLabels->Size = System::Drawing::Size(75, 492);
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
			this->Name = L"OpenGLControl";
			this->Size = System::Drawing::Size(947, 492);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &OpenGLControl::OpenGLControl_Paint);
			this->Resize += gcnew System::EventHandler(this, &OpenGLControl::OpenGLControl_Resize);
			this->panelControl->ResumeLayout(false);
			this->panelControl->PerformLayout();
			this->panelObitList->ResumeLayout(false);
			this->panelObitList->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->panelInfo->ResumeLayout(false);
			this->panelInfo->PerformLayout();
			this->panelOrbit->ResumeLayout(false);
			this->panelOrbit->PerformLayout();
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

		System::Void updateOrbitInfo();

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

	private:
		void init();
};
}
