#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>
#include "ui_textdialog.h"
#include "idata.h"

class QFontDialog;
class QColor;

class TextDialog : public QDialog
{
	Q_OBJECT

public:
	TextDialog(iNote *note ,QWidget *parent = 0);
	~TextDialog();
	QString  GetText(){return ui.textEdit_inputtext->toPlainText().trimmed();}
	QFont GetFont() {return m_font;}
	bool HasBorder() {return hasborder;}
	QColor GetTextcolor(){return m_color;}
	Qt::Alignment GetAlignmode(){return m_alignmode;}

private slots:
	void OnFontdialog();
	void OnColorChanged();
	void OnBordertoggled(bool toggle);
	void OnAlignmodeChanged();
	void OnOK();

private:
	Ui::TextDialog ui;
	iNote *m_note;
	QFont m_font;	
	bool hasborder;
	QColor m_color;
	Qt::Alignment m_alignmode;
};

#endif // TEXTDIALOG_H

