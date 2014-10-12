#include <qshapeview/ShapesViewPlugin.hpp>
#include <qshapeview/ShapefileView.hpp>
#include <qqml.h>

void ShapesViewPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<ShapefileView>(uri, 1, 0, "ShapefileView");
}