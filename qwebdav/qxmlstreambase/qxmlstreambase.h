#ifndef Q_XML_STREAM_BASE
#define Q_XML_STREAM_BASE

#include <QtCore>

#include "qxmlstreamreaderbase.h"
#include "qxmlstreamwriterbase.h"

///\brief Base class template that provide a easy to use interface for a XML stream parser and writer. The data object type is defined by the template argument.
///Derived classes just have to provide parsing and writing functions for the different tags (the readRootTag() and writeRootTag() functions must be reimplemented)
///and the actual DataType object (which is passed to the constructor).
///It is also possible to define the derived class as data object at the same time. So no extra data class is needed.
///
///Possible usage:
///\code
///class MyXmlStream : public QXmlStreamBase<MyXmlStream> {
///	  void readRootTag(DataType& dataObject) {
///		while (readNextStartElement()) {
///				if( name() == "X" ) dataObject.m_x = readElementText();
///				else if ...
///					...
///				else
///					readUnknownElement();
///			}
///		}
///
///		void writeRootTag(DataType& dataObject) {
///			writeStartElement("myXml");
///			writeTextElement("X", dataObject.m_x);
///			...
///			writeEndElement();
///		}
///public:
///	  MyXmlStream(DataType& m_data) :  QXmlStreamBase<XmlData>(*this) { }
///}
///
///main() {
///	MyXmlStream stream;
///	stream.read("myXml.xml");
/// stream.setValueX(blubb);
///	stream.write("newXml.xml");
///}
///
///\endcode
///
///\author Timo Zimmermann
///\version 1.2
template<typename DataType>
class QXmlStreamBase : public QXmlStreamReaderBase<DataType>, public QXmlStreamWriterBase<DataType> 
{
public:
    ///\brief
    QXmlStreamBase(DataType& dataRef) : QXmlBase<DataType>(), QXmlStreamReaderBase<DataType>(), QXmlStreamWriterBase<DataType>() {
        QXmlBase<DataType>::m_dataRef = &dataRef; //set the member manually to avoid multiple initialisations by base class constructors
    }
};


#endif
