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

#ifndef Q_XML_STREAM_BASE
#define Q_XML_STREAM_BASE

#include <QtCore>

#include "QXmlStreamReaderBase.h"
#include "QXmlStreamWriterBase.h"

///\brief Base class template that provide a easy to use interface for a XML stream parser and writer. The data object type is defined by the template argument.
///Derived classes just have to provide parsing and writing functions for the different tags (the readRootTag() and writeRootTag() functions must be reimplemented)
///and the actual DataType object (which is passed to the constructor).
///Its also possible to define the derived class as data object at the same time. So no extra data class is needed.
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
