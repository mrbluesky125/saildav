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

#ifndef Q_XML_BASE_H
#define Q_XML_BASE_H

#if (_MSC_VER < 1600)
#define nullptr 0
#endif

///\brief Base class for the xml stream reader and writer.
///
///\see QXmlStreamBase
///\author Timo Zimmermann
///\version 1.2
template<typename DataType>
class QXmlBase {

protected:
    DataType* m_dataRef;	///<Pointer to the data structure that must be provided by derived classes

public:
    ~QXmlBase() { }

    ///\brief Returns a const reference to the xml data
    const DataType& xmlData() const { return *m_dataRef; }

protected:
    QXmlBase() : m_dataRef(nullptr) { }

    ///brief returns a reference to the data object
    DataType& dataRef() { return *m_dataRef; }
};


#endif
