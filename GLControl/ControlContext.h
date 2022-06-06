#pragma once

#include <Windows.h>

#include <memory>

namespace GL
{
    class ControlContext;
    using ControlContextPtr = std::shared_ptr<ControlContext>;

	class ControlContext
	{
        HWND m_hWnd = nullptr;
        HDC m_hDC = nullptr;

        HGLRC m_rendContext = nullptr;

        int m_nVersionSupported = -1;

        long m_nAccessCounter = 0;

    public:
        static ControlContextPtr Create() { return std::make_shared<ControlContext>(); }

    public:
        bool initContext(HWND hWnd_);

        void releaseContext();

    public:
        bool begin_draw(int width_, int height_);
        void end_draw();

    private:
        bool initWindow(HWND hWnd_);

        void destroyWindow();

        bool initGlew();

        bool hasContext();

        bool createContext();

        bool makeCurrent();

        static int getOpenGLVersion();

	};
}