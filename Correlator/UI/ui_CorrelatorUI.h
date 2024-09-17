/********************************************************************************
** Form generated from reading ui file 'CorrelatorUI.ui'
**
** Created: Wed Jul 2 12:09:34 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CORRELATORUI_H
#define UI_CORRELATORUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "qwt_plot.h"

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionCount_Rate;
    QAction *actionConfigure;
    QAction *actionClear;
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *taRunTab;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QComboBox *SerialPort;
    QLabel *label_5;
    QSpinBox *Wavelength;
    QLabel *label;
    QSpinBox *SampleTime;
    QLabel *label_3;
    QSpinBox *RunTime;
    QwtPlot *Plot;
    QwtPlot *SemiLogPlot;
    QWidget *layoutWidget1;
    QHBoxLayout *hboxLayout;
    QLabel *label_4;
    QLCDNumber *CountRate;
    QWidget *widget;
    QGridLayout *gridLayout1;
    QLabel *label_6;
    QSpinBox *NumberBaselineChannels;
    QLabel *label_7;
    QLabel *Baseline;
    QWidget *widget1;
    QHBoxLayout *hboxLayout1;
    QLabel *label_8;
    QLabel *Slope;
    QWidget *tab_2;
    QWidget *TraceTab;
    QListWidget *Trace;
    QPushButton *pushButton;
    QWidget *ResultsTab;
    QTextEdit *Results;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menuAction;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(1272, 942);
    actionExit = new QAction(MainWindow);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    actionExit->setIcon(QIcon(QString::fromUtf8(":/Icons/icons/exit.png")));
    actionStart = new QAction(MainWindow);
    actionStart->setObjectName(QString::fromUtf8("actionStart"));
    actionStart->setIcon(QIcon(QString::fromUtf8(":/Icons/icons/start.png")));
    actionStop = new QAction(MainWindow);
    actionStop->setObjectName(QString::fromUtf8("actionStop"));
    actionStop->setIcon(QIcon(QString::fromUtf8(":/Icons/icons/stop.png")));
    actionCount_Rate = new QAction(MainWindow);
    actionCount_Rate->setObjectName(QString::fromUtf8("actionCount_Rate"));
    actionCount_Rate->setIcon(QIcon(QString::fromUtf8(":/Icons/icons/Float.png")));
    actionConfigure = new QAction(MainWindow);
    actionConfigure->setObjectName(QString::fromUtf8("actionConfigure"));
    actionConfigure->setIcon(QIcon(QString::fromUtf8(":/Icons/icons/configure.png")));
    actionClear = new QAction(MainWindow);
    actionClear->setObjectName(QString::fromUtf8("actionClear"));
    actionClear->setIcon(QIcon(QString::fromUtf8(":/Icons/icons/filenew.png")));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    tabWidget = new QTabWidget(centralwidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setGeometry(QRect(20, 30, 1211, 811));
    taRunTab = new QWidget();
    taRunTab->setObjectName(QString::fromUtf8("taRunTab"));
    groupBox = new QGroupBox(taRunTab);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(20, 570, 631, 201));
    layoutWidget = new QWidget(groupBox);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(40, 40, 571, 151));
    gridLayout = new QGridLayout(layoutWidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    label_2 = new QLabel(layoutWidget);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 0, 0, 1, 1);

    SerialPort = new QComboBox(layoutWidget);
    SerialPort->setObjectName(QString::fromUtf8("SerialPort"));

    gridLayout->addWidget(SerialPort, 0, 1, 1, 1);

    label_5 = new QLabel(layoutWidget);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout->addWidget(label_5, 0, 2, 1, 1);

    Wavelength = new QSpinBox(layoutWidget);
    Wavelength->setObjectName(QString::fromUtf8("Wavelength"));
    Wavelength->setMinimum(400);
    Wavelength->setMaximum(1000);

    gridLayout->addWidget(Wavelength, 0, 3, 1, 1);

    label = new QLabel(layoutWidget);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 1, 0, 1, 1);

    SampleTime = new QSpinBox(layoutWidget);
    SampleTime->setObjectName(QString::fromUtf8("SampleTime"));
    SampleTime->setMinimum(4);
    SampleTime->setMaximum(1000);

    gridLayout->addWidget(SampleTime, 1, 1, 1, 1);

    label_3 = new QLabel(layoutWidget);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 2, 0, 1, 1);

    RunTime = new QSpinBox(layoutWidget);
    RunTime->setObjectName(QString::fromUtf8("RunTime"));
    RunTime->setMinimum(1);
    RunTime->setMaximum(600);

    gridLayout->addWidget(RunTime, 2, 1, 1, 1);

    Plot = new QwtPlot(taRunTab);
    Plot->setObjectName(QString::fromUtf8("Plot"));
    Plot->setGeometry(QRect(20, 20, 1141, 301));
    SemiLogPlot = new QwtPlot(taRunTab);
    SemiLogPlot->setObjectName(QString::fromUtf8("SemiLogPlot"));
    SemiLogPlot->setGeometry(QRect(40, 340, 571, 200));
    layoutWidget1 = new QWidget(taRunTab);
    layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
    layoutWidget1->setGeometry(QRect(710, 580, 471, 44));
    hboxLayout = new QHBoxLayout(layoutWidget1);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    label_4 = new QLabel(layoutWidget1);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    QFont font;
    font.setPointSize(20);
    label_4->setFont(font);

    hboxLayout->addWidget(label_4);

    CountRate = new QLCDNumber(layoutWidget1);
    CountRate->setObjectName(QString::fromUtf8("CountRate"));
    QPalette palette;
    QBrush brush(QColor(0, 255, 0, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Text, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
    QBrush brush1(QColor(119, 119, 119, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
    CountRate->setPalette(palette);
    CountRate->setNumDigits(8);
    CountRate->setSegmentStyle(QLCDNumber::Filled);

    hboxLayout->addWidget(CountRate);

    widget = new QWidget(taRunTab);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(660, 340, 351, 56));
    gridLayout1 = new QGridLayout(widget);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setContentsMargins(0, 0, 0, 0);
    label_6 = new QLabel(widget);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    gridLayout1->addWidget(label_6, 0, 0, 1, 1);

    NumberBaselineChannels = new QSpinBox(widget);
    NumberBaselineChannels->setObjectName(QString::fromUtf8("NumberBaselineChannels"));
    NumberBaselineChannels->setMinimum(5);
    NumberBaselineChannels->setMaximum(20);

    gridLayout1->addWidget(NumberBaselineChannels, 0, 1, 1, 1);

    label_7 = new QLabel(widget);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout1->addWidget(label_7, 1, 0, 1, 1);

    Baseline = new QLabel(widget);
    Baseline->setObjectName(QString::fromUtf8("Baseline"));
    Baseline->setFrameShape(QFrame::Box);

    gridLayout1->addWidget(Baseline, 1, 1, 1, 1);

    widget1 = new QWidget(taRunTab);
    widget1->setObjectName(QString::fromUtf8("widget1"));
    widget1->setGeometry(QRect(660, 410, 351, 24));
    hboxLayout1 = new QHBoxLayout(widget1);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    label_8 = new QLabel(widget1);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    hboxLayout1->addWidget(label_8);

    Slope = new QLabel(widget1);
    Slope->setObjectName(QString::fromUtf8("Slope"));
    Slope->setFrameShape(QFrame::Box);

    hboxLayout1->addWidget(Slope);

    tabWidget->addTab(taRunTab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    tabWidget->addTab(tab_2, QString());
    TraceTab = new QWidget();
    TraceTab->setObjectName(QString::fromUtf8("TraceTab"));
    Trace = new QListWidget(TraceTab);
    Trace->setObjectName(QString::fromUtf8("Trace"));
    Trace->setGeometry(QRect(30, 60, 751, 681));
    pushButton = new QPushButton(TraceTab);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setGeometry(QRect(810, 60, 92, 30));
    tabWidget->addTab(TraceTab, QString());
    ResultsTab = new QWidget();
    ResultsTab->setObjectName(QString::fromUtf8("ResultsTab"));
    Results = new QTextEdit(ResultsTab);
    Results->setObjectName(QString::fromUtf8("Results"));
    Results->setGeometry(QRect(20, 30, 741, 721));
    tabWidget->addTab(ResultsTab, QString());
    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 1272, 32));
    menu_File = new QMenu(menubar);
    menu_File->setObjectName(QString::fromUtf8("menu_File"));
    menuAction = new QMenu(menubar);
    menuAction->setObjectName(QString::fromUtf8("menuAction"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);
    toolBar = new QToolBar(MainWindow);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

    menubar->addAction(menu_File->menuAction());
    menubar->addAction(menuAction->menuAction());
    menu_File->addAction(actionExit);
    menuAction->addAction(actionStart);
    menuAction->addAction(actionStop);
    menuAction->addAction(actionCount_Rate);
    menuAction->addAction(actionClear);
    toolBar->addAction(actionExit);
    toolBar->addAction(actionStart);
    toolBar->addAction(actionStop);
    toolBar->addAction(actionConfigure);
    toolBar->addAction(actionCount_Rate);
    toolBar->addAction(actionClear);

    retranslateUi(MainWindow);
    QObject::connect(pushButton, SIGNAL(clicked()), Trace, SLOT(clear()));

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    actionExit->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
    actionStart->setText(QApplication::translate("MainWindow", "&Start", 0, QApplication::UnicodeUTF8));
    actionStop->setText(QApplication::translate("MainWindow", "S&top", 0, QApplication::UnicodeUTF8));
    actionCount_Rate->setText(QApplication::translate("MainWindow", "&Count Rate", 0, QApplication::UnicodeUTF8));
    actionConfigure->setText(QApplication::translate("MainWindow", "C&onfigure...", 0, QApplication::UnicodeUTF8));
    actionClear->setText(QApplication::translate("MainWindow", "Clear", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("MainWindow", "Serial Port", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("MainWindow", "Wavelength(nm)", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("MainWindow", "Sample Time (us)", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("MainWindow", "Run Time (s)", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("MainWindow", "Count Rate", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("MainWindow", "Number Baseline Channels", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("MainWindow", "Baseline", 0, QApplication::UnicodeUTF8));
    Baseline->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("MainWindow", "Slope", 0, QApplication::UnicodeUTF8));
    Slope->setText(QApplication::translate("MainWindow", "1.0", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(taRunTab), QApplication::translate("MainWindow", "Run", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("MainWindow", "Clear", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(TraceTab), QApplication::translate("MainWindow", "Trace", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(ResultsTab), QApplication::translate("MainWindow", "Results", 0, QApplication::UnicodeUTF8));
    menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
    menuAction->setTitle(QApplication::translate("MainWindow", "Action", 0, QApplication::UnicodeUTF8));
    toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // UI_CORRELATORUI_H
