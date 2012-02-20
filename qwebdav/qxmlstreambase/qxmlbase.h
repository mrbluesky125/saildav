#ifndef Q_XML_BASE_H
#define Q_XML_BASE_H

///\brief Base class for the xml stream reader and writer.
///
///\see QXmlStreamBase
///\author Timo Zimmermann
///\version 1.2
template<typename DataType>
class QXmlBase {

protected:
    DataType* m_dataRef; ///<Pointer to the data structure that must be provided by derived classes

public:
    ~QXmlBase() { }

    ///\brief Returns a const reference to the xml data
    const DataType& xmlData() const { return *m_dataRef; }

protected:
    QXmlBase() : m_dataRef(0) { }

    ///brief returns a reference to the data object
    DataType& dataRef() { return *m_dataRef; }
};


#endif
