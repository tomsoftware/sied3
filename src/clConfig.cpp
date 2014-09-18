#include "clConfig.h"




clConfig::clConfig()
{
}


clConfig::~clConfig()
{
}


const char * clConfig::getPath(enum_config_value configType)
{
	switch (configType)
	{
		case clConfig::CONFIG_GFX_Path:
			return "D:\\Spiele\\Siedler3\\GFX\\";
		case clConfig::CONFIG_Map_Path:
			return "D:\\Spiele\\Siedler3\\Map\\";
		default:
			break;
	}

	m_error.AddError("(getPath) Unknown Config-Value-Type: %i", configType);
	return "";
}