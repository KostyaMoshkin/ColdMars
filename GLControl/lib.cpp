#include "pch.h"

namespace lib
{
	std::vector<std::string> create_file_list(const char* sPath_)
	{
		std::vector<std::string> vsFileList;

		for (const auto& folder : std::filesystem::directory_iterator(sPath_))
			if (folder.is_directory())
				for (const auto& pedrFile : std::filesystem::directory_iterator(folder))
					vsFileList.push_back(folder.path().string() + "\\" + pedrFile.path().filename().string());
			else
				vsFileList.push_back(folder.path().string());

		return vsFileList;
	}


	void unpackColor(lib::iPoint3D  vColor256_, lib::fPoint3D& vColorRGB_)
	{
		vColorRGB_.x = 1.0f * (vColor256_.r) / 256;
		vColorRGB_.y = 1.0f * (vColor256_.g) / 256;
		vColorRGB_.z = 1.0f * (vColor256_.b) / 256;
	}

	void unpackColor(unsigned nColor256_, lib::fPoint3D& vColorRGB_)
	{
		vColorRGB_.x = 1.0f * ((nColor256_ & 0x00FF0000) >> 16 ) / 256;
		vColorRGB_.y = 1.0f * ((nColor256_ & 0x0000FF00) >> 8) / 256;
		vColorRGB_.z = 1.0f * (nColor256_ & 0x000000FF) / 256;
	}

	//------------------------------------------------------------------------------------------


}