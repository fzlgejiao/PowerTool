#ifndef IGLOBAL_H
#define IGLOBAL_H

#include <QObject>
#include <QMap>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QColor>

typedef enum 
{
	NoError =0,
	VoltageError,
	WidthError
}ErrorType;

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
	 QColor defaultcolor(){return m_defaultcolor;}
	 int  dcwidth(){return m_dcwidth;}
	 bool iscolormap(){return m_iscolormap;}
	 bool iswarning(){return m_iswarning;}

	 bool readGlobalXmlfile();
	 void editOptions();

	 bool saveGlobal2Xml();

	 bool editvoltagelevel(CVoltageLevel *level,float voltage,int width,QColor color);
	 CVoltageLevel * newvoltagelevel(float voltage,int width,QColor color);
	 void deletevoltagelevel(CVoltageLevel *level);

	 inline ErrorType errors(){ ErrorType temp=m_error; m_error=NoError;return temp;}
	
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

signals:
	 void colorMapChanged(bool iscolormap);
	 void warningDialog(bool iswarning);
	 void voltagelevelsChanged();
};

#endif // IGLOBAL_H
