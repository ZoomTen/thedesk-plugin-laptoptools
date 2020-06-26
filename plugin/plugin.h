#ifndef PLUGIN_H
#define PLUGIN_H

// theDesk plugin interface
#include <plugininterface.h>

struct PluginPrivate;
class Plugin : public QObject, public PluginInterface {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "Plugin.json")
        Q_INTERFACES(PluginInterface)

    public:
        Plugin();
        ~Plugin();

    private:
        PluginPrivate* d;

        // PluginInterface interface
    public:
        void activate();
        void deactivate();
};

#endif // PLUGIN_H
