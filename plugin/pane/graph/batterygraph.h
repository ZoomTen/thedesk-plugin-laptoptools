#ifndef BATTERYGRAPH_H
#define BATTERYGRAPH_H

#include <QWidget>

namespace Ui {
class BatteryGraph;
}

struct BatteryGraphPrivate;
class BatteryGraph : public QWidget
{
    Q_OBJECT

public:
    explicit BatteryGraph(QWidget *parent = nullptr);
    ~BatteryGraph();
    void updateBatteryChart();

private:
    Ui::BatteryGraph *ui;
    BatteryGraphPrivate *d;

};

#endif // BATTERYGRAPH_H
