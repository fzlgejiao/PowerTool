#include "iglobal.h"
#include <QDebug>
#include <iostream>
#include <QFile>



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
	,defaultGlobalFile("./iGlobal.xml")
{
	m_error=NoError;
	m_iscolormap=false;
	m_iswarning=false;
	m_dcwidth=0;
	m_dccolor=QColor(Qt::black);
	m_defaultcolor=QColor(Qt::black);
	voltageLevels.clear();
	if(readGlobalXmlfile())
		qSort(voltageLevels.begin(),voltageLevels.end(),LevelvoltageAscending);
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


