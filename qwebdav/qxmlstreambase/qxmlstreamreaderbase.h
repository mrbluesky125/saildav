#ifndef Q_XML_STREAM_READER_BASE_H
#define Q_XML_STREAM_READER_BASE_H

#include <QtCore>
#include <QtDebug>

#include "qxmlbase.h"

///\brief Base class template that provide a easy to use interface for a XML stream parser. The data object type is defined by the template argument.
///Derived classes just have to provide parsing functions for the different tags (the readRootTag() function must be reimplemented)
///and the actual DataType object (which is passed to the constructor). A simple implementation of a tag processing function consists just of 
///one while(readNextStartElement()) and one if/else-block. Remark that all nodes have to be processed (this is how the QXmlStreamReader works).
///This means unkown tags should be processed by the readUnknownElement() function, which skips the complete tag.
///
///Possible usage:
///\code
///class MyXmlReader : public QXmlStreamReaderBase<XmlData> {
///	  DataType m_data;
///	  void readRootTag(DataType& dataObject);
///public:
///	  MyXmlReader() :  QXmlStreamReaderBase<XmlData>(m_data) { }
///}
///\endcode
///
///\see QXmlStreamBase
///\author Timo Zimmermann
///\version 1.2
template<typename DataType>
class QXmlStreamReaderBase : virtual public QXmlBase<DataType>, public QXmlStreamReader
{
public:
    ///\brief
    QXmlStreamReaderBase(DataType& dataRef) : QXmlBase<DataType>() {
        QXmlBase<DataType>::m_dataRef = &dataRef;
    }

    ///\brief Tries to open and parse the given file.
    ///\param filename filname of the xml file
    bool read(const QString& filename) {
        QFile xmlFile(filename);
        if(!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "QXmlStreamReaderBase |" << QObject::tr("Failed to open the xml file:") << filename;
            return false;
        }

        return read(xmlFile);
    }

    ///\brief Reads xml data from a io device
    bool read(QIODevice& device) {
        QXmlStreamReader::setDevice( &device );

        while(readNextStartElement())
            readRootTag(QXmlBase<DataType>::dataRef());

        QXmlStreamReader::TokenType token = readNext();

        ///\bug maybe this is not the case, if the I/O device is a continuos stream, needs further
        ////checking if we reached the end of the document
        //if(token != QXmlStreamReader::EndDocument) {
        //	raiseError(QObject::tr("End of document expected but not reached."));
        //}

        //check if an error occured
        if(error() != NoError) {
            qWarning() << "QXmlStreamReaderBase |" << QObject::tr("Parsing failed. Error:") << errorString();
            qWarning() << "QXmlStreamReaderBase |" << QObject::tr("The xml data object is possibly in an undefined/uncomplete state.");
            return false;
        }

        return true;
    }

protected:
    ///\brief Parameterless constructor for derived classes, which have to set the data pointer manually
    QXmlStreamReaderBase() : QXmlBase<DataType>() { }

    virtual void readRootTag(DataType& dataObject) = 0;

    ///\brief Skipps a complete node. Should be called if the tag is unknown and could not be processed.
    void readUnknownElement() {
        qWarning() << "QXmlStreamReaderBase |" << QObject::tr("Unknown XML Element skipped:") << name();
        skipCurrentElement();
    }

    ///\brief Reads a qobject property of the xml data object
    ///\remarks The templated xml data object must be derived from QObject.
    void readPropertyElement() {
        QXmlBase<DataType>::m_dataRef.setProperty( qPrintable(name().toString()), QVariant::fromValue(readElementText()) );
    }
};

///\brief Deserializes the xml data directly from a data stream
///\remarks Ungetestet.
template<typename DataType>
QDataStream& operator>>(QDataStream& stream, QXmlStreamReaderBase<DataType>& item) {
    QByteArray xmlData;
    stream >> xmlData;

    QBuffer xmlBuffer(&xmlData);
    xmlBuffer.open(QBuffer::ReadOnly | QIODevice::Text);
    item.read(&xmlBuffer);

    return stream;
}

#endif // Q_XML_STREAM_READER_BASE_H
