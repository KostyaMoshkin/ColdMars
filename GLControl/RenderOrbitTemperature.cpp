#include "pch.h"

#include "RenderOrbitTemperature.h"
#include "BufferBounder.h"

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
		m_pOrbitReader->setFileIndex(m_nFirstFile, m_nLastFile, m_vLevelData);

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

		m_pTemperatureVertex = GL::VertexBuffer::Create();
		m_pTemperatureVertex->setUsage(GL_STATIC_DRAW);

		BufferBounder<VertexBuffer> temperatureBounder(m_pTemperatureVertex);

		m_pTemperatureVertex->attribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);

		//-------------------------------------------------------------------------------------------------

		int nBaseHeight;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), BaseHeight()), nBaseHeight))
			nBaseHeight = 3396000;

		m_pOrbitTemperatureProgram->setUniform1i("m_nBaseHeight", &nBaseHeight);

		//-------------------------------------------------------------------------------------------------

		renderBounder.unbound();

		//-------------------------------------------------------------------------------------------------

		setScale();

		//  Координаты вершин
		m_pOrbitReader = orbit::OrbitReader::Create();
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

		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		BufferBounder<RenderOrbitTemperature> renderBounder(this);
		BufferBounder<VertexBuffer> temperatureBounder(m_pTemperatureVertex);
		BufferBounder<TextureBuffer> paletteTextureBounder(m_pPaletteTexture);

		for (const orbit::SPairLevel& levelData : m_vLevelData)
		{
			if (!m_pTemperatureVertex->fillBuffer(levelData.vTemperature.size() * sizeof(float), levelData.vTemperature.data()))
			{
				toLog("Error m_pTemperatureVertex->fillBuffer()");
				return;
			}

			m_pOrbitTemperatureProgram->setUniform1f("m_fAltitudeMinMax", &levelData.fAltitudeMinMax);
			m_pOrbitTemperatureProgram->setUniform1f("m_fAltitudeStep", &levelData.fAltitudeStep);
			m_pOrbitTemperatureProgram->setUniform1f("m_fDistance_begin", &levelData.fDistane_begin);
			m_pOrbitTemperatureProgram->setUniform1f("m_fDistance_end", &levelData.fDistane_end);
			m_pOrbitTemperatureProgram->setUniform1f("m_fLatitude_begin", &levelData.fLatitude_begin);
			m_pOrbitTemperatureProgram->setUniform1f("m_fLatitude_end", &levelData.fLatitude_end);
			m_pOrbitTemperatureProgram->setUniform1f("m_fLongitude_begin", &levelData.fLongitude_begin);
			m_pOrbitTemperatureProgram->setUniform1f("m_fLongitude_end", &levelData.fLongitude_end);

			if(m_bIncludeAtmosphere)
				glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)levelData.vTemperature.size());

			glDrawArrays(GL_LINES, 0, 2);

		}

		renderBounder.unbound();
	}

	bool RenderOrbitTemperature::keyPress(GL::EKeyPress nKey_)
	{
		if ((nKey_ == GL::EKeyPress::key_1) || (nKey_ == GL::EKeyPress::key_2))
		{
			m_fScale = (nKey_ == GL::EKeyPress::key_1) ? m_fScale /= 1.2f : m_fScale *= 1.2f;
			setScale();
		}
		else if ((nKey_ == GL::EKeyPress::key_5) || (nKey_ == GL::EKeyPress::key_6))
		{
			m_nFileId = (nKey_ == GL::EKeyPress::key_5) ? --m_nFileId : ++m_nFileId;
			lib::limit<unsigned>(m_nFileId, 0, (unsigned)m_pOrbitReader->getFileCount() - 1);
			fillVertex();
		}

		return true;
	}

	void RenderOrbitTemperature::setFileRange(int nFirstFile_, int nLasetFile_)
	{
		m_nFirstFile = (unsigned)nFirstFile_;
		m_nLastFile = (unsigned)nLasetFile_;

		fillVertex();
	}

	const orbit::OrbitReaderPtr RenderOrbitTemperature::getReader()
	{
		return m_pOrbitReader;
	}

	void RenderOrbitTemperature::getPalette(std::vector<lib::iPoint3D>& vPalette_, int& nPaletteMin_, int& nPaletteMax_)
	{
		m_pPalette->getPalette(vPalette_, nPaletteMin_, nPaletteMax_);
	}

	unsigned RenderOrbitTemperature::getOrbit_by_number(unsigned nNumber_)
	{
		return m_pOrbitReader->getOrbit_by_number(nNumber_);
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