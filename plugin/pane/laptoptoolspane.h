#ifndef LAPTOPTOOLSPANE_H
#define LAPTOPTOOLSPANE_H

#include <statuscenterpane.h>

namespace Ui {
class LaptopToolsPane;
}

class LaptopToolsPane : public StatusCenterPane
{
    Q_OBJECT

    enum ComboBoxIndexes{
        TotalCharge,
        ChargeRate,
        AppIndex
    };

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

    private slots:
        void on_comboBox_currentIndexChanged(int index);
        void on_pushButton_pressed();
};

#endif // LAPTOPTOOLSPANE_H
