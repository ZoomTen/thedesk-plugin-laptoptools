#include "applist.h"
#include "ui_applist.h"

AppList::AppList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppList)
{
    ui->setupUi(this);
}

AppList::~AppList()
{
    delete ui;
}
