#include "laptoptoolspane.h"
#include "ui_laptoptoolspane.h"

#include <tstackedwidget.h>

#include <QDateTime>

LaptopToolsPane::LaptopToolsPane() :
    StatusCenterPane(),
    ui(new Ui::LaptopToolsPane)
{
    ui->setupUi(this);

    // set transition
    ui->graphStage->setCurrentAnimation(tStackedWidget::SlideHorizontal);
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

// pane functionality
void LaptopToolsPane::on_comboBox_currentIndexChanged(int index)
{
    switch(index){
        case TotalCharge:
            if (ui->graphStage->currentIndex() != 0){ui->graphStage->setCurrentIndex(0);}
            break;
        case ChargeRate:
            if (ui->graphStage->currentIndex() != 0){ui->graphStage->setCurrentIndex(0);}
            break;
        case AppIndex:
            if (ui->graphStage->currentIndex() != 1){ui->graphStage->setCurrentIndex(1);}
            break;
        default:
            // do nothing
            break;
    }
}

void LaptopToolsPane::on_pushButton_pressed()
{
    ui->timeLabel->setText(tr("Updated %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
    ui->batterygraph->updateBatteryChart();
}

