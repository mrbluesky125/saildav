/* This file is part of meedav
 *
 * Copyright (C) 2012 Timo Zimmermann <meedav@timozimmermann.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef Q_XML_STREAM_READER_BASE_H
#define Q_XML_STREAM_READER_BASE_H

#include <QtCore>
#include <QtDebug>

#include "QXmlBase.h"
#include <QXmlStreamReader>

template<typename XmlTagType>
void readXmlElement(QXmlStreamReader& reader, XmlTagType& dataObject) {
    qWarning() << "QXmlStreamReaderBase | No specialized reader function for that type of data is implemented. The current xml element is ignored.";
    reader.skipCurrentElement();
}

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

        return read(&xmlFile);
    }

    ///\brief Reads xml data from a io device
    bool read(QIODevice* device) {
        QXmlStreamReader::setDevice( device );

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

    ///\brief Reads data from the xml using the specialized readXmlElement() function for the data object
    ///\remarks Make sure to implement a specialized template function for the data type or the data can not be read.
    template<typename XmlTagType>
    void readElementObject(XmlTagType& dataObject) {
        readXmlElement<XmlTagType>(*this, dataObject);
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
