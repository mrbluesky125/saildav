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

#ifndef Q_XML_STREAM_WRITER_BASE_H
#define Q_XML_STREAM_WRITER_BASE_H

#include <QtCore>
#include <QtDebug>

#include "QXmlBase.h"
#include <QXmlStreamWriter>
#include <QXmlStreamAttribute>

template<typename XmlTagType>
void writeXmlElement(QXmlStreamWriter& writer, XmlTagType& dataObject) {
    qWarning() << "QXmlStreamWriterBase | No specialized write function for that type of data is implemented. The data is not written to the xml!";
}

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

        write(&xmlFile);
        return true;
    }

    ///\brief
    void write(QIODevice* device) {
        QXmlStreamWriter::setDevice( device );
        setAutoFormatting(true);
        writeStartDocument();
        writeRootTag(QXmlBase<DataType>::dataRef());	//derived classes write their stuff
        writeEndDocument();
    }

    ///\brief Writes data to the xml using the specialized writeXmlElement() function for the data object
    ///\remarks Make sure to implement a specialized template function for the data type or no data will be written.
    template<typename XmlTagType>
    void writeObjectElement(const QString& qualifiedName, XmlTagType& dataObject) {
        writeStartElement(qualifiedName);
        writeXmlElement(*this, dataObject);
        writeEndElement();
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
