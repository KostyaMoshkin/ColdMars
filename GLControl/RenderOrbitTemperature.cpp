#include "pch.h"

#include "RenderOrbitTemperature.h"
#include "BufferBounder.h"

#pragma managed(push, off)

namespace GL {

	static void checkAddRemoveOrbits(const std::vector<unsigned>& vOrbit_, std::vector<unsigned>& vExistOrbit_, std::vector<unsigned>& vAddOrbit_, std::vector<unsigned>& vRemoveOrbit_)
	{
		vAddOrbit_.clear();
		vRemoveOrbit_.clear();

		for (const unsigned nExistOrbit : vExistOrbit_)
		{
			bool bExist = false;
			for (const unsigned nUseOrbit : vOrbit_)
			{
				bExist = nUseOrbit == nExistOrbit;
				if (bExist)
					break;
			}

			if (!bExist)
				vRemoveOrbit_.push_back(nExistOrbit);
		}

		for (const unsigned nUseOrbit : vOrbit_)
		{
			bool bExist = false;
			for (const unsigned nExistOrbit : vExistOrbit_)
			{
				bExist = nUseOrbit == nExistOrbit;
				if (bExist)
					break;
			}

			if (!bExist)
				vAddOrbit_.push_back(nUseOrbit);
		}

		vExistOrbit_ = vOrbit_;
	}

	//---------------------------------------------------------------------------------------------------

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

