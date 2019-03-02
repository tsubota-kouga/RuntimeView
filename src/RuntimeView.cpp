#include "RuntimeView.hpp"

bool RuntimeView::already_produced = false;
int RuntimeView::port = 3210;

RuntimeView::RuntimeView(Basilico* basil)
    // :QWidget{},
     // layout{},
     // viewer{this},
{
    nvim_port = basil->neovim.port;
    filetype = boost::get<String>(basil->neovim.nvim_eval("&filetype"));

    // layout.addWidget(&(basil->neovim), 0, 0);
    // layout.addWidget(&viewer, 0, 1);
    // setLayout(&layout);

    auto dock = new QDockWidget("RuntimeView", basil);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea
            | Qt::RightDockWidgetArea
            | Qt::TopDockWidgetArea
            | Qt::BottomDockWidgetArea);
    setParent(dock);
    dock->setWidget(this);
    basil->addDockWidget(Qt::RightDockWidgetArea, dock);
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
        return std::make_pair(new RuntimeView{basil}, "dock");
    }
}

void RuntimeView::execute(Basilico* basil, Array args)
{
    auto arg1 = boost::get<String>(args.at(1));
    filetype = boost::get<String>(args.at(2));
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
}

void RuntimeView::keyPressedExecute(Basilico* basil)
{
    if(mode == Runtime and wait_allowed_reload())
    {
        load_runtimeview();
    }
}

void RuntimeView::load_runtimeview(bool buffer_change)
{
    if(buffer_change)
    {
        // viewer.load(QUrl(QString::fromStdString(
        //                 "http://localhost:3210?socket=" + nvim_port + "&lang=" + filetype +
        //                 "&file_change=true")));
        load(QUrl(QString::fromStdString(
                        "http://localhost:3210?socket=" + nvim_port + "&lang=" + filetype +
                        "&file_change=true")));
        allowed_reload = false;
    }
    else
    {
        // viewer.load(QUrl(QString::fromStdString(
        //                 "http://localhost:3210?socket=" + nvim_port + "&lang=" + filetype)));
        load(QUrl(QString::fromStdString(
                        "http://localhost:3210?socket=" + nvim_port + "&lang=" + filetype)));
    }
    // port += 5;
}

void RuntimeView::load_view()
{
    if(filetype == "html" or filetype == "xhtml")
    {
        // viewer.load(QUrl(QString::fromStdString("file://" + absolute_path)));
        load(QUrl(QString::fromStdString("file://" + absolute_path)));
    }
}
