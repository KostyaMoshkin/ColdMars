#pragma once

namespace GL
{
	public interface class ContextInterface
	{
       System::IntPtr get_hWnd();

    public:
        property System::Windows::Forms::Control^ ViewControl
        {
            System::Windows::Forms::Control^ get();
        }

	};

}
