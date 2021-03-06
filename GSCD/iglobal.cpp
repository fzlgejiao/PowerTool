#include "iglobal.h"
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QApplication>
#include <QtXml>
#include <QFont>
CVoltageLevel::CVoltageLevel(float voltage,int width,QColor color)
{	
	m_refvoltage=voltage;
	m_width=width;
	m_color=color;
}

bool LevelvoltageAscending(CVoltageLevel *level1,CVoltageLevel *level2)
{
	return (level1->refVoltage() < level2->refVoltage());
}

iGlobal::iGlobal(QObject *parent)
	: QObject(parent)
	,defaultGlobalFile("./GSCD.xml")
{
	m_language=Chinese;
	trans=new QTranslator(this);
	m_changes=0;
	m_error=NoError;
	m_iscolormap=false;
	m_iswarning=false;
	m_dcwidth=0;
	m_dccolor=QColor(Qt::black);
	m_defaultcolor=QColor(Qt::black);
	voltageLevels.clear();
	if(readGlobalXmlfile())
	{ 
		qSort(voltageLevels.begin(),voltageLevels.end(),LevelvoltageAscending);		
	}
	if(m_language==Chinese)	
		trans->load("GSCD_CN.qm",":/language");	
	else
		trans->load("GSCD_EN.qm",":/language");
	QApplication::installTranslator(trans);
	QFont font("Arial",9);	
	QApplication::setFont(font);
}

iGlobal::~iGlobal()
{

}

