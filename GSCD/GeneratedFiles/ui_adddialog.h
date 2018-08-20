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
    QFrame *line;
    QLabel *label_2;
    QLineEdit *lineEdit_addedCnt;
    QLineEdit *lineEdit_hiddenCnt;
    QTableWidget *tableWidget_added;
    QLineEdit *lineEdit_area;
    QComboBox *comboBox_areas;
    QComboBox *comboBox_StatType;
    QLabel *label_4;
    QPushButton *pushButton_add;
    QPushButton *pushButton_addall;
    QPushButton *pushButton_removeall;
    QPushButton *pushButton_remove;
    QTableWidget *tableWidget_hidden;
    QLabel *label_branch;
    QTableWidget *tableWidget_branch;
    QLabel *label_3;
    QLineEdit *lineEdit_name;
    QPushButton *pushButton_font;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AddDialog)
    {
        if (AddDialog->objectName().isEmpty())
            AddDialog->setObjectName(QString::fromUtf8("AddDialog"));
        AddDialog->resize(724, 494);
        label = new QLabel(AddDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 70, 181, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        label->setFont(font);
        line = new QFrame(AddDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 50, 701, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(AddDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(431, 73, 161, 20));
        label_2->setFont(font);
        lineEdit_addedCnt = new QLineEdit(AddDialog);
        lineEdit_addedCnt->setObjectName(QString::fromUtf8("lineEdit_addedCnt"));
        lineEdit_addedCnt->setGeometry(QRect(680, 71, 31, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(9);
        lineEdit_addedCnt->setFont(font1);
        lineEdit_addedCnt->setReadOnly(true);
        lineEdit_hiddenCnt = new QLineEdit(AddDialog);
        lineEdit_hiddenCnt->setObjectName(QString::fromUtf8("lineEdit_hiddenCnt"));
        lineEdit_hiddenCnt->setGeometry(QRect(260, 68, 31, 20));
        lineEdit_hiddenCnt->setFont(font1);
        lineEdit_hiddenCnt->setReadOnly(true);
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
        tableWidget_added->setGeometry(QRect(431, 93, 280, 349));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        tableWidget_added->setFont(font2);
        lineEdit_area = new QLineEdit(AddDialog);
        lineEdit_area->setObjectName(QString::fromUtf8("lineEdit_area"));
        lineEdit_area->setGeometry(QRect(298, 80, 118, 25));
        lineEdit_area->setFont(font);
        lineEdit_area->setAlignment(Qt::AlignCenter);
        lineEdit_area->setReadOnly(true);
        comboBox_areas = new QComboBox(AddDialog);
        comboBox_areas->setObjectName(QString::fromUtf8("comboBox_areas"));
        comboBox_areas->setGeometry(QRect(298, 110, 118, 25));
        comboBox_areas->setFont(font2);
        comboBox_StatType = new QComboBox(AddDialog);
        comboBox_StatType->setObjectName(QString::fromUtf8("comboBox_StatType"));
        comboBox_StatType->setGeometry(QRect(580, 450, 132, 25));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        font3.setPointSize(8);
        comboBox_StatType->setFont(font3);
        label_4 = new QLabel(AddDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(430, 460, 131, 16));
        label_4->setFont(font);
        pushButton_add = new QPushButton(AddDialog);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        pushButton_add->setEnabled(false);
        pushButton_add->setGeometry(QRect(300, 150, 118, 25));
        pushButton_add->setFont(font2);
        pushButton_addall = new QPushButton(AddDialog);
        pushButton_addall->setObjectName(QString::fromUtf8("pushButton_addall"));
        pushButton_addall->setGeometry(QRect(300, 190, 118, 25));
        pushButton_addall->setFont(font2);
        pushButton_removeall = new QPushButton(AddDialog);
        pushButton_removeall->setObjectName(QString::fromUtf8("pushButton_removeall"));
        pushButton_removeall->setGeometry(QRect(300, 270, 118, 25));
        pushButton_removeall->setFont(font2);
        pushButton_remove = new QPushButton(AddDialog);
        pushButton_remove->setObjectName(QString::fromUtf8("pushButton_remove"));
        pushButton_remove->setEnabled(false);
        pushButton_remove->setGeometry(QRect(300, 230, 118, 25));
        pushButton_remove->setFont(font2);
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
        tableWidget_hidden->setGeometry(QRect(10, 90, 280, 221));
        tableWidget_hidden->setFont(font2);
        label_branch = new QLabel(AddDialog);
        label_branch->setObjectName(QString::fromUtf8("label_branch"));
        label_branch->setGeometry(QRect(31, 330, 135, 19));
        label_branch->setFont(font);
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
        tableWidget_branch->setGeometry(QRect(10, 357, 280, 121));
        tableWidget_branch->setFont(font2);
        label_3 = new QLabel(AddDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 20, 104, 19));
        label_3->setFont(font);
        lineEdit_name = new QLineEdit(AddDialog);
        lineEdit_name->setObjectName(QString::fromUtf8("lineEdit_name"));
        lineEdit_name->setGeometry(QRect(130, 19, 154, 23));
        lineEdit_name->setFont(font1);
        pushButton_font = new QPushButton(AddDialog);
        pushButton_font->setObjectName(QString::fromUtf8("pushButton_font"));
        pushButton_font->setGeometry(QRect(300, 20, 118, 28));
        pushButton_font->setFont(font);
        buttonBox = new QDialogButtonBox(AddDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(480, 20, 193, 28));
        buttonBox->setFont(font);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(AddDialog);

        QMetaObject::connectSlotsByName(AddDialog);
    } // setupUi

    void retranslateUi(QDialog *AddDialog)
    {
        AddDialog->setWindowTitle(QApplication::translate("AddDialog", "Station Editor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        AddDialog->setWhatsThis(QApplication::translate("AddDialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt; font-weight:600;\">This Dialog is for Add a new Site or Edit a station</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label->setText(QApplication::translate("AddDialog", "Not included Buses:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AddDialog", "Included Buses", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_added->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AddDialog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_added->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AddDialog", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_added->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("AddDialog", "Name", 0, QApplication::UnicodeUTF8));
        lineEdit_area->setText(QApplication::translate("AddDialog", "Areas:", 0, QApplication::UnicodeUTF8));
        comboBox_areas->clear();
        comboBox_areas->insertItems(0, QStringList()
         << QApplication::translate("AddDialog", "All Areas", 0, QApplication::UnicodeUTF8)
        );
        comboBox_StatType->clear();
        comboBox_StatType->insertItems(0, QStringList()
         << QApplication::translate("AddDialog", "Hydropower Station", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AddDialog", "ThermalPower Station", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("AddDialog", "Station Type:", 0, QApplication::UnicodeUTF8));
        pushButton_add->setText(QApplication::translate("AddDialog", "Insert>", 0, QApplication::UnicodeUTF8));
        pushButton_addall->setText(QApplication::translate("AddDialog", "Insert All>>", 0, QApplication::UnicodeUTF8));
        pushButton_removeall->setText(QApplication::translate("AddDialog", "<<Remove All", 0, QApplication::UnicodeUTF8));
        pushButton_remove->setText(QApplication::translate("AddDialog", "<Remove", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_hidden->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("AddDialog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_hidden->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("AddDialog", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_hidden->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("AddDialog", "Name", 0, QApplication::UnicodeUTF8));
        label_branch->setText(QApplication::translate("AddDialog", "connected Buses:", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_branch->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("AddDialog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_branch->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("AddDialog", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_branch->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("AddDialog", "Name", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AddDialog", "Station Name:", 0, QApplication::UnicodeUTF8));
        lineEdit_name->setText(QString());
        pushButton_font->setText(QApplication::translate("AddDialog", "Font...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AddDialog: public Ui_AddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDDIALOG_H
