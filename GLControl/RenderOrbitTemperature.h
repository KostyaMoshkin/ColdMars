#pragma once
#include "Render.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IndirectBuffer.h"
#include "TextureBuffer.h"
#include "Palette.h"

#include "MegdrReader.h"
#include "OrbitReader.h"

#include "Lib.h"
#include "Vocabulary.h"

namespace GL {

	class RenderOrbitTemperature;
	using RenderOrbitTemperaturePtr = std::shared_ptr<RenderOrbitTemperature>;

	class RenderOrbitTemperature : public Render
	{
		static const char* ViewAngle()	{ return "ViewAngle";	}
		static const char* BaseHeight() { return "BaseHeight"; }

		ShaderProgramPtr m_pOrbitTemperatureProgram = nullptr;
		VertexBufferPtr m_pTemperatureVertex = nullptr;
		IndexBufferPtr m_pIndex = nullptr;
		IndirectBufferPtr m_pIndirect = nullptr;
		TextureBufferPtr m_pPaletteTexture = nullptr;

		PalettePtr m_pPalette = nullptr;
		orbit::OrbitReaderPtr m_pOrbitReader = nullptr;

		lib::Matrix4 m_mRotate = lib::Matrix4(1.0f);
		lib::Matrix4 m_mTransform = lib::Matrix4(1.0f);

		lib::fPoint2D m_fCamPosition;

		std::vector<orbit::SPairLevel> m_vLevelData;

		float m_fScale = 5.0f;
		float m_fViewAngle = 45.0;
		GLfloat m_fViewAspect = 1.0;

		unsigned m_nFileId = 4;

		unsigned m_nIndexCount = 0;

		GLuint m_nVAO = 0;

	public:
		RenderOrbitTemperature();

		static RenderOrbitTemperaturePtr Create() { return std::make_shared<RenderOrbitTemperature>(); }

	private:
		void setScale();

		bool fillPalette();
		bool fillVertex();

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

