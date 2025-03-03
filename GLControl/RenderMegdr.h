#pragma once
#include "Render.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IndirectBuffer.h"
#include "TextureBuffer.h"
#include "ShaderStorageBuffer.h"

#include "MegdrReader.h"

namespace GL {

	class RenderMegdr;
	using RenderMegdrPtr = std::shared_ptr<RenderMegdr>;

	class RenderMegdr : public Render
	{
		ShaderProgramPtr m_pMegdrProgram = nullptr;
		VertexBufferPtr m_pRadiusVertex = nullptr;
		VertexBufferPtr m_pTopographyVertex = nullptr;
		IndexBufferPtr m_pIndex = nullptr;
		IndirectBufferPtr m_pIndirect = nullptr;
		TextureBufferPtr m_pPaletteTexture = nullptr;
		TextureBufferPtr m_pAlbedoTexture = nullptr;
		TextureBufferPtr m_pDigitsTexture = nullptr;
		ShaderStorageBufferPtr m_pSSBO = nullptr;

		megdr::MegdrReaderPtr m_pMegdr = nullptr;

		lib::Matrix4 m_mRotate = lib::Matrix4(1.0f);
		lib::Matrix4 m_mTransform = lib::Matrix4(1.0f);

		lib::fPoint2D m_fCamPosition;

		int m_nWindowHeight;

		GLuint m_nVAO = 0;

	public:
		RenderMegdr();

		static RenderMegdrPtr Create() { return std::make_shared<RenderMegdr>(); }

	private:
		void setScale();

		bool fillMegdrVertex();
		bool fillAlbedo(const char* sFileName_, unsigned nMarsTone_);
		bool fillDigit(const char* sFileName_, unsigned nMarsTone_);

	public:
		// ������������ ����� Render
		bool init() override;
		void draw() override;
		void setViewAngle(lib::Matrix4& mPerspective_) override;
		void lookAt(lib::Matrix4& mView_) override;
		void rotate(lib::Matrix4& mRotate_) override;
		void translate(lib::Matrix4& mTranslate_) override;
		void bound() override;
		void unbound() override;

		float getScale() override;
		void setScale(float fScale_) override;

		void changeLongitudeMode(longitude::mode longitudeMode_) override;;

		void sizeChanged(int nWidth_, int nHeight_) override;

	public:
		void on_mouse_click(int nPosX_, int nPosY_);

		lib::fPoint2D getClickCoords();

	};
}

