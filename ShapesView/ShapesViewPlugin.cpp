#include "ShapesViewPlugin.hpp"
#include "ShapefileView.hpp"
#include <qqml.h>
#include <iostream>

void ShapesViewPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<ShapefileView>(uri, 1, 0, "ShapefileView");
}