#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>
#include "ui_textdialog.h"


class QFontDialog;
class QColor;

enum AlignMode
{
	AlignLeft=0,
	Center,
	AlignRight
};

class TextDialog : public QDialog
{
	Q_OBJECT

public:
	TextDialog(const QString& text ,QWidget *parent = 0);
	~TextDialog();
	QString  GetText(){return m_text;}
	QFont GetFont() {return m_font;}
	bool HasBorder() {return hasborder;}
	QColor GetTextcolor(){return m_color;}

private slots:
	void OnFontdialog();
	void OnColorChanged();
private:
	Ui::TextDialog ui;
	QString m_text;
	QFont m_font;
	QFont defaultfont;
	bool hasborder;
	QColor m_color;
};

#endif // TEXTDIALOG_H
