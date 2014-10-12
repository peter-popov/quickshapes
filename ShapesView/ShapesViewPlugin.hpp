#include <QQmlExtensionPlugin>

class ShapesViewPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ShapesView")

public:
    virtual void registerTypes(const char *uri);
};