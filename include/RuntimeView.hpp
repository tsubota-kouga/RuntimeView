#ifndef ___RuntimeView_H_
#define ___RuntimeView_H_

#include <QtWebEngineWidgets>

#include <future>

#include "BasilPlugin.hpp"
#include "Basilico.hpp"
class Basilico;

class RuntimeView: public BasilPlugin, public QWebEngineView
{
    enum JobMode{
        Runtime,
        FileLoad
    };

    String nvim_port;
    String filetype;
    String absolute_path;

    bool allowed_reload;

    JobMode mode;
    static bool already_produced;
    static int port;
public:
    RuntimeView(Basilico* basil);

    void load_runtimeview(bool buffer_change=false);

    void load_view();

    static std::pair<RuntimeView*, String> factory(Basilico* basil, Array args);

protected:
    void execute(Basilico* basil, Array args) override;

    void keyPressedExecute(Basilico* basil) override;

private:
    bool wait_allowed_reload()
    {
        if(!allowed_reload)
        {
            allowed_reload = true;
            return false;
        }
        return true;
    }
};

#endif
