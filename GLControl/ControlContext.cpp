#include "pch.h"

#include "ControlContext.h"

#include <GLEW/glew.h>
#include <GLEW/wglew.h>

#include <string>

namespace GL
{
    static void destroy_context(HDC hDC_, HGLRC rendContext_)
    {
        if (rendContext_ != nullptr && hDC_ != nullptr)
            wglMakeCurrent(hDC_, nullptr);

        if (rendContext_ != nullptr)
            wglDeleteContext(rendContext_);
    }

    //---------------------------------------------------------------------------------

    bool ControlContext::begin_draw(int width_, int height_)
    {
        if (!makeCurrent())
            return false;

        if (width_ * height_ == 0)
            return true;

        glViewport(0, 0, width_, height_);

        glClearColor(1.0f, 1.0f, 1.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        return true;
    }

    void ControlContext::end_draw()
    {
        if (m_rendContext != nullptr)
            SwapBuffers(m_hDC);

        wglMakeCurrent(m_hDC, nullptr);
    }

    bool ControlContext::initContext(HWND hWnd_)
    {
        if (hasContext())
            return true;

        if (!initWindow(hWnd_))
            return false;

        if (!initGlew())
            return false;

        m_nVersionSupported = GL::ControlContext::getOpenGLVersion();

        //if (!createContext())
        //    return false;

        return true;
    }

    void ControlContext::releaseContext()
    {
        if (m_rendContext != nullptr)
        {
            destroy_context(m_hDC, m_rendContext);
            m_rendContext = nullptr;
        }

        destroyWindow();
    }

    bool ControlContext::initWindow(HWND hWnd_)
    {
        m_hWnd = hWnd_;
        if (m_hWnd == nullptr)
            return false;

        m_hDC = GetDC(m_hWnd);
        if (m_hDC == nullptr)
            return false;

        PIXELFORMATDESCRIPTOR pfd = {
          sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
          1,                     // version number  
          PFD_DRAW_TO_WINDOW |   // support window  
          PFD_SUPPORT_OPENGL |   // support OpenGL  
          PFD_DOUBLEBUFFER,      // double buffered  
          PFD_TYPE_RGBA,         // RGBA type  
          24,                    // 24-bit color depth  
          0, 0, 0, 0, 0, 0,      // color bits ignored  
          0,                     // no alpha buffer  
          0,                     // shift bit ignored  
          0,                     // no accumulation buffer  
          0, 0, 0, 0,            // accum bits ignored  
          32,                    // 32-bit z-buffer      
          0,                     // no stencil buffer  
          0,                     // no auxiliary buffer  
          PFD_MAIN_PLANE,        // main layer  
          0,                     // reserved  
          0, 0, 0                // layer masks ignored  
        };

        int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
        if (pixelFormat == 0)
        {
            return false;
        }

        if (!SetPixelFormat(m_hDC, pixelFormat, &pfd))
        {
            return false;
        }

        return true;
    }

    void ControlContext::destroyWindow()
    {
        if (m_hDC == nullptr)
            return;

        if (m_hWnd != nullptr)
            ReleaseDC(m_hWnd, m_hDC);

        m_hDC = nullptr;
    }

    bool ControlContext::initGlew()
    {
        static bool glew_created = false;

        if (glew_created)
            return true;

        //------------------------------------------------------------------------------------------
        // init temp context

        HGLRC tempContext = wglCreateContext(m_hDC);
        if (tempContext == nullptr)
        {
            return false;
        }

        if (!wglMakeCurrent(m_hDC, tempContext))
        {
            wglDeleteContext(tempContext);

            return false;
        }

        //------------------------------------------------------------------------------------------

        GLenum glewInitResult = UINT_MAX;

        try
        {
            glewInitResult = glewInit();
            wglewInit();
        }
        catch (...)
        {
        }

        //------------------------------------------------------------------------------------------

        if (glewInitResult != GLEW_OK)
        {
            wglMakeCurrent(m_hDC, nullptr);

            return false;
        }

        m_rendContext = tempContext;

        //wglMakeCurrent(m_hDC, nullptr);
        glew_created = true;
        return true;
    }

    bool ControlContext::hasContext()
    {
        //GUARD_SECTION_CS(m_csAccess);
        return m_rendContext != nullptr;
    }

    bool ControlContext::createContext()
    {
        //GUARD_SECTION_CS(m_csAccess);

        m_rendContext = wglCreateContext(m_hDC);

        return m_rendContext != nullptr;
    }

    bool ControlContext::makeCurrent()
    {
        if (!m_rendContext)
            return false;

        if (!wglMakeCurrent(m_hDC, m_rendContext))
        {
            return false;
        }

        return true;
    }

    int ControlContext::getOpenGLVersion()
    {
        static int nVersionFull = -1;

        if (nVersionFull != -1)
            return nVersionFull;

        //------------------------------------------------------------------------------------------

        const GLubyte* pVersion = glGetString(GL_VERSION);
        if (!pVersion)
            return nVersionFull;

        std::string sVersion = (const char*)pVersion;
        if (sVersion.length() < 3)
            return nVersionFull;

        int nVersionMain = sVersion[0] - '0';
        int nVersionSub = sVersion[2] - '0';

        nVersionFull = nVersionMain * 10 + nVersionSub;
        //       nVersionFull = 30;

              //------------------------------------------------------------------------------------------

        //if ( nVersionFull < 20 )
        //  FORCE_ASSERTM("Current Open GL version is not supported.");

        return nVersionFull;
    }

}