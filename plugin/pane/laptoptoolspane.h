#ifndef LAPTOPTOOLSPANE_H
#define LAPTOPTOOLSPANE_H

#include <statuscenterpane.h>

namespace Ui {
class LaptopToolsPane;
}

class LaptopToolsPane : public StatusCenterPane
{
    Q_OBJECT

    // that's right, a parent of StatusCenterPane, NOT QWidget!!
public:
    explicit LaptopToolsPane();
    ~LaptopToolsPane();

    // StatusCenterPane interface stuff
    QString name();
    QString displayName();
    QIcon icon();
    QWidget *leftPane();

private:
    Ui::LaptopToolsPane *ui;
};

#endif // LAPTOPTOOLSPANE_H
