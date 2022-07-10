#include "pch.h"

#include "RenderOrbitTemperature.h"
#include "BufferBounder.h"

#pragma managed(push, off)

namespace GL {
	RenderOrbitTemperature::RenderOrbitTemperature()
	{
	}

	void RenderOrbitTemperature::setScale()
	{
		lib::limit(m_fScale, 0.1f, 30.0f);

		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		m_pOrbitTemperatureProgram->setUniform1f("m_fScale", &m_fScale);
	}

	void RenderOrbitTemperature::setViewAngle(lib::Matrix4& mPerspective_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		m_pOrbitTemperatureProgram->setUniformMat4f("m_mPerspective", &mPerspective_[0][0]);
	}

	void RenderOrbitTemperature::lookAt(lib::Matrix4& mView_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		m_pOrbitTemperatureProgram->setUniformMat4f("m_mView", &mView_[0][0]);
	}

	void RenderOrbitTemperature::rotate(lib::Matrix4& mRotate_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		m_pOrbitTemperatureProgram->setUniformMat4f("m_mRotate", &mRotate_[0][0]);
	}

	void RenderOrbitTemperature::translate(lib::Matrix4& mTranslate_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		m_pOrbitTemperatureProgram->setUniformMat4f("m_mTranslate", &mTranslate_[0][0]);
	}

	bool RenderOrbitTemperature::fillPalette()
	{
		float fDataMin;
		float fDataMax;

		m_pPalette->getMinMax(fDataMin, fDataMax);

		const unsigned nPaletteSize = m_pPalette->getInterpolate();

		std::vector<lib::fPoint3D> vColorText(nPaletteSize);
		for (size_t i = 0; i < nPaletteSize; ++i)
			lib::unpackColor(m_pPalette->get(int(fDataMin + (fDataMax - fDataMin) * i / (nPaletteSize - 1))), vColorText[i]);

		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		BufferBounder<TextureBuffer> paletteTextureBounder(m_pPaletteTexture);

		if (!m_pPaletteTexture->fillBuffer1D(GL_RGB, vColorText.size(), GL_RGB, GL_FLOAT, vColorText.data()))
		{
			toLog("ERROR    m_pPeletteTexture->fillBuffer1D(GL_RGB, vColorText.size(), GL_RGB, GL_FLOAT, vColorText.data()))");
			return false;
		}

		m_pOrbitTemperatureProgram->setUniform1f("m_fPaletteValueMin", &fDataMin);
		m_pOrbitTemperatureProgram->setUniform1f("m_fPaletteValueMax", &fDataMax);

		return true;
	}

	bool RenderOrbitTemperature::fillVertex()
	{
		if (!m_pOrbitReader)
			toLog("!m_pOrbitReader ");

		m_pOrbitReader->setFileIndex(m_nFirstFile, m_nLastFile, m_vLevelData, m_bIncludeAtmosphere);

		m_bNeedFillLevelBufer = true;

		return true;
	}

