#include "GraphicsPch.h"
#include "Factory/Factory.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<Factory> Factory::instance = nullptr;

	void Factory::DeleteFactory()
	{
		if (instance != nullptr)
		{
			SafeReset(instance)
		}
	}
}
