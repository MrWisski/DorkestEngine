/*
    Name : Dorkest Log System
    Created : 5/26/2021
    Author : MrWisski
    License : (WISSKI-1)

    Copyright 2012 - 2021 Alex Wisnieski AKA MrWisski

    Permission is hereby granted, free of charge, to any person (hereafter "You") obtaining a copy or portion of this software, its resulting
    compilation products, or associated documentation files (hereafter "Software"), to deal in the Software with the following
    restrictions, using the following terminology :

    TERMINOLOGY :
        1) COMMERCIAL : Any use of the Software in which You, or others recieve compensation for any reason, of any kind.

    RESTRICTIONS :
        1) You MAY use, copy, publish, distribute, and/or merge, this Software, with any project or library or software,
           that is not COMMERCIAL in nature or intent.

        2) You MAY NOT use, copy, publish, distribute, redistribute, sublicense, merge and/or sell copies of any software including partially,
           containing whole, or any portion of the Software, nor permit persons to whom the Software is furnished to do so for COMMERCIAL
           uses.

        3) Derivations of the Software are permitted, provided the above copyright notice and this permission notice are NOT
           modified, with the exception of character encoding changes that do not substantially change the intent of the notice or the
           above copyright notice, or the readibility or useability of this permission notice or the above copyright notice, except for
           COMMERCIAL uses.

        4) You MAY NOT imply endorsement of any product that includes or derives from the Software, by any any person or group of people
           involved in the creation of the Software without direct written permission from said persons or group of people.

        5) You MAY NOT use the legal names, handles, aliases, or nicknames of any person involved in the creation of the Software,
           to promote any product derived from, or using the Software in any way, without written permission from said persons.

    The above copyright notice and this permission notice shall be included in all copies or portions of the Software.

    Any of the above restrictions which are unlawful or not permitted, whole or in part, in the jurisdiction You live in will not
    affect the rest of the portion of the restriction it exists in, the other restrictions listed in this permission notice.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

    CERTAIN JURISDICTIONS DO NOT ALLOW LIMITATIONS ON IMPLIED WARRANTIES OR THE EXCLUSION OR LIMITATION OF CERTAIN DAMAGES.
    IF YOU RESIDE IN SUCH A JURISDICTION, SOME OR ALL OF THE ABOVE DISCLAIMERS, EXCLUSIONS, OR LIMITATIONS MAY NOT APPLY TO YOU,
    AND YOU MAY HAVE ADDITIONAL RIGHTS. THE LIMITATIONS OR EXCLUSIONS OF WARRANTIES, REMEDIES, OR LIABILITY CONTAINED IN THESE TERMS
    APPLY TO YOU TO THE FULLEST EXTENT SUCH LIMITATIONS OR EXCLUSIONS ARE PERMITTED UNDER THE LAWS OF THE JURISDICTION IN WHICH
    YOU ARE LOCATED.
*/

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

