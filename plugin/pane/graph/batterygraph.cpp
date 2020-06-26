#include "batterygraph.h"
#include "ui_batterygraph.h"

// functionality
#include <QChart>
#include <QChartView>
#include <QPainter>
#include <QLineSeries>

#include <QDBusMessage>
#include <QDBusInterface>
#include <QDBusReply>

#include <QDesktopWidget>
#include <QDateTime>

#include <QDateTimeAxis>


//DBus Battery Info Structure
struct BatteryInfo {
    uint time, state;
    double value;
};
Q_DECLARE_METATYPE(BatteryInfo)

const QDBusArgument &operator<<(QDBusArgument &argument, const BatteryInfo &info) {
    argument.beginStructure();
    argument << info.time << info.value << info.state;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, BatteryInfo &info) {
    argument.beginStructure();
    argument >> info.time >> info.value >> info.state;
    argument.endStructure();
    return argument;
}

struct BatteryGraphPrivate{
    QtCharts::QChart batteryChart;
};

BatteryGraph::BatteryGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BatteryGraph)
{
    ui->setupUi(this);
    d = new BatteryGraphPrivate();

    //Set up battery chart
    d->batteryChart.setBackgroundVisible(false);
    d->batteryChart.legend()->hide();

    QtCharts::QChartView* batteryChartView = new QtCharts::QChartView(&d->batteryChart);
    batteryChartView->setRenderHint(QPainter::Antialiasing);
    ((QBoxLayout*) ui->graphArea->layout())->insertWidget(0, batteryChartView);

    updateBatteryChart();
}

BatteryGraph::~BatteryGraph()
{
    delete ui;
}

// show battery function

void BatteryGraph::updateBatteryChart() {
    //theshell void InfoPaneDropdown::updateBatteryChart()
    for (QtCharts::QAbstractAxis* axis : d->batteryChart.axes()) {
        d->batteryChart.removeAxis(axis);
        axis->deleteLater();
    }

    // fetch default battery
    QString batteryPath;
    QDBusInterface *i = new QDBusInterface("org.freedesktop.UPower",
                                           "/org/freedesktop/UPower",
                                           "org.freedesktop.UPower",
                                           QDBusConnection::systemBus(),
                                           this);
    QDBusReply< QList<QDBusObjectPath> > reply = i->call("EnumerateDevices"); //Get all devices
   if (reply.isValid()) { //Check if the reply is ok
       for (QDBusObjectPath device : reply.value()) {
           if (device.path().contains("battery")) {
               batteryPath = device.path();
           }
       }
   }

    // get history
    QDBusMessage historyMessage = QDBusMessage::createMethodCall("org.freedesktop.UPower",
                                                                 batteryPath,
                                                                 "org.freedesktop.UPower.Device",
                                                                 "GetHistory");
    QVariantList historyMessageArguments;
    historyMessageArguments.append("charge");
    historyMessageArguments.append((uint) 0); //Get surplus data so we can plot some data off the left of the graph
    historyMessageArguments.append((uint) 10000);
    historyMessage.setArguments(historyMessageArguments);

    QDBusReply<QDBusArgument> historyArgument = QDBusConnection::systemBus().call(historyMessage);

    // FIXME: what's an SC_DPI?
    float currentDPI = QApplication::desktop()->logicalDpiX();
    float dpiScaling = currentDPI / (float) 96;

    // draw the graph
    QtCharts::QLineSeries* batteryChartData = new QtCharts::QLineSeries;
    QPen dataPen;
    dataPen.setColor(this->palette().color(QPalette::Highlight));
    dataPen.setWidth(2 * dpiScaling);
    batteryChartData->setPen(dataPen);

    QtCharts::QLineSeries* batteryChartTimeRemainingData = new QtCharts::QLineSeries;
    batteryChartTimeRemainingData->setBrush(QBrush(this->palette().color(QPalette::Disabled, QPalette::WindowText)));

    QPen remainingTimePen;
    remainingTimePen.setColor(this->palette().color(QPalette::Disabled, QPalette::Highlight));
    remainingTimePen.setDashPattern(QVector<qreal>() << 3 << 3);
    remainingTimePen.setDashOffset(3);
    remainingTimePen.setWidth(2 * dpiScaling);
    batteryChartTimeRemainingData->setPen(remainingTimePen);

    int firstDateTime = QDateTime::currentSecsSinceEpoch() / 60;
    qint64 msecsSinceFull = -1;
    uint lastState = -1;
    bool takeNextSinceLastFull = false;
    if (historyArgument.isValid()) {
        QDBusArgument arrayArgument = historyArgument.value();
        arrayArgument.beginArray();
        while (!arrayArgument.atEnd()) {
            BatteryInfo info;
            arrayArgument >> info;

            qint64 msecs = info.time;
            msecs = msecs * 1000;

            if (info.value >= 90 && info.state == 2 && lastState == 1 && msecsSinceFull < msecs) {
                takeNextSinceLastFull = true;
            } else if (takeNextSinceLastFull) {
                takeNextSinceLastFull = false;
                msecsSinceFull = msecs;
            }
            lastState = info.state;

            if (info.value != 0 && info.state != 0) {
                batteryChartData->append(msecs, info.value);
                if (firstDateTime > info.time / 60) {
                    firstDateTime = info.time / 60;
                }
            }
        }
        arrayArgument.endArray();
    }
        batteryChartData->append(QDateTime::currentMSecsSinceEpoch(), batteryChartData->at(batteryChartData->count() - 1).y());


    d->batteryChart.removeAllSeries();
    d->batteryChart.addSeries(batteryChartData);

    QtCharts::QDateTimeAxis *xAxis = new QtCharts::QDateTimeAxis;
        xAxis->setMax(QDateTime::currentDateTime());
        xAxis->setMin(xAxis->max().addSecs(-43200)); //Half a day
    d->batteryChart.addAxis(xAxis, Qt::AlignBottom);
    xAxis->setLabelsColor(this->palette().color(QPalette::WindowText));
    xAxis->setFormat("hh:mm");
    xAxis->setTickCount(9);
    batteryChartData->attachAxis(xAxis);
}
