#include "pch.h"

#include "RenderMegdr.h"
#include "BufferBounder.h"

#include "lib.h"
#include "Vocabulary.h"

#include "MegdrReader.h"
#include "XML\XMLreader.h"
#include "LOG\logger.h"

#include <memory>

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

		//-------------------------------------------------------------------------------------

		m_pAlbedoTexture = GL::TextureBuffer::Create(GL_TEXTURE_2D, GL_TEXTURE1, GL_LINEAR);
		m_pAlbedoTexture->alignment(1);

		int nPalette = 1;
		m_pMegdrProgram->setUniform1i("m_tAlbedo", &nPalette);

		//-------------------------------------------------------------------------------------

		m_pRadiusVertex = GL::VertexBuffer::Create();
		m_pRadiusVertex->setUsage(GL_STATIC_DRAW);

		//-------------------------------------------------------------------------------------------------

		m_pTopographyVertex = GL::VertexBuffer::Create();
		m_pTopographyVertex->setUsage(GL_STATIC_DRAW);

		//-------------------------------------------------------------------------------------------------

		renderBounder.unbound();

		//-------------------------------------------------------------------------------------------------

		setScale();

		//  Координаты вершин
		m_pMegdr = megdr::MegdrReader::Create();

		m_pMegdr->setConfig(getConfig());

		if (!m_pMegdr->init())
			return false;;

		if (!fillVertex())
			return false;

		//-------------------------------------------------------------------------------------------------

		unsigned nMarsTone;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), MarsTone()), nMarsTone))
			nMarsTone = 0x00c36b15;

		std::string sAlbedoFile;
		if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Albedo()), sAlbedoFile))
			return false;

		if ( !fillAlbedo(sAlbedoFile.c_str(), nMarsTone) )
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
		BufferBounder<IndexBuffer> indexBounder(m_pIndex);
		BufferBounder<IndirectBuffer> indirectBounder(m_pIndirect);

		if (getVersionGl() >= 43)
			glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, nullptr, (GLsizei)m_pMegdr->getLinesCount() - 1, 0);
		else
			for (int i = 0; i < (GLsizei)m_pMegdr->getLinesCount() - 1; ++i)
				glDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, (void *)size_t(i * m_pMegdr->getIndirectCommandSize()));

		renderBounder.unbound();
	}

	bool RenderMegdr::keyPress(GL::EKeyPress nKey_)
	{
		if ((nKey_ == GL::EKeyPress::key_1) || (nKey_ == GL::EKeyPress::key_2) )
		{
			m_fScale = (nKey_ == GL::EKeyPress::key_1) ? m_fScale /= 1.2f : m_fScale *= 1.2f;
			setScale();
		}
		else if ((nKey_ == GL::EKeyPress::key_5) || (nKey_ == GL::EKeyPress::key_6))
		{
			if (!m_pMegdr->changeMedgr(nKey_ == GL::EKeyPress::key_6))
				return false;

			if (!fillVertex())
				return false;
		}

		Sleep(300);

		return true;
	}

	bool RenderMegdr::fillAlbedo(const char *sFileName_, unsigned nMarsTone_)
	{
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		BufferBounder<TextureBuffer> albedoTextureBounder(m_pAlbedoTexture);

		lib::fPoint3D vColor;
		lib::unpackColor(nMarsTone_, vColor);

		m_pMegdrProgram->setUniformVecf("m_vMarsTone", &vColor.x);

		//-----------------------------------------------------------------------------------

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

		//-----------------------------------------------------------------------------------

		return true;
	}

	bool RenderMegdr::fillVertex()
	{
		BufferBounder<ShaderProgram> programBounder(m_pMegdrProgram);
		BufferBounder<RenderMegdr> renderBounder(this);

		int nLines = m_pMegdr->getLinesCount();
		int nLineSamples = m_pMegdr->getLineSamplesCount();

		m_pMegdrProgram->setUniform1i("m_nLines", &nLines);
		m_pMegdrProgram->setUniform1i("m_nLineSamples", &nLineSamples);

		int nBaseHeight;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), BaseHeight()), nBaseHeight))
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

}

#pragma managed(pop)