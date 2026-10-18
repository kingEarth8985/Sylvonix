#pragma once
#include <string>

namespace SylvonixRenderer
{
	struct WindowDescriptor
	{
		std::string title;
		int width;
		int height;
	};

	struct ApplicationDescriptor
	{
		std::string name;
		std::string version;

		WindowDescriptor windowDescriptor;
	};
}