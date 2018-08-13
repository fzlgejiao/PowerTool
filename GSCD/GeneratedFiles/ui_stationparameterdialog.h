/********************************************************************************
** Form generated from reading UI file 'stationparameterdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATIONPARAMETERDIALOG_H
#define UI_STATIONPARAMETERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_StationParameterDialog
{
public:
    QLabel *label_2;
    QFrame *line;
    QTableWidget *tableWidget_parameter;
    QPushButton *pushButton_tideway;
    QPushButton *pushButton_voltage;
    QPushButton *pushButton_OK;
    QLabel *label;
    QLineEdit *lineEdit_name;

    void setupUi(QDialog *StationParameterDialog)
    {
        if (StationParameterDialog->objectName().isEmpty())
            StationParameterDialog->setObjectName(QString::fromUtf8("StationParameterDialog"));
        StationParameterDialog->resize(709, 348);
        label_2 = new QLabel(StationParameterDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(610, 30, 64, 22));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        label_2->setFont(font);
        line = new QFrame(StationParameterDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(600, 40, 91, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        tableWidget_parameter = new QTableWidget(StationParameterDialog);
        if (tableWidget_parameter->columnCount() < 7)
            tableWidget_parameter->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_parameter->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_parameter->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_parameter->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_parameter->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_parameter->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_parameter->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_parameter->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget_parameter->setObjectName(QString::fromUtf8("tableWidget_parameter"));
        tableWidget_parameter->setGeometry(QRect(21, 72, 671, 221));
        pushButton_tideway = new QPushButton(StationParameterDialog);
        pushButton_tideway->setObjectName(QString::fromUtf8("pushButton_tideway"));
        pushButton_tideway->setGeometry(QRect(50, 310, 93, 28));
        pushButton_voltage = new QPushButton(StationParameterDialog);
        pushButton_voltage->setObjectName(QString::fromUtf8("pushButton_voltage"));
        pushButton_voltage->setGeometry(QRect(170, 310, 93, 28));
        pushButton_OK = new QPushButton(StationParameterDialog);
        pushButton_OK->setObjectName(QString::fromUtf8("pushButton_OK"));
        pushButton_OK->setGeometry(QRect(600, 310, 93, 28));
        label = new QLabel(StationParameterDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 32, 113, 19));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        lineEdit_name = new QLineEdit(StationParameterDialog);
        lineEdit_name->setObjectName(QString::fromUtf8("lineEdit_name"));
        lineEdit_name->setGeometry(QRect(140, 32, 137, 22));
        label_2->raise();
        pushButton_OK->raise();
        line->raise();

        retranslateUi(StationParameterDialog);

        QMetaObject::connectSlotsByName(StationParameterDialog);
    } // setupUi

    void retranslateUi(QDialog *StationParameterDialog)
    {
        StationParameterDialog->setWindowTitle(QApplication::translate("StationParameterDialog", "Station", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StationParameterDialog", "Unit:-----", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_parameter->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("StationParameterDialog", "Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_parameter->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("StationParameterDialog", "Ref-Voltage", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_parameter->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("StationParameterDialog", "Voltage", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_parameter->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("StationParameterDialog", "Angle", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_parameter->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("StationParameterDialog", "Energy", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_parameter->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("StationParameterDialog", "Load", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_parameter->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("StationParameterDialog", "Compensation", 0, QApplication::UnicodeUTF8));
        pushButton_tideway->setText(QApplication::translate("StationParameterDialog", "Power flow...", 0, QApplication::UnicodeUTF8));
        pushButton_voltage->setText(QApplication::translate("StationParameterDialog", "Voltage...", 0, QApplication::UnicodeUTF8));
        pushButton_OK->setText(QApplication::translate("StationParameterDialog", "OK", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StationParameterDialog", "Station Name:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StationParameterDialog: public Ui_StationParameterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONPARAMETERDIALOG_H
