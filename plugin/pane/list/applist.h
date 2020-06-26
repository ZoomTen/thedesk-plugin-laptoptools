#ifndef APPLIST_H
#define APPLIST_H

#include <QWidget>

namespace Ui {
class AppList;
}

class AppList : public QWidget
{
    Q_OBJECT

public:
    explicit AppList(QWidget *parent = nullptr);
    ~AppList();

private:
    Ui::AppList *ui;
};

#endif // APPLIST_H
