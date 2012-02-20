#ifndef Q_XML_STREAM_WRITER_BASE_H
#define Q_XML_STREAM_WRITER_BASE_H

#include <QtCore>
#include <QtDebug>

#include "qxmlbase.h"

///\brief Base class template that provide a easy to use interface for a XML stream writer. The data object type is defined by the template argument.
///Derived classes just have to provide writing functions for the different tags (the writeRootTag() function must be reimplemented) and the actual 
///DataType object (which is passed to the constructor).
///
///Possible usage:
///\code
///class MyXmlWriter : public QXmlStreamWriterBase<XmlData> {
///	  DataType m_data;
///	  void writeRootTag(DataType& dataObject);
///public:
///	  MyXmlWriter() :  QXmlStreamWriterBase<XmlData>(m_data) { }
///}
///\endcode
///
///\see QXmlStreamBase
///\author Timo Zimmermann
///\version 1.2
template<typename DataType>
class QXmlStreamWriterBase : virtual public QXmlBase<DataType>, public QXmlStreamWriter
{
public:
    ///\brief Standard constructor
    QXmlStreamWriterBase(DataType& dataRef) : QXmlBase<DataType>() {
        QXmlBase<DataType>::m_dataRef = &dataRef;
    }

    ///\brief Tries to open and write the given file.
    ///\param filename filname of the xml file
    bool write(const QString& filename) {
        QFile xmlFile(filename);
        if(!xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qWarning() << "QXmlStreamWriterBase |" << QObject::tr("Failed to open the xml file:") << filename;
            return false;
        }

        write(xmlFile);
        return true;
    }

    ///\brief
    void write(QIODevice& device) {
        QXmlStreamWriter::setDevice( &device );
        setAutoFormatting(true);
        writeStartDocument();
        writeRootTag(QXmlBase<DataType>::dataRef());	//derived classes write their stuff
        writeEndDocument();
    }

protected:
    ///\brief Parameterless constructor for derived classes, which have to set the data pointer manually
    QXmlStreamWriterBase() : QXmlBase<DataType>() { }

    virtual void writeRootTag(DataType& dataObject) = 0;

    ///\brief Writes a qobject property of the xml data object
    ///\remarks The templated xml data object must be derived from QObject.
    void writePropertyElement(const char* propertyName) {
        writeTextElement(propertyName, QXmlBase<DataType>::m_dataRef.property(propertyName).toString());
    }
};

///\brief Serializes the xml data directly to a data stream
///\remarks Ungetestet.
template<typename DataType>
QDataStream& operator<<(QDataStream& stream, const QXmlStreamWriterBase<DataType>& item) {
    QByteArray xmlData;
    QBuffer xmlBuffer(&xmlData);
    xmlBuffer.open(QBuffer::WriteOnly | QIODevice::Text);
    item.write(&xmlBuffer);
    xmlBuffer.close();

    stream << xmlData;
    return stream;
}

#endif // Q_XML_WRITER_BASE_H
