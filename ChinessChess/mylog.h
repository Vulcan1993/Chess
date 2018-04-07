#ifndef _MYTRACER_H_20180405_
#define _MYTRACER_H_20180405_
#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES

#include"glog/logging.h"
#include<string>
#include<iostream>

void initGLOG(char* appName);

void unInitGLOG();

//代码跟踪器

#define TRACER \
    MyTracer myTracer(__FUNCTION__,__FILE__)
class MyTracer
{
public:
    inline MyTracer(std::string msg = "", std::string file = "") : m_msg(msg), m_file(file)
    {
        LOG(WARNING) << "Enter : " << m_msg << "  " << m_file;
    }
    inline ~MyTracer()
    {
        LOG(WARNING) << "Leave : " << m_msg<<"  "<<m_file;
    }

private:
    std::string m_msg;
    std::string m_file;
};

#endif