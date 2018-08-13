/********************************************************************************
** Form generated from reading UI file 'adddialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDDIALOG_H
#define UI_ADDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_AddDialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QFrame *line;
    QDialogButtonBox *buttonBox;
    QLabel *label_3;
    QLineEdit *lineEdit_name;
    QPushButton *pushButton_add;
    QPushButton *pushButton_addall;
    QPushButton *pushButton_remove;
    QPushButton *pushButton_removeall;
    QTableWidget *tableWidget_added;
    QTableWidget *tableWidget_hidden;
    QLabel *label_branch;
    QTableWidget *tableWidget_branch;
    QLabel *label_4;
    QComboBox *comboBox_StatType;

    void setupUi(QDialog *AddDialog)
    {
        if (AddDialog->objectName().isEmpty())
            AddDialog->setObjectName(QString::fromUtf8("AddDialog"));
        AddDialog->resize(738, 546);
        label = new QLabel(AddDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 60, 221, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(AddDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(430, 60, 191, 16));
        label_2->setFont(font);
        line = new QFrame(AddDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(30, 40, 671, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        buttonBox = new QDialogButtonBox(AddDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(480, 10, 193, 28));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label_3 = new QLabel(AddDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 20, 121, 16));
        label_3->setFont(font);
        lineEdit_name = new QLineEdit(AddDialog);
        lineEdit_name->setObjectName(QString::fromUtf8("lineEdit_name"));
        lineEdit_name->setGeometry(QRect(180, 20, 137, 22));
        pushButton_add = new QPushButton(AddDialog);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        pushButton_add->setEnabled(false);
        pushButton_add->setGeometry(QRect(320, 95, 93, 28));
        pushButton_addall = new QPushButton(AddDialog);
        pushButton_addall->setObjectName(QString::fromUtf8("pushButton_addall"));
        pushButton_addall->setGeometry(QRect(320, 160, 93, 28));
        pushButton_remove = new QPushButton(AddDialog);
        pushButton_remove->setObjectName(QString::fromUtf8("pushButton_remove"));
        pushButton_remove->setEnabled(false);
        pushButton_remove->setGeometry(QRect(320, 220, 93, 28));
        pushButton_removeall = new QPushButton(AddDialog);
        pushButton_removeall->setObjectName(QString::fromUtf8("pushButton_removeall"));
        pushButton_removeall->setGeometry(QRect(320, 280, 93, 28));
        tableWidget_added = new QTableWidget(AddDialog);
        if (tableWidget_added->columnCount() < 3)
            tableWidget_added->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_added->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_added->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_added->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget_added->setObjectName(QString::fromUtf8("tableWidget_added"));
        tableWidget_added->setGeometry(QRect(430, 90, 271, 401));
        tableWidget_hidden = new QTableWidget(AddDialog);
        if (tableWidget_hidden->columnCount() < 3)
            tableWidget_hidden->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_hidden->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_hidden->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_hidden->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        tableWidget_hidden->setObjectName(QString::fromUtf8("tableWidget_hidden"));
        tableWidget_hidden->setGeometry(QRect(32, 90, 271, 241));
        label_branch = new QLabel(AddDialog);
        label_branch->setObjectName(QString::fromUtf8("label_branch"));
        label_branch->setGeometry(QRect(30, 340, 135, 19));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label_branch->setFont(font1);
        tableWidget_branch = new QTableWidget(AddDialog);
        if (tableWidget_branch->columnCount() < 3)
            tableWidget_branch->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_branch->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_branch->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_branch->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        tableWidget_branch->setObjectName(QString::fromUtf8("tableWidget_branch"));
        tableWidget_branch->setGeometry(QRect(32, 372, 271, 151));
        label_4 = new QLabel(AddDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(428, 500, 105, 19));
        label_4->setFont(font1);
        comboBox_StatType = new QComboBox(AddDialog);
        comboBox_StatType->setObjectName(QString::fromUtf8("comboBox_StatType"));
        comboBox_StatType->setGeometry(QRect(540, 500, 158, 22));

        retranslateUi(AddDialog);

        QMetaObject::connectSlotsByName(AddDialog);
    } // setupUi

    void retranslateUi(QDialog *AddDialog)
    {
        AddDialog->setWindowTitle(QApplication::translate("AddDialog", "Station Editor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        AddDialog->setWhatsThis(QApplication::translate("AddDialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt; font-weight:600;\">This Dialog is for Add a new Site or Edit a station</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label->setText(QApplication::translate("AddDialog", "Buses not in station:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AddDialog", "Buses in station:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AddDialog", "Station Name:", 0, QApplication::UnicodeUTF8));
        lineEdit_name->setText(QString());
        pushButton_add->setText(QApplication::translate("AddDialog", "Insert>", 0, QApplication::UnicodeUTF8));
        pushButton_addall->setText(QApplication::translate("AddDialog", "Insert All>>", 0, QApplication::UnicodeUTF8));
        pushButton_remove->setText(QApplication::translate("AddDialog", "<Remove", 0, QApplication::UnicodeUTF8));
        pushButton_removeall->setText(QApplication::translate("AddDialog", "<<Remove All", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_added->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AddDialog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_added->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AddDialog", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_added->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("AddDialog", "Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_hidden->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("AddDialog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_hidden->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("AddDialog", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_hidden->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("AddDialog", "Name", 0, QApplication::UnicodeUTF8));
        label_branch->setText(QApplication::translate("AddDialog", "Connected bus", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_branch->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("AddDialog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_branch->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("AddDialog", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_branch->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("AddDialog", "Name", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AddDialog", "Station Type:", 0, QApplication::UnicodeUTF8));
        comboBox_StatType->clear();
        comboBox_StatType->insertItems(0, QStringList()
         << QApplication::translate("AddDialog", "Hydropower Station", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AddDialog", "ThermalPower Station", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class AddDialog: public Ui_AddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDDIALOG_H
