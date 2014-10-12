#include "ShapesViewPlugin.hpp"
#include "ShapefileView.hpp"
#include <qqml.h>
#include <iostream>

void ShapesViewPlugin::registerTypes(const char *uri)
{
	std::cout << "registering " << uri << std::endl;
    qmlRegisterType<ShapefileView>(uri, 1, 0, "ShapefileView");
}