#include "RuntimeView.hpp"

bool RuntimeView::already_produced = false;
int RuntimeView::start_server_port = 3210;

RuntimeView::RuntimeView(Basilico* basil)
    :QWidget{},
     BasilPlugin{},
     layout{},
     viewer{this},
     toolbar{this}
{
    server_port = start_server_port;
    start_server_port += 5;
    nvim_port = basil->getNeoVim().port;
    filetype = boost::get<String>(basil->getNeoVim().nvim_eval("&filetype"));

    layout.setContentsMargins(0, 0, 0, 0);
    layout.addWidget(&toolbar, 0, 1, 1, 1);
    layout.addWidget(&viewer, 1, 1, 1, 1);
    setLayout(&layout);

    settingToolBar(basil);
}

void RuntimeView::load_runtimeview(bool buffer_change)
{
    if(buffer_change)
    {
        viewer.load(QUrl(QString::fromStdString(
                        "http://localhost:" + std::to_string(server_port) +
                        "?socket=" + nvim_port + "&lang=" + filetype +
                        "&file_change=true")));
        allowed_reload = false;
    }
    else
    {
        viewer.load(QUrl(QString::fromStdString(
                        "http://localhost:" + std::to_string(server_port) +
                        "?socket=" + nvim_port + "&lang=" + filetype)));
    }
}

void RuntimeView::load_view()
{
    if(filetype == "html" or filetype == "xhtml")
    {
        viewer.load(QUrl(QString::fromStdString("file://" + absolute_path)));
    }
}

void RuntimeView::settingToolBar(Basilico* basil)
{
    toolbar.addAction(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton),
                      "Quit",
                      [=]{
                      basil->killPlugin(this);
                      already_produced = false;
                      });
}

std::pair<RuntimeView*, String> RuntimeView::factory(Basilico* basil, Array args)
{
    if(RuntimeView::already_produced)
    {
        return std::make_pair(nullptr, "");
    }
    else
    {
        RuntimeView::already_produced = true;
        return std::make_pair(new RuntimeView{basil}, "split");
    }
}

void RuntimeView::execute(Basilico* basil, Array args)
{
    auto&& arg1 = boost::get<String>(args.at(1));
    if(arg1 == "Reload")
    {
        mode = Runtime;
        load_runtimeview(true);
    }
    else if(arg1 == "LoadFile")
    {
        mode = FileLoad;
        absolute_path = boost::get<String>(args.at(3));
        load_view();
    }
    else if(arg1 == "Finish")
    {
        basil->killPlugin(this);
        already_produced = false;
    }
}

void RuntimeView::keyPressedExecute(Basilico* basil)
{
    if(mode == Runtime and wait_allowed_reload())
    {
        load_runtimeview();
    }
}

void RuntimeView::autocmdExecute(Basilico* basil, String autocmd)
{
    if(autocmd == "TabEnter")
    {
        load_runtimeview(true);
    }
}

