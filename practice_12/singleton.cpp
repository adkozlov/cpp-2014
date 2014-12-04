#include "singleton.h"
#include <iostream>

std::weak_ptr<singleton> singleton::instance_ = std::weak_ptr<singleton>(std::shared_ptr<singleton>(new singleton()));

std::shared_ptr<singleton> singleton::get_instance()
{
	if (!instance_.expired())
	{
		return instance_.lock();
	} else {	
		instance_.reset();

		std::shared_ptr<singleton> result(new singleton());
		instance_ = std::weak_ptr<singleton>(result);

		return result;
	}
}