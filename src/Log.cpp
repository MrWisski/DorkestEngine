#include "Util/Log.h"
#include <iostream>
#include <fstream>
#include <time.h>

Log* Log::instance = 0;

void Log::_error(std::string msg, std::string caller)
{
    if(caller == "::")
    {
        caller = "Main";
    }
    if(Log::Instance()->errorflags.end() == Log::Instance()->errorflags.find(caller))
    {
        Log::Instance()->errorflags[caller] = true;
    }

    Log::Instance()->log(ERR, caller, msg);
}

void Log::_info(std::string msg, std::string caller)
{
    if(caller == "::")
    {
        caller = "Main";
    }
    if(Log::Instance()->infoflags.end() == Log::Instance()->infoflags.find(caller))
    {
        Log::Instance()->infoflags[caller] = true;
    }

    Log::Instance()->log(INFO, caller, msg);
}
void Log::_debug(std::string msg, std::string caller)
{
    if(caller == "::")
    {
        caller = "Main";
    }
    if(Log::Instance()->debugflags.end() == Log::Instance()->debugflags.find(caller))
    {
        Log::Instance()->debugflags[caller] = true;
    }

    Log::Instance()->log(DEBUG, caller, msg);
}

void Log::render()
{

    if (this->vis) {
        *open = true;
        this->imguiconsole->Draw("dorkestEngine Debug Console", open);
        if (*open == false) {
            this->vis = false;
        }
    }
}

void Log::show()
{
    this->vis = true;
}

void Log::hide()
{
    this->vis = false;
}

void Log::toggleShowHide()
{
    this->vis = !this->vis;
}

void Log::log(log_type p_type, std::string caller,std::string p_msg)
{
    if(caller == "::")
    {
        std::cout << "setting caller to Main\n";
        caller = "Main";
        std::cout << "Caller set to main. \n";
    }
    if(Log::Instance()->logflags.end() == Log::Instance()->logflags.find(caller))
    {
        Log::Instance()->logflags[caller] = true;
    }

    //Are we even saving the logs from this module??
    if(Log::Instance()->logflags[caller] == false)
    {
        //std::cout << "We ARE NOT saving logs from " + caller << std::endl;
        return;
    }
    //std::cout << "We ARE saving logs from " + caller << std::endl;

    //Are we even saving the specific type logs from this module??
    switch (p_type)
    {
    case ERR:
        if(Log::Instance()->errorflags[caller] == false)
        {
            std::cout << "Aborting error message." << std::endl;
            return;
        }

        break;
    case INFO:
        if(Log::Instance()->infoflags[caller] == false)
        {
            std::cout << "Aborting info message." << std::endl;
            return;
        }

        break;
    case DEBUG:
        if(Log::Instance()->debugflags[caller] == false)
        {
            std::cout << "Aboorting debug message." << std::endl;
            return;
        }

        break;
    }

    //std::cout << "Building time info.\n";
    time_t rawtime;
    struct tm timeinfo;
    char buffer[80];
    std::string timestr;

    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    strftime(buffer, 80, "%I:%M:%S", &timeinfo);

    std::string str(buffer);
    timestr = buffer;

    std::string msg = "";

    switch (p_type)
    {
    case ERR:
        msg += "[ERROR]\t";
        break;
    case INFO:
        msg += "[Info]\t";
        break;
    case DEBUG:
        msg += "[Debug]\t";
        break;
    }

    msg += caller + " - " + p_msg;
    if(Log::Instance()->console == true)
    {
        std::cout << msg << std::endl;
    }
    m_messages.push_back(timestr+msg);
    imguiconsole->AddLog(msg.c_str());
}

void Log::save()
{
    time_t rawtime;
    struct tm timeinfo;
    char buffer[80];

    time(&rawtime);
    localtime_s(&timeinfo,&rawtime);


    if (Log::instance->fName == "")
    {
        strftime(buffer, 80, "%d-%m-%Y_%I-%M-%S.log", &timeinfo);
        std::string str(buffer);
        fName = buffer;

    }
    if (Log::instance->dir == "")
    {
        dir = "Logs/";
    }

    std::string fileName = dir + fName;

    std::ofstream logFile;
    logFile.open(fileName);

    for (unsigned int i = 0; i < m_messages.size(); ++i)
    {
        logFile << m_messages[i] << "\n";
    }

    logFile.close();

}

void Log::disableEntry(std::string cname)
{
    Log::Instance()->logflags[cname] = false;
}
void Log::enableEntry(std::string cname)
{
    Log::Instance()->logflags[cname] = true;

}
void Log::toggleEntry(std::string cname)
{
    Log::Instance()->logflags[cname] = !Log::Instance()->logflags[cname];

}