CVoltageLevel * iGlobal::findlevel(float voltage)
{
	if(voltageLevels.isEmpty())
		return NULL;
	foreach(CVoltageLevel* level, voltageLevels)
	{
		if(level->m_refvoltage == voltage)
			return level;
	}
	return NULL;
}
bool iGlobal::readGlobalXmlfile()
{
	QFile file(defaultGlobalFile);
	if(!file.exists()) {
			saveGlobal2Xml();
			return false;
	}
	
	if (!file.open(QFile::ReadWrite | QFile::Text)) {
         std::cerr << "Error: Cannot write file "                
                  << qPrintable(file.errorString()) << std::endl;
		return false;
    }
	voltageLevels.clear();
	globalxmlReader.setDevice(&file);
  
	while (!globalxmlReader.atEnd()) 
	{
		QXmlStreamReader::TokenType type=globalxmlReader.readNext();

		if(type==QXmlStreamReader::StartElement) 
		{
			if(globalxmlReader.name() == "levels")
			{
				readVoltagelevel();
			}else if(globalxmlReader.name() == "options")
			{	
				readOptions();
			}
		}
	}

	file.close();

    if (globalxmlReader.hasError()) {
		QString ss=globalxmlReader.errorString();
        std::cerr << "Error: Failed to parse file "                 
                  << qPrintable(globalxmlReader.errorString()) << std::endl;
        return false;
    } else if (file.error() != QFile::NoError) {
        std::cerr << "Error: Cannot read file " 
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }	
    return true;
}
bool iGlobal::saveGlobal2Xml()
{
	QFile file(defaultGlobalFile);
    if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        std::cerr << "Error: Cannot write file "                
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

	xmlWriter.writeStartElement("Global");
	xmlWriter.writeAttribute("version", "1.0");
	//******************voltage levels------start************/
    xmlWriter.writeStartElement("levels");	
	foreach(CVoltageLevel *level,voltageLevels)
	{
		xmlWriter.writeStartElement("level");
		xmlWriter.writeAttribute("voltage", QString::number(level->m_refvoltage));			
		xmlWriter.writeAttribute("width", QString::number(level->m_width));	
		xmlWriter.writeAttribute("color", level->m_color.name());			
		xmlWriter.writeEndElement();
	}	
	xmlWriter.writeEndElement();
	//******************voltage levels------end************/

	//******************Options------start************/
	xmlWriter.writeStartElement("options");
	xmlWriter.writeTextElement("colormap", m_iscolormap?"true":"false");
	xmlWriter.writeTextElement("warning", m_iswarning?"true":"false");
	xmlWriter.writeTextElement("dcwidth", QString::number(m_dcwidth));
	xmlWriter.writeTextElement("dccolor", m_dccolor.name());
	xmlWriter.writeTextElement("defaultcolor", m_defaultcolor.name());
	xmlWriter.writeTextElement("language", QString::number(m_language));
	xmlWriter.writeEndElement();
	//******************Options------end************/
	xmlWriter.writeEndElement();								//match root element("Global");

    xmlWriter.writeEndDocument();
    file.close();
    if (file.error()) {
        std::cerr << "Error: Cannot write file "                
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    return true;
}
void iGlobal::readVoltagelevel()
{
	int index=0;	
	while (!globalxmlReader.atEnd()) {		
		QXmlStreamReader::TokenType type=globalxmlReader.readNext();
		if (type==QXmlStreamReader::EndElement && globalxmlReader.name()=="levels" ){		
			break;
		}

		if (type==QXmlStreamReader::StartElement) {
			if (globalxmlReader.name() == "level") {				
				float	refvoltage = globalxmlReader.attributes().value("voltage").toString().toFloat();
				int width = globalxmlReader.attributes().value("width").toString().toInt();
				QString colorname=globalxmlReader.attributes().value("color").toString();

				if( (refvoltage!=0)||(width!=0)||(QColor(colorname).isValid()))							//data validation
				{
					CVoltageLevel *level=new CVoltageLevel(refvoltage,width,QColor(colorname));				
					voltageLevels.append(level);
					index++;
				}				
			}	
			else
				 globalxmlReader.raiseError("Element Name Error ");    
		} 		
	}
}

void iGlobal::readOptions()
{
	while (!globalxmlReader.atEnd()) 
	{		
		QXmlStreamReader::TokenType type=globalxmlReader.readNext();
		if (type==QXmlStreamReader::EndElement && globalxmlReader.name()=="options" ){		
			break;
		}

		if (type==QXmlStreamReader::StartElement)  
		{
			if(globalxmlReader.name()=="colormap")
				m_iscolormap=QVariant(globalxmlReader.readElementText(QXmlStreamReader::SkipChildElements)).toBool();	
			else if(globalxmlReader.name()=="warning")
				m_iswarning=QVariant(globalxmlReader.readElementText(QXmlStreamReader::SkipChildElements)).toBool();
			else if(globalxmlReader.name()=="dcwidth")
				m_dcwidth=globalxmlReader.readElementText(QXmlStreamReader::SkipChildElements).toInt();
			else if(globalxmlReader.name()=="dccolor")
				m_dccolor=globalxmlReader.readElementText(QXmlStreamReader::SkipChildElements);
			else if(globalxmlReader.name()=="defaultcolor")
				m_defaultcolor=globalxmlReader.readElementText(QXmlStreamReader::SkipChildElements);
			else if(globalxmlReader.name()=="language")
				m_language=(Language)globalxmlReader.readElementText(QXmlStreamReader::SkipChildElements).toInt();
			else 
				globalxmlReader.raiseError("Unknown options element ");    
		} 		
	}
}

void iGlobal::editOptions()
{


}

bool iGlobal::editvoltagelevel(CVoltageLevel *level,float voltage,int width,QColor color)
{
	CVoltageLevel *find=findlevel(voltage);
	if(find && find!=level ){
			m_error=VoltageError;
			return false;
	}
	
	level->m_refvoltage=voltage;
	level->m_width=width;
	level->m_color=color;
	qSort(voltageLevels.begin(),voltageLevels.end(),LevelvoltageAscending);
	widthValidated(level);
	return true;
}

 CVoltageLevel * iGlobal::newvoltagelevel(float voltage,int width,QColor color)
{
	CVoltageLevel *find=findlevel(voltage);
	if(find){
			m_error=VoltageError;
			return NULL;
	}	
	CVoltageLevel *newlevel=new CVoltageLevel(voltage,width,color);	
	voltageLevels.append(newlevel);
	//when add new node, sort all by voltage
	qSort(voltageLevels.begin(),voltageLevels.end(),LevelvoltageAscending);
	widthValidated(newlevel);

	return newlevel;
}
void iGlobal::deletevoltagelevel(CVoltageLevel *level)
{
	voltageLevels.removeOne(level);
}

void iGlobal::widthValidated(CVoltageLevel *newlevel)
{
	if(voltageLevels.count()==1) return ;					//it's always OK when only one level

	int newlevelindex=voltageLevels.indexOf(newlevel);
	int width=newlevel->m_width;
	if(newlevelindex==0)
	{
		auto largeone=voltageLevels.at(newlevelindex+1);
		if(largeone->m_width < width)
		{
			newlevel->m_width=largeone->m_width;
			m_error=WidthError;
		}
	}else if(newlevelindex==voltageLevels.count()-1)
	{
		auto smallerone=voltageLevels.at(newlevelindex-1);
		if(smallerone->m_width > width)
		{
			newlevel->m_width=smallerone->m_width;
			m_error=WidthError;
		}
	}else
	{
		auto largeone=voltageLevels.at(newlevelindex+1);
		auto smallerone=voltageLevels.at(newlevelindex-1);
		if( (largeone->m_width < width) || (smallerone->m_width > width) )
		{
			newlevel->m_width=smallerone->m_width;
			m_error=WidthError;
		}
	}
}
bool iGlobal::setlanguage(Language lan)
{
	if(m_language==lan) return false;
	m_language=lan;
	//save to XML file
	QFile file(defaultGlobalFile);
	QDomDocument  root;
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
         std::cerr << "Error: Cannot open file "                
                  << qPrintable(file.errorString()) << std::endl;
		return false;
    }
	if(!root.setContent(&file)) 
	{
		file.close();
		return false;
	}
	file.close();
	QDomNodeList lan_node=root.documentElement().elementsByTagName("language");
	if(lan_node.size()>1) return false;
	else if(lan_node.size()==0)					//if language option is not exist ,new it
	{
		QDomElement root_el=root.documentElement();
		QDomElement option_el=root.documentElement().elementsByTagName("options").at(0).toElement();
		QDomElement lan_el=root.createElement("language");
		lan_el.appendChild(root.createTextNode(QString::number(m_language)));
		option_el.appendChild(lan_el);
		root_el.appendChild(option_el);
	}else if(lan_node.size()==1)	
	{
		QDomNode oldnode=lan_node.at(0).firstChild();
		QDomElement el = lan_node.at(0).toElement();
		el.firstChild().setNodeValue(QString::number(m_language));
		QDomNode newnode = el.firstChild(); 
		el.replaceChild(newnode,oldnode);
	}else 
		return false;
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return false;
	}
	QTextStream out(&file);
	root.save(out,4);
	file.close();
	//set application to target language	 
	if(m_language==Chinese)	
		trans->load("GSCD_CN.qm",":/language");	
	else
		trans->load("GSCD_EN.qm",":/language");	

	return true;
}


