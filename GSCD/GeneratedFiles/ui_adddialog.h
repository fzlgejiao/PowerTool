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
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QListView *listView_hidenitems;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_add;
    QPushButton *pushButton_addall;
    QPushButton *pushButton_remove;
    QPushButton *pushButton_removeall;
    QListView *listView_showitems;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *AddDialog)
    {
        if (AddDialog->objectName().isEmpty())
            AddDialog->setObjectName(QString::fromUtf8("AddDialog"));
        AddDialog->resize(674, 476);
        buttonBox = new QDialogButtonBox(AddDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(440, 400, 156, 23));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        horizontalLayoutWidget = new QWidget(AddDialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(50, 80, 551, 301));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        listView_hidenitems = new QListView(horizontalLayoutWidget);
        listView_hidenitems->setObjectName(QString::fromUtf8("listView_hidenitems"));

        horizontalLayout->addWidget(listView_hidenitems);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton_add = new QPushButton(horizontalLayoutWidget);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        pushButton_add->setEnabled(false);

        verticalLayout->addWidget(pushButton_add);

        pushButton_addall = new QPushButton(horizontalLayoutWidget);
        pushButton_addall->setObjectName(QString::fromUtf8("pushButton_addall"));

        verticalLayout->addWidget(pushButton_addall);

        pushButton_remove = new QPushButton(horizontalLayoutWidget);
        pushButton_remove->setObjectName(QString::fromUtf8("pushButton_remove"));
        pushButton_remove->setEnabled(false);

        verticalLayout->addWidget(pushButton_remove);

        pushButton_removeall = new QPushButton(horizontalLayoutWidget);
        pushButton_removeall->setObjectName(QString::fromUtf8("pushButton_removeall"));

        verticalLayout->addWidget(pushButton_removeall);


        horizontalLayout->addLayout(verticalLayout);

        listView_showitems = new QListView(horizontalLayoutWidget);
        listView_showitems->setObjectName(QString::fromUtf8("listView_showitems"));

        horizontalLayout->addWidget(listView_showitems);

        label = new QLabel(AddDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 60, 111, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(AddDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(370, 60, 81, 16));
        label_2->setFont(font);

        retranslateUi(AddDialog);

        QMetaObject::connectSlotsByName(AddDialog);
    } // setupUi

    void retranslateUi(QDialog *AddDialog)
    {
        AddDialog->setWindowTitle(QApplication::translate("AddDialog", "\346\267\273\345\212\240\347\253\231\347\202\271", 0, QApplication::UnicodeUTF8));
        pushButton_add->setText(QApplication::translate("AddDialog", "Insert>", 0, QApplication::UnicodeUTF8));
        pushButton_addall->setText(QApplication::translate("AddDialog", "Inser All>>", 0, QApplication::UnicodeUTF8));
        pushButton_remove->setText(QApplication::translate("AddDialog", "<Revoke", 0, QApplication::UnicodeUTF8));
        pushButton_removeall->setText(QApplication::translate("AddDialog", "<<Revoke All", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AddDialog", " \346\234\252\347\273\230\345\210\266\347\232\204\347\253\231\347\202\271:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AddDialog", "\345\233\276\344\270\255\347\253\231\347\202\271:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AddDialog: public Ui_AddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDDIALOG_H
