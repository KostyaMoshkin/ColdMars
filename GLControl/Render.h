#pragma once

#include "lib.h"
#include "Vocabulary.h"
#include "CConfig.h"

#include <memory>

 namespace GL {

     struct ShaderName
     {
         static const int orbittemperature_vertex       = IDR_DATA1;
         static const int orbittemperature_fragment     = IDR_DATA2;
         static const int megdr_vertex                  = IDR_DATA3;
         static const int megdr_fragment                = IDR_DATA4;

         static const char* getName(int ID_)
         {
             switch (ID_)
             {
             case orbittemperature_vertex:      return "orbittemperature_vertex";
             case orbittemperature_fragment:    return "orbittemperature_fragment";
             case megdr_vertex:                 return "megdr_vertex";
             case megdr_fragment:               return "megdr_fragment";
             default: return "";
             }
         }
     };

      class Render;
      using RenderPtr = std::shared_ptr<Render>;

      // Класс интерфейс рисования. Для каждой версии OpenGL и для каждой графической сущности свой наследник
      class Render : public lib::CConfig
      {
          bool m_bVisible = false;
          int m_nVersionFull = 0;

      protected:
          float m_fScale = 5.0f;

      public:
          Render() = default;
          virtual ~Render() = default;

        public:
            virtual bool init() = 0;
            virtual void lookAt(lib::Matrix4& mView_) = 0;
            virtual void rotate(lib::Matrix4& mRotate_) = 0;
            virtual void translate(lib::Matrix4& mTranslate_) = 0;
            virtual void setViewAngle(lib::Matrix4& mPerspective_) = 0;
            virtual void draw() = 0;

            virtual bool keyPress(GL::EKeyPress nKey_) = 0;

            virtual void bound() = 0;
            virtual void unbound() = 0;


      public:
            virtual float getScale() = 0;
            virtual void setScale(float fScale_) = 0;

      public:
          void setVisible(bool bVisible_) { m_bVisible = bVisible_; }
          bool isVisible() { return m_bVisible; }

          void setVersionGl(int nVersionFull_) { m_nVersionFull = nVersionFull_; }
          int getVersionGl() { return m_nVersionFull; }

      };

}
