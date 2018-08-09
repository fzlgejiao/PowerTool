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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StationParameterDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_name;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QFrame *line;
    QTableWidget *tableWidget_parameter;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_tideway;
    QPushButton *pushButton_voltage;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_OK;

    void setupUi(QDialog *StationParameterDialog)
    {
        if (StationParameterDialog->objectName().isEmpty())
            StationParameterDialog->setObjectName(QString::fromUtf8("StationParameterDialog"));
        StationParameterDialog->resize(709, 448);
        verticalLayoutWidget = new QWidget(StationParameterDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(40, 50, 611, 331));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        lineEdit_name = new QLineEdit(verticalLayoutWidget);
        lineEdit_name->setObjectName(QString::fromUtf8("lineEdit_name"));

        horizontalLayout->addWidget(lineEdit_name);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        label_2->setFont(font1);

        horizontalLayout->addWidget(label_2);


        verticalLayout->addLayout(horizontalLayout);

        line = new QFrame(verticalLayoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        tableWidget_parameter = new QTableWidget(verticalLayoutWidget);
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

        verticalLayout->addWidget(tableWidget_parameter);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_tideway = new QPushButton(verticalLayoutWidget);
        pushButton_tideway->setObjectName(QString::fromUtf8("pushButton_tideway"));

        horizontalLayout_2->addWidget(pushButton_tideway);

        pushButton_voltage = new QPushButton(verticalLayoutWidget);
        pushButton_voltage->setObjectName(QString::fromUtf8("pushButton_voltage"));

        horizontalLayout_2->addWidget(pushButton_voltage);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_OK = new QPushButton(verticalLayoutWidget);
        pushButton_OK->setObjectName(QString::fromUtf8("pushButton_OK"));

        horizontalLayout_2->addWidget(pushButton_OK);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(StationParameterDialog);

        QMetaObject::connectSlotsByName(StationParameterDialog);
    } // setupUi

    void retranslateUi(QDialog *StationParameterDialog)
    {
        StationParameterDialog->setWindowTitle(QApplication::translate("StationParameterDialog", "Station", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StationParameterDialog", "Station Name:", 0, QApplication::UnicodeUTF8));
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
        pushButton_tideway->setText(QApplication::translate("StationParameterDialog", "TideWay", 0, QApplication::UnicodeUTF8));
        pushButton_voltage->setText(QApplication::translate("StationParameterDialog", "Voltage", 0, QApplication::UnicodeUTF8));
        pushButton_OK->setText(QApplication::translate("StationParameterDialog", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StationParameterDialog: public Ui_StationParameterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONPARAMETERDIALOG_H
