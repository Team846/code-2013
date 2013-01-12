#ifndef CONFIG_LOADER_DATA_H
#define CONFIG_LOADER_DATA_H
#include "ComponentData.h"

namespace data
{
/*!
 * @brief Namespace containing data pertaining to the configuration loader.
 * @author Raphael Chang
 */
namespace configLoader
{
class ConfigLoaderData
{
public:
	bool load;
	bool save;
	bool apply;

	ConfigLoaderData()
	{
		load = save = apply = false;
	}
};
}
}

#endif
