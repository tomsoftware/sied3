#ifndef CLCONFIG_H
#define CLCONFIG_H

#include "clError.h"


class clConfig
{
private:

	static clError m_error;

public:
	enum enum_config_value
	{
		CONFIG_GFX_Path,
		CONFIG_Map_Path
	};

public:
	clConfig();
	~clConfig();
	static const char * clConfig::getPath(enum_config_value configType);
};


__declspec(selectany)
clError clConfig::m_error = clError("clConfig");

#endif