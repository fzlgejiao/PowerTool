/********************************************************************************
** Form generated from reading UI file 'textdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTDIALOG_H
#define UI_TEXTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TextDialog
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_alignleft;
    QRadioButton *radioButton_aligncenter;
    QRadioButton *radioButton_alignright;
    QCheckBox *checkBox_border;
    QDialogButtonBox *buttonBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QPushButton *pushButton_font;
    QSpacerItem *horizontalSpacer;
    QTextEdit *textEdit_inputtext;

    void setupUi(QDialog *TextDialog)
    {
        if (TextDialog->objectName().isEmpty())
            TextDialog->setObjectName(QString::fromUtf8("TextDialog"));
        TextDialog->resize(532, 230);
        widget = new QWidget(TextDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 20, 491, 191));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_4 = new QHBoxLayout(groupBox);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioButton_alignleft = new QRadioButton(groupBox);
        radioButton_alignleft->setObjectName(QString::fromUtf8("radioButton_alignleft"));

        horizontalLayout_3->addWidget(radioButton_alignleft);

        radioButton_aligncenter = new QRadioButton(groupBox);
        radioButton_aligncenter->setObjectName(QString::fromUtf8("radioButton_aligncenter"));

        horizontalLayout_3->addWidget(radioButton_aligncenter);

        radioButton_alignright = new QRadioButton(groupBox);
        radioButton_alignright->setObjectName(QString::fromUtf8("radioButton_alignright"));

        horizontalLayout_3->addWidget(radioButton_alignright);


        horizontalLayout_4->addLayout(horizontalLayout_3);


        horizontalLayout->addWidget(groupBox);

        checkBox_border = new QCheckBox(widget);
        checkBox_border->setObjectName(QString::fromUtf8("checkBox_border"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        checkBox_border->setFont(font);

        horizontalLayout->addWidget(checkBox_border);

        buttonBox = new QDialogButtonBox(widget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);

        horizontalLayout_2->addWidget(label);

        pushButton_font = new QPushButton(widget);
        pushButton_font->setObjectName(QString::fromUtf8("pushButton_font"));
        pushButton_font->setFont(font);

        horizontalLayout_2->addWidget(pushButton_font);

        horizontalSpacer = new QSpacerItem(218, 21, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        textEdit_inputtext = new QTextEdit(widget);
        textEdit_inputtext->setObjectName(QString::fromUtf8("textEdit_inputtext"));

        verticalLayout->addWidget(textEdit_inputtext);


        retranslateUi(TextDialog);

        QMetaObject::connectSlotsByName(TextDialog);
    } // setupUi

    void retranslateUi(QDialog *TextDialog)
    {
        TextDialog->setWindowTitle(QApplication::translate("TextDialog", "TextDialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("TextDialog", "Align", 0, QApplication::UnicodeUTF8));
        radioButton_alignleft->setText(QApplication::translate("TextDialog", "Left", 0, QApplication::UnicodeUTF8));
        radioButton_aligncenter->setText(QApplication::translate("TextDialog", "Center", 0, QApplication::UnicodeUTF8));
        radioButton_alignright->setText(QApplication::translate("TextDialog", "Right", 0, QApplication::UnicodeUTF8));
        checkBox_border->setText(QApplication::translate("TextDialog", "Border", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TextDialog", "Text:", 0, QApplication::UnicodeUTF8));
        pushButton_font->setText(QApplication::translate("TextDialog", "Font", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TextDialog: public Ui_TextDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTDIALOG_H
