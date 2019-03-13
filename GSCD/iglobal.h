#ifndef IGLOBAL_H
#define IGLOBAL_H

#include <QObject>
#include <QMap>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QColor>
#include <QTranslator>

#define	CHG_COLORMAP		0x01
#define	CHG_WARNING 		0x02
#define	CHG_DCWIDTH		    0x04
#define	CHG_DCCOLOR 		0x08
#define	CHG_DEFAULTCOLOR 	0x10
#define	CHG_LEVELS 			0x20

typedef enum 
{
	NoError =0,
	VoltageError,
	WidthError
}ErrorType;

typedef enum 
{
	English =0,
	Chinese,
	Lan_Cnt
}Language;

class CVoltageLevel
{

public:
	CVoltageLevel(float voltage,int width,QColor color);
	
	~CVoltageLevel(){}
		
	float refVoltage() {return m_refvoltage;}
	void setvoltage(float v){m_refvoltage=v;}

	int getwidth(){return m_width;}
	void setwidth(int width){m_width=width;}

	QColor  getcolor(){return m_color;}
	void setcolor(QColor color){m_color=color;}
	
private:	
	friend class iGlobal;
	float m_refvoltage;
	int m_width;
	QColor m_color;	
};


bool LevelvoltageAscending(CVoltageLevel *level1,CVoltageLevel *level2);


class iGlobal : public QObject
{
	Q_OBJECT

public:
	iGlobal(QObject *parent=0);
	~iGlobal();

	 static iGlobal & Instance()
     {
        static iGlobal global;
        return global;
     }	 	 
	
	 QList<CVoltageLevel *> & getVoltagelevels(){return voltageLevels;}
	 QColor dccolor(){return m_dccolor;}
	 void setdccolor(QColor color){m_dccolor=color;}

	 QColor defaultcolor(){return m_defaultcolor;}
	 void setdefaultcolor(QColor color){m_defaultcolor=color;}

	 int  dcwidth(){return m_dcwidth;}
	 void setdcwidth(int width){m_dcwidth=width;}

	 bool iscolormap(){return m_iscolormap;}
	 void setcolormap(bool isapply){m_iscolormap=isapply;}

	 bool iswarning(){return m_iswarning;}
	 void setwarning(bool isapply){m_iswarning=isapply;}

	 bool readGlobalXmlfile();
	 void editOptions();

	 bool saveGlobal2Xml();

	 bool editvoltagelevel(CVoltageLevel *level,float voltage,int width,QColor color);
	 CVoltageLevel * newvoltagelevel(float voltage,int width,QColor color);
	 void deletevoltagelevel(CVoltageLevel *level);

	 ushort changes(){return m_changes;}
	 inline ErrorType errors(){ ErrorType temp=m_error; m_error=NoError;return temp;}

	 Language getlanguage() {return m_language;}
	 void setlanguage(Language lan);

public slots:
	 void Onchanges(ushort changes){emit globalChanged(); m_changes=changes;}

private:
	QList<CVoltageLevel *>	voltageLevels;	
	QColor m_dccolor;
	QColor m_defaultcolor;
	int m_dcwidth;
	bool m_iscolormap;
	bool m_iswarning;
	ErrorType m_error;

	QString defaultGlobalFile;
	QXmlStreamReader globalxmlReader;	
	void readVoltagelevel();
	void readOptions();
	void widthValidated(CVoltageLevel *level);	
	CVoltageLevel * findlevel(float voltage);
	ushort m_changes;	
	Language m_language;
	QTranslator *trans;
signals:
	void globalChanged(void);
};

#endif // IGLOBAL_H
