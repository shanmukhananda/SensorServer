#pragma once

#include <QDebug>
#include <QFileInfo>

#include <sstream>
#include <thread>

namespace log_util {
class block_info {
public:
    explicit block_info(const QString& name) : _name(name) {
        qDebug() << "==>" << _name.toStdString().c_str();
    }
    ~block_info() {
        qDebug() << "<==" << _name.toStdString().c_str();
    }

private:
    QString _name;
}; // class block_info

inline QString tid_to_string(const std::thread::id& tid) {
    std::stringstream ss;
    ss << tid;
    return QString(ss.str().c_str());
}
} // namespace log_util

#define CALL_SITE                                                        \
    QString("[" + QFileInfo(__FILE__).fileName() + "(" +                 \
            QString::number(__LINE__) + ")" + ":" +                      \
            log_util::tid_to_string(std::this_thread::get_id()) + +":" + \
            QString(Q_FUNC_INFO) + "]")

#define LOG_SCOPE log_util::block_info __block_info__(CALL_SITE)
#define LOG_ERROR qCritical() << CALL_SITE.toStdString().c_str()
#define LOG_WARNING qWarning() << CALL_SITE.toStdString().c_str()
#define LOG_INFO qInfo() << CALL_SITE.toStdString().c_str()
#define LOG_DEBUG qDebug() << CALL_SITE.toStdString().c_str()
