#pragma once
#include "Render.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IndirectBuffer.h"
#include "TextureBuffer.h"

#include "MegdrReader.h"
#include "OrbitReader.h"

#include "Lib.h"
#include "Vocabulary.h"

namespace GL {

	class RenderMegdr;
	using RenderMegdrPtr = std::shared_ptr<RenderMegdr>;

	class RenderMegdr : public Render
	{
		static const char* Albedo()		{ return "Albedo";		}
		static const char* MarsTone()	{ return "MarsTone";	}
		static const char* BaseHeight()	{ return "BaseHeight"; }

		ShaderProgramPtr m_pMegdrProgram = nullptr;
		VertexBufferPtr m_pRadiusVertex = nullptr;
		VertexBufferPtr m_pTopographyVertex = nullptr;
		IndexBufferPtr m_pIndex = nullptr;
		IndirectBufferPtr m_pIndirect = nullptr;
		TextureBufferPtr m_pPaletteTexture = nullptr;
		TextureBufferPtr m_pAlbedoTexture = nullptr;

		megdr::MegdrReaderPtr m_pMegdr = nullptr;
		orbit::OrbitReaderPtr m_pOrbitReader = nullptr;

		lib::Matrix4 m_mRotate = lib::Matrix4(1.0f);
		lib::Matrix4 m_mTransform = lib::Matrix4(1.0f);

		lib::fPoint2D m_fCamPosition;

		float m_fScale = 5.0f;

		GLuint m_nVAO = 0;

	public:
		RenderMegdr();

		static RenderMegdrPtr Create() { return std::make_shared<RenderMegdr>(); }

	private:
		void setScale();

		bool fillVertex();
		bool fillAlbedo(const char* sFileName_, unsigned nMarsTone_);

	public:
		// ������������ ����� Render
		bool init() override;
		void draw() override;
		void setViewAngle(lib::Matrix4& mPerspective_) override;
		void lookAt(lib::Matrix4& mView_) override;
		virtual void rotate(float fAngle_) override;
		void bound() override;
		void unbound() override;

		// ������������ ����� Render
		virtual bool keyPress(GL::EKeyPress nKey_) override;
	};
}