	bool RenderOrbitTemperature::fillPalette(int nPaletteIndex_)
	{
		m_pPalette->changePalette(nPaletteIndex_);

		float fDataMin;
		float fDataMax;

		m_pPalette->getMinMax(fDataMin, fDataMax);

		const unsigned nPaletteSize = m_pPalette->getInterpolate();

		std::vector<lib::fPoint3D> vColorText(nPaletteSize);
		for (size_t i = 0; i < nPaletteSize; ++i)
			lib::unpackColor(m_pPalette->get(fDataMin + (fDataMax - fDataMin) * i / (nPaletteSize - 1)), vColorText[i]);

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

	bool RenderOrbitTemperature::fillVertex(unsigned nOrbitStart_, unsigned nOrbitEnd_)
	{
		if (!m_pOrbitReader)
		{
			toLog("!m_pOrbitReader ");
			return false;
		}

		std::vector<unsigned> vOrbit;
		for (unsigned i = nOrbitStart_; i <= nOrbitEnd_; ++i)
			vOrbit.push_back(i);

		setFileArray(vOrbit);

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

		int nBaseHeight;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), Key::BaseHeight()), nBaseHeight))
			nBaseHeight = 3396000;

		m_pOrbitTemperatureProgram->setUniform1i("m_nBaseHeight", &nBaseHeight);

		//-------------------------------------------------------------------------------------------------

		renderBounder.unbound();

		//-------------------------------------------------------------------------------------------------

		setScale();

		//  Координаты вершин
		m_pOrbitReader = orbit::OrbitBinReader::Create();
		m_pOrbitReader->setConfig(getConfig());

		if (!m_pOrbitReader->init())
			return false;

		if (!fillVertex(0, 1))
			return false;
		//-------------------------------------------------------------------------------------------------

		m_pPalette = GL::Palette::Create();

		m_pPalette->setConfig(getConfig());

		if (!m_pPalette->init())
			return false;

		if (!fillPalette(1))
			return false;

		//-------------------------------------------------------------------------------------------------

		setVisible(true);

		//-------------------------------------------------------------------------------------------------

		return true;
	}

	void RenderOrbitTemperature::draw()
	{
		if (!isVisible())
			return;

		if (m_bNeedFillLevelBufer)
		{
			if (!fillLevelBuffer())
				return;
		}

		BufferBounder<ShaderProgram> programBounder(m_pOrbitTemperatureProgram);
		BufferBounder<RenderOrbitTemperature> renderBounder(this);
		BufferBounder<TextureBuffer> paletteTextureBounder(m_pPaletteTexture);

		for (int j = 0; j < m_pvvTemperatureVertex.size(); ++j)
		{
			BufferBounder<ShaderStorageBuffer> ssboBounder(m_pvLevelPosition[j].second);
			for (int i = 0; i < m_pvvTemperatureVertex[j].second.size(); ++i)
			{
				BufferBounder<VertexBuffer> temperatureBounder(m_pvvTemperatureVertex[j].second[i].first);
				m_pvvTemperatureVertex[j].second[i].first->attribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);

				m_pOrbitTemperatureProgram->setUniform1i("m_nLevelIndex", &i);

				if (m_displayMode == display::mode::temperature && m_bIncludeAtmosphere)
					glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)m_pvvTemperatureVertex[j].second[i].second.nCount);

				glLineWidth(m_displayMode == display::mode::temperature && m_bIncludeAtmosphere ? 3.0f : 9.0f);

				glDrawArrays(GL_LINES, 0, 2);
			}
		}

		renderBounder.unbound();
	}

	bool RenderOrbitTemperature::fillLevelBuffer()
	{
		m_bNeedFillLevelBufer = false;

		for (const unsigned nRemoveOrbit : m_vRemoveOrbit)
			for (int i = 0; i < m_pvvTemperatureVertex.size(); ++i)
				if (nRemoveOrbit == m_pvvTemperatureVertex[i].first)
				{
					m_pvvTemperatureVertex.erase(m_pvvTemperatureVertex.begin() + i);
					m_pvLevelPosition.erase(m_pvLevelPosition.begin() + i);
					--i;
				}

		for (int i = 0; i < m_vAddOrbit.size(); ++i)
		{
			m_pOrbitReader->setFileIndex(m_vAddOrbit[i], m_vLevelData, m_fLocalTimeStart, m_fLocalTimeEnd);

			std::vector<std::pair<VertexBufferPtr, orbit::SPairLevel>> vTemperatureVertex(m_vLevelData.size());
			std::vector<orbit::SLevelCoord> vLevelPosition(m_vLevelData.size());

			for (int l = 0; l < m_vLevelData.size(); ++l)
			{
				VertexBufferPtr& pVertex = vTemperatureVertex[l].first;

				pVertex = GL::VertexBuffer::Create();
				pVertex->setUsage(GL_STATIC_DRAW);

				BufferBounder<VertexBuffer> temperatureBounder(pVertex);

				orbit::SPairLevel& levelData = m_vLevelData[l];
				unsigned& nPointBufferCount = vTemperatureVertex[l].second.nCount;

				if (m_displayMode == display::mode::temperature)
				{
					if (m_bIncludeAtmosphere)
					{
						vTemperatureVertex[l].second.vTemperature = levelData.vTemperature;
						nPointBufferCount = (unsigned)levelData.vTemperature.size();

						if (!pVertex->fillBuffer(nPointBufferCount * sizeof(float), levelData.vTemperature.data()))
						{
							toLog("Error pVertex->fillBuffer");
							return false;
						}
					}
					else
					{
						nPointBufferCount = (unsigned)levelData.vSerfaceTemperature.size();;

						if (!pVertex->fillBuffer(nPointBufferCount * sizeof(float), levelData.vSerfaceTemperature.data()))
						{
							toLog("Error pVertex->fillBuffer");
							return false;
						}
					}
				}
				else if (m_displayMode == display::mode::dust)
				{
					nPointBufferCount = (unsigned)levelData.vDust.size();

					if (!pVertex->fillBuffer(nPointBufferCount * sizeof(float), levelData.vDust.data()))
					{
						toLog("Error pVertex->fillBuffer");
						return false;
					}
				}
				else if (m_displayMode == display::mode::ice)
				{
					nPointBufferCount = (unsigned)levelData.vIce.size();

					if (!pVertex->fillBuffer(nPointBufferCount * sizeof(float), levelData.vIce.data()))
					{
						toLog("Error pVertex->fillBuffer");
						return false;
					}
				}

				vLevelPosition[l].fAltitudeMinMax = levelData.fAltitudeMinMax;
				vLevelPosition[l].fAltitudeStep = levelData.fAltitudeStep;
				vLevelPosition[l].fDistance_begin = levelData.fDistance_begin;
				vLevelPosition[l].fDistance_end = levelData.fDistance_end;
				vLevelPosition[l].fLatitude_begin = levelData.fLatitude_begin;
				vLevelPosition[l].fLatitude_end = levelData.fLatitude_end;
				vLevelPosition[l].fLongitude_begin = levelData.fLongitude_begin;
				vLevelPosition[l].fLongitude_end = levelData.fLongitude_end;
			}

			if (vTemperatureVertex.empty())
				continue;

			m_pvvTemperatureVertex.push_back({ m_vAddOrbit[i], vTemperatureVertex });

			ShaderStorageBufferPtr pLevelPosition = GL::ShaderStorageBuffer::Create(0);

			BufferBounder<ShaderStorageBuffer> ssboBounder(pLevelPosition);
			if (!pLevelPosition->fillBuffer(sizeof(orbit::SLevelCoord) * vLevelPosition.size(), vLevelPosition.data()))
			{
				toLog("ERROR pLevelPosition[i].second->fillBuffer");
				return false;
			}

			m_pvLevelPosition.push_back({ m_vAddOrbit[i], pLevelPosition });
		}

		return true;
	}

	void RenderOrbitTemperature::setFileArray(const std::vector<unsigned>& vOrbit_)
	{
		if (vOrbit_.empty())
			m_vLevelData.clear();

		checkAddRemoveOrbits(vOrbit_, m_vExistOrbit, m_vAddOrbit, m_vRemoveOrbit);

		m_bNeedFillLevelBufer = true;
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

	void RenderOrbitTemperature::setIncludeAtmosphere(bool bInclude_)
	{
		m_bIncludeAtmosphere = bInclude_;
		m_bNeedFillLevelBufer = true;
	}

	void RenderOrbitTemperature::setLocalTimeFilter(double fLocalTimeStart_, double fLocalTimeEnd_)
	{
		m_fLocalTimeStart = fLocalTimeStart_;
		m_fLocalTimeEnd = fLocalTimeEnd_;

		m_vRemoveOrbit = m_vExistOrbit;
		m_vAddOrbit = m_vExistOrbit;

		m_bNeedFillLevelBufer = true;
	}

	void RenderOrbitTemperature::changeDisplay(display::mode displayMode_)
	{
		m_displayMode = displayMode_;
		m_pPalette->changePalette((int)displayMode_);

		fillPalette((int)displayMode_);

		m_vRemoveOrbit = m_vExistOrbit;
		m_vAddOrbit = m_vExistOrbit;

		m_bNeedFillLevelBufer = true;
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