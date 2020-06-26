#include "laptoptoolspane.h"
#include "ui_laptoptoolspane.h"

LaptopToolsPane::LaptopToolsPane() :
    StatusCenterPane(),
    ui(new Ui::LaptopToolsPane)
{
    ui->setupUi(this);
}

LaptopToolsPane::~LaptopToolsPane()
{
    delete ui;
}

// StatusCenterPane interface stuff

QString LaptopToolsPane::name()
{
    // it's best to match this with our class name
    return "LaptopToolsPane";
}

QString LaptopToolsPane::displayName()
{
    // this is what appears in the left pane
    return tr("Laptop Tools");
}

QIcon LaptopToolsPane::icon()
{
    // the icon showing up in the left pane
    return QIcon::fromTheme("battery-full");
}

QWidget *LaptopToolsPane::leftPane()
{
    // we don't have a parent pane, so let's leave it null
    return nullptr;
}

