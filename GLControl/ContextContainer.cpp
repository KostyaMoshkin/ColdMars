#include "pch.h"

#include "ContextContainer.h"
#include "ControlContext.h"

namespace GL
{
	ContextContainer::ContextContainer(ContextInterface^ pContextInterface_)
		: m_pContextInterface(pContextInterface_)
	{
		InitializeComponent();

		this->panel->Controls->Add(m_pContextInterface->ViewControl);
	}

	void ContextContainer::InitializeComponent(void)
	{
		this->panel = (gcnew System::Windows::Forms::Panel());
		this->SuspendLayout();
		// 
		// panel
		// 
		this->panel->Dock = System::Windows::Forms::DockStyle::Fill;
		this->panel->Location = System::Drawing::Point(373, 0);
		this->panel->Name = L"panel";
		this->panel->Size = System::Drawing::Size(141, 452);
		this->panel->TabIndex = 0;
		// 
		// Form1
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(514, 452);
		this->Controls->Add(this->panel);
		this->ResumeLayout(false);

	}

	System::IntPtr ContextContainer::hWnd::get()
	{
		return System::IntPtr(m_pContextInterface->get_hWnd().ToPointer());
	}

}