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

class Ui_AddDialog
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget_hidden;
    QFrame *line_2;
    QLabel *label_branch;
    QTableWidget *tableWidget_branch;
    QVBoxLayout *verticalLayout_4;
    QLineEdit *lineEdit_area;
    QComboBox *comboBox_areas;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_5;
    QPushButton *pushButton_add;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton_addall;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButton_remove;
    QSpacerItem *verticalSpacer_4;
    QPushButton *pushButton_removeall;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget_added;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QComboBox *comboBox_StatType;
    QLabel *label;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLineEdit *lineEdit_name;
    QPushButton *pushButton_font;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;
    QFrame *line;
    QLabel *label_2;
    QLineEdit *lineEdit_addedCnt;
    QLineEdit *lineEdit_hiddenCnt;

    void setupUi(QDialog *AddDialog)
    {
        if (AddDialog->objectName().isEmpty())
            AddDialog->setObjectName(QString::fromUtf8("AddDialog"));
        AddDialog->resize(652, 500);
        horizontalLayoutWidget = new QWidget(AddDialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(50, 91, 551, 381));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidget_hidden = new QTableWidget(horizontalLayoutWidget);
        if (tableWidget_hidden->columnCount() < 3)
            tableWidget_hidden->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_hidden->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_hidden->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_hidden->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget_hidden->setObjectName(QString::fromUtf8("tableWidget_hidden"));

        verticalLayout_2->addWidget(tableWidget_hidden);

        line_2 = new QFrame(horizontalLayoutWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_2);

        label_branch = new QLabel(horizontalLayoutWidget);
        label_branch->setObjectName(QString::fromUtf8("label_branch"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label_branch->setFont(font);

        verticalLayout_2->addWidget(label_branch);

        tableWidget_branch = new QTableWidget(horizontalLayoutWidget);
        if (tableWidget_branch->columnCount() < 3)
            tableWidget_branch->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_branch->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_branch->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_branch->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        tableWidget_branch->setObjectName(QString::fromUtf8("tableWidget_branch"));

        verticalLayout_2->addWidget(tableWidget_branch);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        lineEdit_area = new QLineEdit(horizontalLayoutWidget);
        lineEdit_area->setObjectName(QString::fromUtf8("lineEdit_area"));
        lineEdit_area->setFont(font);
        lineEdit_area->setAlignment(Qt::AlignCenter);
        lineEdit_area->setReadOnly(true);

        verticalLayout_4->addWidget(lineEdit_area);

        comboBox_areas = new QComboBox(horizontalLayoutWidget);
        comboBox_areas->setObjectName(QString::fromUtf8("comboBox_areas"));

        verticalLayout_4->addWidget(comboBox_areas);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);

        pushButton_add = new QPushButton(horizontalLayoutWidget);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        pushButton_add->setEnabled(false);

        verticalLayout->addWidget(pushButton_add);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButton_addall = new QPushButton(horizontalLayoutWidget);
        pushButton_addall->setObjectName(QString::fromUtf8("pushButton_addall"));

        verticalLayout->addWidget(pushButton_addall);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        pushButton_remove = new QPushButton(horizontalLayoutWidget);
        pushButton_remove->setObjectName(QString::fromUtf8("pushButton_remove"));
        pushButton_remove->setEnabled(false);

        verticalLayout->addWidget(pushButton_remove);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        pushButton_removeall = new QPushButton(horizontalLayoutWidget);
        pushButton_removeall->setObjectName(QString::fromUtf8("pushButton_removeall"));

        verticalLayout->addWidget(pushButton_removeall);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        verticalLayout_4->addLayout(verticalLayout);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tableWidget_added = new QTableWidget(horizontalLayoutWidget);
        if (tableWidget_added->columnCount() < 3)
            tableWidget_added->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_added->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_added->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_added->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        tableWidget_added->setObjectName(QString::fromUtf8("tableWidget_added"));

        verticalLayout_3->addWidget(tableWidget_added);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(horizontalLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        horizontalLayout_3->addWidget(label_4);

        comboBox_StatType = new QComboBox(horizontalLayoutWidget);
        comboBox_StatType->setObjectName(QString::fromUtf8("comboBox_StatType"));

        horizontalLayout_3->addWidget(comboBox_StatType);


        verticalLayout_3->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(verticalLayout_3);

        label = new QLabel(AddDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 73, 135, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        horizontalLayoutWidget_2 = new QWidget(AddDialog);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(50, 10, 551, 41));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(8);
        font2.setBold(true);
        font2.setWeight(75);
        label_3->setFont(font2);

        horizontalLayout_2->addWidget(label_3);

        lineEdit_name = new QLineEdit(horizontalLayoutWidget_2);
        lineEdit_name->setObjectName(QString::fromUtf8("lineEdit_name"));

        horizontalLayout_2->addWidget(lineEdit_name);

        pushButton_font = new QPushButton(horizontalLayoutWidget_2);
        pushButton_font->setObjectName(QString::fromUtf8("pushButton_font"));
        pushButton_font->setFont(font);

        horizontalLayout_2->addWidget(pushButton_font);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(horizontalLayoutWidget_2);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_2->addWidget(buttonBox);

        line = new QFrame(AddDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(50, 50, 551, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(AddDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(380, 73, 126, 20));
        label_2->setFont(font1);
        lineEdit_addedCnt = new QLineEdit(AddDialog);
        lineEdit_addedCnt->setObjectName(QString::fromUtf8("lineEdit_addedCnt"));
        lineEdit_addedCnt->setGeometry(QRect(570, 70, 31, 20));
        lineEdit_addedCnt->setReadOnly(true);
        lineEdit_hiddenCnt = new QLineEdit(AddDialog);
        lineEdit_hiddenCnt->setObjectName(QString::fromUtf8("lineEdit_hiddenCnt"));
        lineEdit_hiddenCnt->setGeometry(QRect(220, 70, 31, 20));
        lineEdit_hiddenCnt->setReadOnly(true);

        retranslateUi(AddDialog);

        QMetaObject::connectSlotsByName(AddDialog);
    } // setupUi

    void retranslateUi(QDialog *AddDialog)
    {
        AddDialog->setWindowTitle(QApplication::translate("AddDialog", "Station Editor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        AddDialog->setWhatsThis(QApplication::translate("AddDialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt; font-weight:600;\">This Dialog is for Add a new Site or Edit a station</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        QTableWidgetItem *___qtablewidgetitem = tableWidget_hidden->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AddDialog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_hidden->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AddDialog", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_hidden->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("AddDialog", "Name", 0, QApplication::UnicodeUTF8));
        label_branch->setText(QApplication::translate("AddDialog", "connect to which", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_branch->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("AddDialog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_branch->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("AddDialog", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_branch->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("AddDialog", "Name", 0, QApplication::UnicodeUTF8));
        lineEdit_area->setText(QApplication::translate("AddDialog", "Areas:", 0, QApplication::UnicodeUTF8));
        comboBox_areas->clear();
        comboBox_areas->insertItems(0, QStringList()
         << QApplication::translate("AddDialog", "All Areas", 0, QApplication::UnicodeUTF8)
        );
        pushButton_add->setText(QApplication::translate("AddDialog", "Insert>", 0, QApplication::UnicodeUTF8));
        pushButton_addall->setText(QApplication::translate("AddDialog", "Insert All>>", 0, QApplication::UnicodeUTF8));
        pushButton_remove->setText(QApplication::translate("AddDialog", "<Revoke", 0, QApplication::UnicodeUTF8));
        pushButton_removeall->setText(QApplication::translate("AddDialog", "<<Revoke All", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_added->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("AddDialog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_added->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("AddDialog", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_added->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("AddDialog", "Name", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AddDialog", "Station Type:", 0, QApplication::UnicodeUTF8));
        comboBox_StatType->clear();
        comboBox_StatType->insertItems(0, QStringList()
         << QApplication::translate("AddDialog", "Hydropower Station", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AddDialog", "ThermalPower Station", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("AddDialog", "HiddenItemList:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AddDialog", "Site Name:", 0, QApplication::UnicodeUTF8));
        lineEdit_name->setText(QString());
        pushButton_font->setText(QApplication::translate("AddDialog", "Font", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AddDialog", "AddedItemList:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AddDialog: public Ui_AddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDDIALOG_H
