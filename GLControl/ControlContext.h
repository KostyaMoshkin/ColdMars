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

        int m_nWidth = 0;
        int m_nHeight = 0;

        long m_nAccessCounter = 0;

    public:
        static ControlContextPtr Create() { return std::make_shared<ControlContext>(); }

    public:
        bool initContext(HWND hWnd_);

        void releaseContext();

    public:
        bool begin_draw();
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

    class ContextSession
    {
        ControlContextPtr m_pContext;
        bool m_started = false;

    public:
        ContextSession(ControlContextPtr pContext_, int width_, int height_)
            : m_pContext(pContext_)
        {
            m_started = m_pContext->begin_draw(width_, height_);
        }

        ContextSession(ControlContextPtr pContext_)
            : m_pContext(pContext_)
        {
            m_started = m_pContext->begin_draw();
        }

        ~ContextSession()
        {
            m_pContext->end_draw();
        }

        bool is_started() const
        {
            return m_started;
        }
    };
}