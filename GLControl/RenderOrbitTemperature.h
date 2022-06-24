#pragma once
#include "Render.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IndirectBuffer.h"
#include "TextureBuffer.h"
#include "Palette.h"

#include "MegdrReader.h"
#include "OrbitTextReader.h"
#include "OrbitBinReader.h"

namespace GL {

	class RenderOrbitTemperature;
	using RenderOrbitTemperaturePtr = std::shared_ptr<RenderOrbitTemperature>;

	class RenderOrbitTemperature : public Render
	{
		ShaderProgramPtr m_pOrbitTemperatureProgram = nullptr;
		VertexBufferPtr m_pTemperatureVertex = nullptr;
		IndexBufferPtr m_pIndex = nullptr;
		IndirectBufferPtr m_pIndirect = nullptr;
		TextureBufferPtr m_pPaletteTexture = nullptr;

		PalettePtr m_pPalette = nullptr;
		//orbit::OrbitTextReaderPtr m_pOrbitReader = nullptr;
		orbit::OrbitReaderPtr m_pOrbitReader = nullptr;

		lib::Matrix4 m_mRotate = lib::Matrix4(1.0f);
		lib::Matrix4 m_mTransform = lib::Matrix4(1.0f);

		lib::fPoint2D m_fCamPosition;

		std::vector<orbit::SPairLevel> m_vLevelData;

		float m_fViewAngle = 45.0;

		unsigned m_nFileId = 4;
		unsigned m_nFirstFile = 0;
		unsigned m_nLastFile = 2;

		unsigned m_nIndexCount = 0;

		bool m_bIncludeAtmosphere = true;

		GLuint m_nVAO = 0;

	public:
		RenderOrbitTemperature();

		static RenderOrbitTemperaturePtr Create() { return std::make_shared<RenderOrbitTemperature>(); }

	private:
		void setScale();

		bool fillPalette();
		bool fillVertex();

	public:
		// Унаследовано через Render
		bool init() override;
		void draw() override;
		void setFileArray(const std::vector<unsigned>& vOrbit_);
		void setViewAngle(lib::Matrix4& mPerspective_) override;
		void lookAt(lib::Matrix4& mView_) override;
		void rotate(lib::Matrix4& mRotate_) override;
		void translate(lib::Matrix4& mTranslate_) override;
		void bound() override;
		void unbound() override;

		float getScale() override;
		void setScale(float fScale_) override;

	public:
		void setFileRange(int nFirstFile_, int nLasetFile_);

		const orbit::OrbitReaderPtr getReader();

		void getPalette(std::vector<lib::iPoint3D>& vPalette_, int& nPaletteMin_, int& nPaletteMax_);

		unsigned getOrbitIndex_by_OrbitNumber(unsigned nNumber_);

		unsigned getOrbit_by_LS(unsigned nNumber_);

		void setIncludeAtmosphere(bool bInclude_);
	};
}

