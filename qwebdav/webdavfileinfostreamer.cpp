#include "webdavfileinfostreamer.h"

QWebdavFileInfoStreamer::QWebdavFileInfoStreamer(QWebdavUrlInfo& data) : QXmlStreamBase<QWebdavUrlInfo>(data)
{

}

void QWebdavFileInfoStreamer::readRootTag(QWebdavUrlInfo& dataObject)
{
    while (readNextStartElement()) {
//        if( name() == "X" ) dataObject.m_x = readElementText();
//        else if ...

//        else
//            readUnknownElement();
    }
}

void QWebdavFileInfoStreamer::writeRootTag(QWebdavUrlInfo& dataObject)
{
//    writeStartElement("myXml");
//    writeTextElement("X", dataObject.m_x);
//    writeEndElement();
}
