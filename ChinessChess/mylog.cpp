#include "mylog.h"
#include<QString>
#include<QDir>
#include<QCoreApplication>

void initGLOG(char* appName)
{
    QString dirStr = QCoreApplication::applicationDirPath();
    QDir dir = QDir(dirStr);
    if (!dir.exists("Logs"))
    {
        dir.mkdir("Logs");
    }
    dir.cd("Logs");
    std::string logFilePath = dir.path().toStdString();
    FLAGS_log_dir = logFilePath;
    google::InitGoogleLogging(appName);
    google::SetLogDestination(google::GLOG_INFO, (logFilePath + "/INFO_").c_str());
    google::SetLogDestination(google::GLOG_WARNING, (logFilePath + "/WARNING_").c_str());
    google::SetLogDestination(google::GLOG_ERROR, (logFilePath + "/ERROR_").c_str());
    google::SetLogDestination(google::GLOG_FATAL, (logFilePath + "/FATAL_").c_str());
    google::SetStderrLogging(google::GLOG_INFO);
    google::SetLogFilenameExtension("log_");
    FLAGS_colorlogtostderr = true;  // Set log color
    FLAGS_logbufsecs = 0;  // Set log output speed(s)
    FLAGS_max_log_size = 1024;  // Set max log file size
    FLAGS_stop_logging_if_full_disk = true;  // If disk is full

}

void unInitGLOG()
{
    google::ShutdownGoogleLogging();
}
