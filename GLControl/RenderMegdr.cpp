#include "pch.h"

#include "RenderMegdr.h"
#include "BufferBounder.h"

#include "MegdrReader.h"

#pragma managed(push, off)

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

namespace GL {

	RenderMegdr::RenderMegdr()
	{
	}

	void RenderMegdr::setScale()
	{
		lib::limit(m_fScale, 0.1f, 30.0f);

		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		m_pMegdrProgram->setUniform1f("m_fScale", &m_fScale);
	}

	void RenderMegdr::setViewAngle(lib::Matrix4& mPerspective_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		m_pMegdrProgram->setUniformMat4f("m_mPerspective", &mPerspective_[0][0]);
	}

	void RenderMegdr::lookAt(lib::Matrix4& mView_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		m_pMegdrProgram->setUniformMat4f("m_mView", &mView_[0][0]);
	}

	void RenderMegdr::rotate(lib::Matrix4& mRotate_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		m_pMegdrProgram->setUniformMat4f("m_mRotate", &mRotate_[0][0]);
	}

	void RenderMegdr::translate(lib::Matrix4& mTranslate_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		m_pMegdrProgram->setUniformMat4f("m_mTranslate", &mTranslate_[0][0]);
	}

	bool RenderMegdr::init()
	{
		m_pIndex = GL::IndexBuffer::Create();
		m_pIndirect = GL::IndirectBuffer::Create();

		ShaderProgramPtr pMegdrProgram = ShaderProgram::Create();

		pMegdrProgram->addShader(ShaderName::megdr_vertex, ShaderProgram::ShaderType::Vertex());
		pMegdrProgram->addShader(ShaderName::megdr_fragment, ShaderProgram::ShaderType::Fragment());

		bool bProgramCompile = pMegdrProgram->init();
		if (!bProgramCompile)
			return false;

		m_pMegdrProgram = pMegdrProgram;

		glGenVertexArrays(1, &m_nVAO);

		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		BufferBounder<RenderMegdr> renderBounder(this);

		//-------------------------------------------------------------------------------------------------

		m_pAlbedoTexture = GL::TextureBuffer::Create(GL_TEXTURE_2D, GL_TEXTURE1, GL_LINEAR);
		m_pAlbedoTexture->alignment(1);

		int nPalette = 1;
		m_pMegdrProgram->setUniform1i("m_tAlbedo", &nPalette);

		//-------------------------------------------------------------------------------------------------

		m_pSSBO = GL::ShaderStorageBuffer::Create(0);
		BufferBounder<ShaderStorageBuffer> ssboBounder(m_pSSBO);

		if (!m_pSSBO->bookSpace(sizeof(lib::fPoint3D)))
		{
			toLog("ERROR m_pSSBO->bookSpace");
			return false;
		}

		lib::fPoint3D surfaceCoord = lib::fPoint3D(9999, 9999, 9999);
		if (!m_pSSBO->fillBuffer(sizeof(lib::fPoint3D), &surfaceCoord.x))
			return false;

		//-------------------------------------------------------------------------------------------------

		m_pDigitsTexture = GL::TextureBuffer::Create(GL_TEXTURE_2D, GL_TEXTURE2, GL_NEAREST);
		m_pDigitsTexture->alignment(1);

		int nDigit = 2;
		m_pMegdrProgram->setUniform1i("m_tDigit", &nDigit);

		//-------------------------------------------------------------------------------------------------

		m_pRadiusVertex = GL::VertexBuffer::Create();
		m_pRadiusVertex->setUsage(GL_STATIC_DRAW);

		//-------------------------------------------------------------------------------------------------

		m_pTopographyVertex = GL::VertexBuffer::Create();
		m_pTopographyVertex->setUsage(GL_STATIC_DRAW);

		//-------------------------------------------------------------------------------------------------

		unsigned nNetColor;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), Key::NetColor()), nNetColor))
			nNetColor = 0xFFFFFFFF;

		lib::fPoint3D vNetColor;
		lib::unpackColor(nNetColor, vNetColor);

		m_pMegdrProgram->setUniformVecf("m_vNetColor", &vNetColor.x);

		//-------------------------------------------------------------------------------------------------

		setScale();

		sizeChanged(2048, 2048);

		//-------------------------------------------------------------------------------------------------

		renderBounder.unbound();

		//-------------------------------------------------------------------------------------------------

		//  Координаты вершин
		m_pMegdr = megdr::MegdrReader::Create();

		m_pMegdr->setConfig(getConfig());

		if (!m_pMegdr->init())
			return false;;

		if (!fillMegdrVertex())
			return false;

		//-------------------------------------------------------------------------------------------------

		std::string sAlbedoFile;
		if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Key::Albedo()), sAlbedoFile))
			return false;

		unsigned nMarsTone;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), Key::MarsTone()), nMarsTone))
			nMarsTone = 0x00c36b15;

		if ( !fillAlbedo(sAlbedoFile.c_str(), nMarsTone) )
			return false;

		//-------------------------------------------------------------------------------------------------

		std::string sDigitFile;
		if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Key::Digits()), sDigitFile))
			return false;

		unsigned nDigitsColor;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), Key::DigitsColor()), nDigitsColor))
			nMarsTone = 0x00c36b15;

		if ( !fillDigit(sDigitFile.c_str(), nDigitsColor) )
			return false;

		//-------------------------------------------------------------------------------------------------

		setVisible(true);
		return true;
	}

	void RenderMegdr::draw()
	{
		if (!isVisible())
			return;

		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		BufferBounder<RenderMegdr> renderBounder(this);
		BufferBounder<VertexBuffer> radiusBounder(m_pRadiusVertex);
		BufferBounder<VertexBuffer> areoidBounder(m_pTopographyVertex);
		BufferBounder<TextureBuffer> albedoTextureBounder(m_pAlbedoTexture);
		BufferBounder<TextureBuffer> digitTextureBounder(m_pDigitsTexture);
		BufferBounder<IndexBuffer> indexBounder(m_pIndex);
		BufferBounder<IndirectBuffer> indirectBounder(m_pIndirect);
		BufferBounder<ShaderStorageBuffer> ssboBounder(m_pSSBO);

		if (getVersionGl() >= 43)
			glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, nullptr, (GLsizei)m_pMegdr->getLinesCount() - 1, 0);
		else
			for (int i = 0; i < (GLsizei)m_pMegdr->getLinesCount() - 1; ++i)
				glDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, (void *)size_t(i * m_pMegdr->getIndirectCommandSize()));

		renderBounder.unbound();
	}

	bool RenderMegdr::fillAlbedo(const char *sFileName_, unsigned nMarsTone_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		BufferBounder<TextureBuffer> albedoTextureBounder(m_pAlbedoTexture);

		lib::fPoint3D vColor;
		lib::unpackColor(nMarsTone_, vColor);

		m_pMegdrProgram->setUniformVecf("m_vMarsTone", &vColor.x);

		//-------------------------------------------------------------------------------------------------

		int nWidth;
		int nHeight;
		int nCompress;
		stbi_uc* pImage = stbi_load(sFileName_, &nWidth, &nHeight, &nCompress, 1);

		if (pImage == NULL) {
			toLog("ERROR open image file: " + std::string(sFileName_));
			return false;
		}

		if (!m_pAlbedoTexture->fillBuffer2D(GL_RED, nWidth, nHeight, GL_RED, GL_UNSIGNED_BYTE, pImage))
		{
			toLog("ERROR load albedo into GPU");
			return false;
		}

		stbi_image_free(pImage);

		//-------------------------------------------------------------------------------------------------

		return true;
	}

	bool RenderMegdr::fillDigit(const char *sFileName_, unsigned nDigitColor_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		BufferBounder<TextureBuffer> digitTextureBounder(m_pDigitsTexture);

		lib::fPoint3D vColor;
		lib::unpackColor(nDigitColor_, vColor);

		m_pMegdrProgram->setUniformVecf("m_vDigitColor", &vColor.x);

		//-------------------------------------------------------------------------------------------------

		int nWidth;
		int nHeight;
		int nCompress;
		stbi_uc* pImage = stbi_load(sFileName_, &nWidth, &nHeight, &nCompress, 1);

		if (pImage == NULL) {
			toLog("ERROR open image file: " + std::string(sFileName_));
			return false;
		}

		if (!m_pDigitsTexture->fillBuffer2D(GL_RED, nWidth, nHeight, GL_RED, GL_UNSIGNED_BYTE, pImage))
		{
			toLog("ERROR load digit into GPU");
			return false;
		}

		stbi_image_free(pImage);

		//-------------------------------------------------------------------------------------------------

		return true;
	}

	bool RenderMegdr::fillMegdrVertex()
	{
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		BufferBounder<RenderMegdr> renderBounder(this);

		int nLines = m_pMegdr->getLinesCount();
		int nLineSamples = m_pMegdr->getLineSamplesCount();

		m_pMegdrProgram->setUniform1i("m_nLines", &nLines);
		m_pMegdrProgram->setUniform1i("m_nLineSamples", &nLineSamples);

		int nBaseHeight;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), Key::BaseHeight()), nBaseHeight))
			nBaseHeight = 3396000;

		m_pMegdrProgram->setUniform1i("m_nBaseHeight", &nBaseHeight);

		//-------------------------------------------------------------------------------------------------

		BufferBounder<VertexBuffer> radiusBounder(m_pRadiusVertex);

		if (!m_pRadiusVertex->fillBuffer(m_pMegdr->getRadiusSize(), m_pMegdr->getRadius()))
		{
			toLog("Error m_pRadiusVertex->fillBuffer()");
			return false;
		}

		m_pRadiusVertex->attribIPointer(0, 1, GL_SHORT, 0, 0);

		BufferBounder<VertexBuffer> topographyBounder(m_pTopographyVertex);

		if (!m_pTopographyVertex->fillBuffer(m_pMegdr->getTopographySize(), m_pMegdr->getTopography()))
		{
			toLog("Error m_pTopographyVertex->fillBuffer()");
			return false;
		}

		m_pTopographyVertex->attribIPointer(1, 1, GL_SHORT, 0, 0);

		//-------------------------------------------------------------------------------------------------

		BufferBounder<IndexBuffer> indexBounder(m_pIndex);

		if (!m_pIndex->fillBuffer(m_pMegdr->getIndecesSize(), m_pMegdr->getIndeces()))
		{
			toLog("Error m_pIndex->fillBuffer()");
			return false;
		}

		BufferBounder<IndirectBuffer> indirectBounder(m_pIndirect);

		if (!m_pIndirect->fillBuffer(m_pMegdr->getIndirectSize(), m_pMegdr->getIndirect()))
		{
			toLog("Error m_pIndirect->fillBuffer()");
			return false;
		}

		renderBounder.unbound();

		return true;
	}

	void RenderMegdr::bound()
	{
		glBindVertexArray(m_nVAO);
	}

	void RenderMegdr::unbound()
	{
		glBindVertexArray(0);
	}

	float RenderMegdr::getScale()
	{
		return m_fScale;
	}

	void RenderMegdr::setScale(float fScale_)
	{
		m_fScale = fScale_;
		setScale();
	}

	void RenderMegdr::sizeChanged(int nWidth_, int nHeight_)
	{
		m_nWindowHeight = nHeight_;
	}

	void RenderMegdr::on_mouse_click(int nPosX_, int nPosY_)
	{
		BufferBounder<RenderMegdr> renderBounder(this);
		BufferBounder<ShaderStorageBuffer> ssboBounder(m_pSSBO);
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);

		lib::fPoint2D vWindowClick = lib::fPoint2D((float)nPosX_, (float)m_nWindowHeight - nPosY_);
		m_pMegdrProgram->setUniformVec2f("m_vWindowClick", &vWindowClick.x);
	}

	lib::fPoint2D RenderMegdr::getClickCoords()
	{
		BufferBounder<RenderMegdr> renderBounder(this);
		BufferBounder<ShaderStorageBuffer> ssboBounder(m_pSSBO);
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);

		lib::fPoint2D clickCoord = lib::fPoint2D(0, 0);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(lib::fPoint2D), &clickCoord.x);

		lib::fPoint3D surfaceCoord = lib::fPoint3D(9999, 9999, 9999);
		if (!m_pSSBO->fillBuffer(sizeof(lib::fPoint3D), &surfaceCoord.x))
			return lib::fPoint2D(9999, 9999);
		
		return clickCoord;
	}

}

#pragma managed(pop)