	bool RenderOrbitTemperature::init()
	{
		m_pIndex = GL::IndexBuffer::Create();
		m_pIndirect = GL::IndirectBuffer::Create();

		ShaderProgramPtr pOrbitTemperatureProgram = ShaderProgram::Create();

		pOrbitTemperatureProgram->addShader(ShaderName::orbittemperature_vertex, ShaderProgram::ShaderType::Vertex());
		pOrbitTemperatureProgram->addShader(ShaderName::orbittemperature_fragment, ShaderProgram::ShaderType::Fragment());

		bool bProgramCompile = pOrbitTemperatureProgram->init();
		if (!bProgramCompile)
			return false;

		m_pOrbitTemperatureProgram = pOrbitTemperatureProgram;

		glGenVertexArrays(1, &m_nVAO);

		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		BufferBounder<RenderOrbitTemperature> renderBounder(this);

		//-------------------------------------------------------------------------------------

		m_pPaletteTexture = GL::TextureBuffer::Create(GL_TEXTURE_1D, GL_TEXTURE0, GL_NEAREST);
		m_pPaletteTexture->alignment(0);

		int nPalette = 0;
		m_pOrbitTemperatureProgram->setUniform1i("m_tPalette", &nPalette);

		//-------------------------------------------------------------------------------------------------

		m_pLevelPosition = GL::ShaderStorageBuffer::Create(0);

		//-------------------------------------------------------------------------------------------------

		int nBaseHeight;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), Key::BaseHeight()), nBaseHeight))
			nBaseHeight = 3396000;

		m_pOrbitTemperatureProgram->setUniform1i("m_nBaseHeight", &nBaseHeight);

		//-------------------------------------------------------------------------------------------------

		renderBounder.unbound();

		//-------------------------------------------------------------------------------------------------

		setScale();

		//  ���������� ������
		m_pOrbitReader = orbit::OrbitBinReader::Create();
		m_pOrbitReader->setConfig(getConfig());

		if (!m_pOrbitReader->init())
			return false;

		if (!fillVertex())
			return false;
		//-------------------------------------------------------------------------------------------------

		m_pPalette = GL::Palette::Create();

		m_pPalette->setConfig(getConfig());

		if (!m_pPalette->init())
			return false;

		if (!fillPalette())
			return false;

		//-------------------------------------------------------------------------------------------------

		setVisible(true);
		return true;

		//-------------------------------------------------------------------------------------------------

		return true;
	}

	void RenderOrbitTemperature::draw()
	{
		if (!isVisible())
			return;

		if (m_bNeedFillLevelBufer)
			fillLevelBuffer();

		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		BufferBounder<RenderOrbitTemperature> renderBounder(this);
		BufferBounder<TextureBuffer> paletteTextureBounder(m_pPaletteTexture);
		BufferBounder<ShaderStorageBuffer> ssboBounder(m_pLevelPosition);

		for (int i = 0; i < m_pvTemperatureVertex.size(); ++i)
		{
			BufferBounder<VertexBuffer> temperatureBounder(m_pvTemperatureVertex[i].first);
			m_pvTemperatureVertex[i].first->attribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);

			m_pOrbitTemperatureProgram->setUniform1i("m_nLevelIndex", &i);

			if(m_bIncludeAtmosphere)
				glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)m_pvTemperatureVertex[i].second.vTemperature.size());

			glLineWidth(m_bIncludeAtmosphere ? 3.0f : 9.0f);

			glDrawArrays(GL_LINES, 0, 2);
		}

		renderBounder.unbound();
	}

	bool RenderOrbitTemperature::fillLevelBuffer()
	{
		m_bNeedFillLevelBufer = false;

		std::vector<orbit::SLevelCoord> vLevelCoord(m_vLevelData.size());

		m_pvTemperatureVertex.clear();
		m_pvTemperatureVertex.resize(m_vLevelData.size());

		for (int i = 0; i < m_vLevelData.size(); ++i)
		{
			m_pvTemperatureVertex[i].first = GL::VertexBuffer::Create();
			m_pvTemperatureVertex[i].first->setUsage(GL_STATIC_DRAW);

			BufferBounder<VertexBuffer> temperatureBounder(m_pvTemperatureVertex[i].first);

			orbit::SPairLevel& levelData = m_vLevelData[i];
			m_pvTemperatureVertex[i].second = levelData;

			if (!m_pvTemperatureVertex[i].first->fillBuffer(levelData.vTemperature.size() * sizeof(float), levelData.vTemperature.data()))
			{
				toLog("Error m_pvTemperatureVertex[i].first->fillBuffer");
				return false;
			}

			vLevelCoord[i].fAltitudeMinMax = levelData.fAltitudeMinMax;
			vLevelCoord[i].fAltitudeStep = levelData.fAltitudeStep;
			vLevelCoord[i].fDistance_begin = levelData.fDistance_begin;
			vLevelCoord[i].fDistance_end = levelData.fDistance_end;
			vLevelCoord[i].fLatitude_begin = levelData.fLatitude_begin;
			vLevelCoord[i].fLatitude_end = levelData.fLatitude_end;
			vLevelCoord[i].fLongitude_begin = levelData.fLongitude_begin;
			vLevelCoord[i].fLongitude_end = levelData.fLongitude_end;
		}

		//------------------------------------------------------------------------------------------------

		BufferBounder<ShaderStorageBuffer> ssboBounder(m_pLevelPosition);
		if (!m_pLevelPosition->fillBuffer(sizeof(orbit::SLevelCoord) * vLevelCoord.size(), vLevelCoord.data()))
		{
			toLog("ERROR m_pLevelPosition->fillBuffer");
			return false;
		}
	}

	void RenderOrbitTemperature::setFileArray(const std::vector<unsigned>& vOrbit_)
	{
		if (vOrbit_.empty())
			m_vLevelData.clear();

		for (int i = 0; i < vOrbit_.size(); ++i)
		{
			m_nFirstFile = vOrbit_[i];
			m_nLastFile = vOrbit_[i] + 1;

			m_pOrbitReader->setFileIndex(m_nFirstFile, m_nLastFile, m_vLevelData, m_bIncludeAtmosphere, i == 0);
		}

		m_bNeedFillLevelBufer = true;
	}

	void RenderOrbitTemperature::setFileRange(int nFirstFile_, int nLasetFile_)
	{
		m_nFirstFile = (unsigned)nFirstFile_;
		m_nLastFile = (unsigned)nLasetFile_;

		fillVertex();
	}

	const orbit::OrbitBinReaderPtr RenderOrbitTemperature::getReader()
	{
		return m_pOrbitReader;
	}

	void RenderOrbitTemperature::getPalette(std::vector<lib::iPoint3D>& vPalette_, int& nPaletteMin_, int& nPaletteMax_)
	{
		m_pPalette->getPalette(vPalette_, nPaletteMin_, nPaletteMax_);
	}

	unsigned RenderOrbitTemperature::getOrbitIndex_by_OrbitNumber(unsigned nNumber_)
	{
		return m_pOrbitReader->getOrbitIndex_by_OrbitNumber(nNumber_);
	}

	unsigned RenderOrbitTemperature::getOrbit_by_LS(unsigned nNumber_)
	{
		return m_pOrbitReader->getOrbit_by_LS(nNumber_);
	}

	void RenderOrbitTemperature::setIncludeAtmosphere(bool bInclude_)
	{
		m_bIncludeAtmosphere = bInclude_;
	}

	void RenderOrbitTemperature::bound()
	{
		glBindVertexArray(m_nVAO);
	}

	void RenderOrbitTemperature::unbound()
	{
		glBindVertexArray(0);
	}
	float RenderOrbitTemperature::getScale()
	{
		return m_fScale;
	}

	void RenderOrbitTemperature::setScale(float fScale_)
	{
		m_fScale = fScale_;
		setScale();
	}
}

#pragma managed(pop)