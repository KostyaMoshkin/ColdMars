#include "pch.h"

#include "XMLreader.h"

namespace lib {

	XMLreader::XMLreader(const char* sXMLpath_)
	{
		m_xDOC = TiXmlDocument();

		if (!m_xDOC.LoadFile(sXMLpath_, TIXML_ENCODING_UNKNOWN))
			return;

		m_xRoot = m_xDOC.RootElement();
		m_xCurrentNode = m_xRoot;
	}
	XMLreader::~XMLreader()
	{
	}

	XMLnodePtr XMLreader::getRoot()
	{
		return m_xRoot;
	}

	bool XMLreader::changeCurrentNode(const char* sNode_)
	{
		if (!sNode_)
			m_xCurrentNode = m_xRoot;
		else
		{
			if (!m_xCurrentNode)
				return false;

			m_xCurrentNode = m_xCurrentNode->FirstChild(sNode_);
		}

		return !!m_xCurrentNode;
	}

	XMLnodePtr XMLreader::getCurrentNode()
	{
		return m_xCurrentNode;
	}

	bool XMLreader::getNextCurrentNode()
	{
		if (!m_xCurrentNode)
			return false;

		m_xCurrentNode = m_xCurrentNode->NextSiblingElement();
		return !m_xCurrentNode;
	}

	bool XMLreader::getFloat(XMLnodePtr xNode_, float& nValue_)
	{
		if (!xNode_)
			return false;

		std::string sValue = std::string(xNode_->FirstChild()->Value());

		if (sValue.empty())
			return false;

		try
		{
			nValue_ = (float)std::stof(sValue);
		}
		catch (const std::exception&)
		{
			return false;
		}

		return true;
	}

	bool XMLreader::getFloat(XMLnodePtr xNode_, const char* sNode_, float& nValue_)
	{
		if (!xNode_)
			return false;

		auto attribute = xNode_->ToElement()->Attribute(sNode_);
		if (!attribute)
		{
			toLog("WARNING Expect attribute: " + std::string(sNode_) + " in node " + std::string(xNode_->Value()));
			return false;
		}

		std::string sValue = attribute;

		if (sValue.empty())
			return false;

		try
		{
			nValue_ = (float)std::stod(sValue);
		}
		catch (const std::exception&)
		{
			return false;
		}

		return true;
	}

	bool XMLreader::getInt(XMLnodePtr xNode_, int& nValue_)
	{
		if (!xNode_)
			return false;

		std::string sValue = std::string(xNode_->FirstChild()->Value());

		if (sValue.empty())
			return false;

		int nBase = 10;

		if (sValue.find('x') != std::string::npos)
			nBase = 16;

		try
		{
			nValue_ = (int)std::stol(sValue, 0, nBase);
		}
		catch (const std::exception&)
		{
			return false;
		}

		return true;
	}

	bool XMLreader::getInt(XMLnodePtr xNode_, unsigned& nValue_)
	{
		if (!xNode_)
			return false;

		std::string sValue = std::string(xNode_->FirstChild()->Value());

		if (sValue.empty())
			return false;

		int nBase = 10;

		if (sValue.find('x') != std::string::npos)
			nBase = 16;

		try
		{
			nValue_ = (unsigned)std::stoul(sValue, 0, nBase);
		}
		catch (const std::exception&)
		{
			return false;
		}

		return true;
	}

	bool XMLreader::getInt(XMLnodePtr xNode_, const char* sNode_, unsigned& nValue_)
	{
		if (!xNode_)
			return false;

		auto attribute = xNode_->ToElement()->Attribute(sNode_);
		if (!attribute)
		{
			toLog("WARNING Expect attribute: " + std::string(sNode_) + " in node " + std::string(xNode_->Value()));
			return false;
		}

		std::string sValue = attribute;

		if (sValue.empty())
			return false;

		int nBase = 10;

		if (sValue.find('x') != std::string::npos)
			nBase = 16;

		try
		{
			nValue_ = (unsigned)std::stol(sValue, 0, nBase);
		}
		catch (const std::exception&)
		{
			return false;
		}

		return true;
	}

	bool XMLreader::getInt(XMLnodePtr xNode_, const char* sNode_, int& nValue_)
	{
		if (!xNode_)
			return false;

		auto attribute = xNode_->ToElement()->Attribute(sNode_);
		if (!attribute)
		{
			toLog("WARNING Expect attribute: " + std::string(sNode_) + " in node " + std::string(xNode_->Value()));
			return false;
		}

		std::string sValue = attribute;

		if (sValue.empty())
			return false;

		int nBase = 10;

		if (sValue.find('x') != std::string::npos)
			nBase = 16;

		try
		{
			nValue_ = (int)std::stol(sValue, 0, nBase);
		}
		catch (const std::exception&)
		{
			return false;
		}

		return true;
	}

	bool XMLreader::getSting(XMLnodePtr xNode_, std::string& sValue_)
	{
		const char* sValue = xNode_->FirstChild()->Value();

		if (!sValue)
			return false;

		sValue_ = std::string(xNode_->FirstChild()->Value());

		return true;
	}

	XMLnodePtr XMLreader::getNode(XMLnodePtr xNode_, const char* sNode_)
	{
		if (!xNode_)
			return nullptr;

		return xNode_->FirstChild(sNode_);
	}
}
