#pragma once
#include "CConfig.h"

namespace GL {
	
	class Palette;
	using PalettePtr = std::shared_ptr<Palette>;

	class Palette : public lib::CConfig
	{
		std::vector<std::pair<float, lib::iPoint3D>> m_vPalette;

		float m_fMinValue = 0;
		float m_fMaxValue = 0;

		unsigned m_nActivePaletteID = 1;
		unsigned m_nPaletteInterpolate = 16;

		std::map<unsigned, lib::XMLnodePtr> m_vPaletteMap;

	public:
		Palette();
		~Palette();

		static PalettePtr Create() { return std::make_shared<Palette>(); }


		bool init();

	protected:
		void arrange();

	public:
		void add(float value_, lib::iPoint3D color_);
		lib::iPoint3D get(double value_);
		void getMinMax(float& fMin_, float& fMax_);
		unsigned getInterpolate();
		bool fillPalette(unsigned nPaletteID_);
		bool changePalette(int nPaletteIndex_);
		void getPalette(std::vector<lib::iPoint3D>& vPalette_, float& fPaletteMin_, float& fPaletteMax_);
	};
}

