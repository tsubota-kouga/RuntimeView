#ifndef ___RuntimeView_H_
#define ___RuntimeView_H_

#include <QtWebEngineWidgets>

#include "BasilPlugin.hpp"
#include "Basilico.hpp"
class Basilico;

class RuntimeView: public BasilPlugin, public QWidget
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
    static int start_server_port;
    int server_port;

    QGridLayout layout;
    QWebEngineView viewer;
    QToolBar toolbar;

public:
    RuntimeView(Basilico* basil);

    void load_runtimeview(bool buffer_change=false);

    void load_view();

    void settingToolBar(Basilico* basil);

    static std::pair<RuntimeView*, String> factory(Basilico* basil, Array args);

    virtual void execute(Basilico* basil, Array args) override;

    virtual void keyPressedExecute(Basilico* basil) override;

    virtual void autocmdExecute(Basilico* basil, String autocmd) override;

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
