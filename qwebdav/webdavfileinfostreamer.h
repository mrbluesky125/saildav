#ifndef WEBDAVFILEINFOSTREAMER_H
#define WEBDAVFILEINFOSTREAMER_H

#include "qxmlstreambase.h"
#include "webdavfileinfo.h"

class QWebdavFileInfoStreamer : public QXmlStreamBase<QWebdavUrlInfo>
{
public:
    QWebdavFileInfoStreamer(QWebdavUrlInfo& data);

    void readRootTag(QWebdavUrlInfo& dataObject);
    void writeRootTag(QWebdavUrlInfo& dataObject);
};

#endif // WEBDAVFILEINFOSTREAMER_H
