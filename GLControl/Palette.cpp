#include "pch.h"

#include "Palette.h"

#pragma managed(push, off)

namespace GL {
	Palette::Palette()
	{
	}

	Palette::~Palette()
	{
	}

	bool Palette::init()
	{
		lib::XMLnodePtr xmlPaletteDefault = lib::XMLreader::getNode(getConfig(), Key::sPaletteDefault());

		//if (!!xmlPaletteDefault && !lib::XMLreader::getInt(xmlPaletteDefault, m_nActivePaletteID))
		//	m_nActivePaletteID = 1;

		//---------------------------------------------------------------------------------------

		lib::XMLnodePtr xmlPalette = lib::XMLreader::getNode(getConfig(), Key::sPalette());
		while (!!xmlPalette)
		{
			int nId = -1;
			if (!lib::XMLreader::getInt(xmlPalette, Key::sId(), nId))
			{
				toLog("Config should contain id attribut for: <Palette id=\"2\" interpolate=\"1600\">");
				return false;
			}

			m_vPaletteMap[nId] = xmlPalette;

			xmlPalette = xmlPalette->NextSibling(Key::sPalette());
		}

		//---------------------------------------------------------------------------------------

		if (!fillPalette(m_nActivePaletteID))
			return false;

		return true;
	}

	bool Palette::fillPalette(unsigned nPaletteID_)
	{
		m_vPalette.clear();

		lib::XMLnodePtr xmlActivePalette = nullptr;

		if (m_vPaletteMap.find(nPaletteID_) != m_vPaletteMap.end())
			xmlActivePalette = m_vPaletteMap[nPaletteID_];
		else
			xmlActivePalette = lib::XMLreader::getNode(getConfig(), Key::sPalette());

		lib::XMLnodePtr xmlColor = lib::XMLreader::getNode(xmlActivePalette, Key::sColor());
		while (!!xmlColor)
		{
			unsigned nColor = 0;
			int nTemperature = 0;

			if (lib::XMLreader::getInt(xmlColor, nColor) && lib::XMLreader::getInt(xmlColor, Key::sTemperature(), nTemperature))
				add(nTemperature, { nColor >> 16, (nColor & 0x00FF00) >> 8, nColor & 0x0000FF });

			xmlColor = xmlColor->NextSibling(Key::sColor());
		}

		if (!lib::XMLreader::getInt(xmlActivePalette, Key::sInterpolate(), m_nPaletteInterpolate))
		{
			toLog("Config should contain interpolate attribut for: <Palette id=\"2\" interpolate=\"1600\">");
			m_nPaletteInterpolate = 2;
		}

		m_nPaletteInterpolate = std::min<unsigned>(std::max<unsigned>(m_nPaletteInterpolate, 2), 8192);

		return true;
	}

	bool Palette::changePalette(int nPaletteIndex_)
	{
		m_nActivePaletteID = nPaletteIndex_;

		if (!fillPalette(m_nActivePaletteID))
			return false;

		return true;
	}

	void Palette::getPalette(std::vector<lib::iPoint3D>& vPalette_, int& nPaletteMin_, int& nPaletteMax_)
	{
		vPalette_.resize(m_nPaletteInterpolate);

		for (int i = 0; i < vPalette_.size(); ++i)
			vPalette_[i] = get(m_nMinValue + 1.0 * i / m_nPaletteInterpolate * (m_nMaxValue - m_nMinValue));

		nPaletteMin_ = m_nMinValue;
		nPaletteMax_ = m_nMaxValue;
	}

	void Palette::arrange()
	{
		std::sort(m_vPalette.begin(), m_vPalette.end(), [] (std::pair<int, lib::iPoint3D> p1, std::pair<int, lib::iPoint3D> p2) {return p1.first < p2.first;});

		m_nMinValue = m_vPalette[0].first;
		m_nMaxValue = m_vPalette[m_vPalette.size() - 1].first;
	}

	void Palette::add(int value_, lib::iPoint3D color_)
	{
		m_vPalette.push_back({value_, color_});
		arrange();
	}

	lib::iPoint3D Palette::get(double value_)
	{
		if (value_ <= m_nMinValue)
			return m_vPalette[0].second;

		if (value_ >= m_nMaxValue)
			return m_vPalette[m_vPalette.size() - 1].second;

		std::pair<int, lib::iPoint3D> paletteElementPrevious = m_vPalette[0];

		for (std::pair<int, lib::iPoint3D> paletteElement : m_vPalette)
		{
			if (value_ == paletteElement.first)
				return paletteElement.second;

			if (value_ > paletteElement.first)
			{
				paletteElementPrevious = paletteElement;
				continue;
			}

			int nPaletteValueDistace =  paletteElement.first - paletteElementPrevious.first;

			if (nPaletteValueDistace == 0)
				continue;

			double nValuDistance = value_ - paletteElementPrevious.first;

			double fRation = 1.0 / nPaletteValueDistace * nValuDistance;

			return lib::iPoint3D(paletteElementPrevious.second.r + (paletteElement.second.r - paletteElementPrevious.second.r) * fRation, 
				paletteElementPrevious.second.g + (paletteElement.second.g - paletteElementPrevious.second.g) * fRation, 
				paletteElementPrevious.second.b + (paletteElement.second.b - paletteElementPrevious.second.b) * fRation);
		}

		return lib::iPoint3D(0, 0, 0);
	}

	void Palette::getMinMax(float& fMin_, float& fMax_)
	{
		fMin_ = (float)m_nMinValue;
		fMax_ = (float)m_nMaxValue;
	}

	unsigned Palette::getInterpolate()
	{
		return m_nPaletteInterpolate;
	}

}

#pragma managed(pop)