#pragma once

namespace orbit {

	namespace Key {
		static const char* OrbitFileName()					{ return "OrbitFile"; }
		static const char* NptFileName()					{ return "NptFile"; }
		static const char* LevelFileName()					{ return "LevelFile"; }
		static const char* LevelBufferSize()				{ return "LevelBufferSize"; }
		static const char* OrbitDir()						{ return "OrbitDir"; }
		static const char* TemperartureAltitudeMax()		{ return "TemperartureAltitudeMax"; }
		static const char* TemperatureInterpolateCount()	{ return "TemperatureInterpolateCount"; }
	}
}

namespace GL {
	namespace Key {
		static const char* OrbitDir() { return "OrbitDir"; }

		static const char* sPalette()		{ return "Palette";		}
		static const char* sId()			{ return "id";			}
		static const char* Level()			{ return "Level";		}
		static const char* sColor()			{ return "Color";		}
		static const char* sInterpolate()	{ return "interpolate";	}

		static const char* Albedo()			{ return "Albedo";		}
		static const char* MarsTone()		{ return "MarsTone";	}
		static const char* NetColor()		{ return "NetColor";	}
		static const char* Digits()			{ return "Digits";		}
		static const char* DigitsColor()	{ return "DigitsColor";	}
		static const char* BaseHeight()		{ return "BaseHeight";	}

		static const char* ViewAngle()		{ return "ViewAngle";	}
	}
}

namespace megdr {
	namespace Key {
		static const char* sMegdr()				{ return "Megdr";			}
		static const char* sRadiusFile()		{ return "RadiusFile";		}
		static const char* sTopographyFile()	{ return "TopographyFile";	}
		static const char* nLines()				{ return "Lines";			}
		static const char* nLineSamples()		{ return "LineSamples";		}
		static const char* id()					{ return "id";				}
		static const char* sCount()				{ return "Count";			}
		static const char* line()				{ return "line";			}
		static const char* sample()				{ return "sample";			}
	}
}

namespace display {
	enum mode
	{
		temperature = 1,
		dust = 2,
		ice = 3,
	};
}

namespace longitude {
	enum mode
	{
		angle = 1,
		localtime = 2,
	};
}
