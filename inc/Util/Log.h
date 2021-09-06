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

#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <ostream>
#include <sstream>

//Make some global defines to make logging simple and painless.
#define error(msg) Log::Instance()->_error(msg, __FUNCTION__);
#define info(msg) Log::Instance()->_info(msg, __FUNCTION__);
#define debug(msg) Log::Instance()->_debug(msg, __FUNCTION__);

#define debugss *(Log::Instance()->d_stream())
#define infoss *(Log::Instance()->i_stream())
#define errorss *(Log::Instance()->e_stream())


class Log
{
public:
    
    using Buffer_p = std::unique_ptr<std::ostringstream, std::function<void(std::ostringstream*)>>;

    Buffer_p d_stream() {
        return Buffer_p(new std::ostringstream, [&](std::ostringstream* st) {
            *st << std::endl;
            if (Log::Instance()->debugflags.end() == Log::Instance()->debugflags.find("[DEBUG STREAM]")) Log::Instance()->debugflags["[DEBUG STREAM]"] = true;
            logStream(DEBUG, st->str());
            });
    }

    Buffer_p i_stream() {
        return Buffer_p(new std::ostringstream, [&](std::ostringstream* st) {
            if (Log::Instance()->infoflags.end() == Log::Instance()->infoflags.find("[INFO STREAM]")) Log::Instance()->infoflags["[INFO STREAM]"] = true;
            logStream(INFO, st->str());
            });
    }

    Buffer_p e_stream() {
        return Buffer_p(new std::ostringstream, [&](std::ostringstream* st) {

            if (Log::Instance()->errorflags.end() == Log::Instance()->errorflags.find("[ERROR STREAM]")) Log::Instance()->errorflags["[ERROR STREAM]"] = true;
            logStream(ERR, st->str());
            });
    }


    enum log_type
    {
        ERR, INFO, DEBUG
    };

    static Log* Instance()
    {
        if (instance == 0)
        {
            instance = new Log();
        }

        return instance;
    }

    void _info(std::string msg, std::string caller);
    void _debug(std::string msg, std::string caller);
    void _error(std::string msg, std::string caller);
    

    void save();
    void disableEntry(std::string cname);
    void enableEntry(std::string cname);
    void toggleEntry(std::string cname);



    void toConsole(bool state)
    {
        console = state;
    }
    void clear()
    {
        m_messages.clear();
    }
    void cleanup()
    {
        save();
        clear();
    }
    std::vector< std::string > getMessages() const
    {
        return m_messages;
    }

    void setLogDirectory(std::string directory)
    {
        dir = directory;
    }
    void setLogFileName(std::string filename)
    {
        fName = filename;
    }

private:
    bool vis = false;
    std::string dir;
    std::string fName;
    //Does the actual heavy lifting of saving the log messages to our std::vector
    void log(log_type p_type, std::string caller,std::string p_msg);
    void logStream(log_type p_type, std::string p_msg);
    //Our instance.
    static Log* instance;
    //bool to indicate if we're outputting to the console.
    bool console = true;

    //Vector saving our log messages
    std::vector<std::string> m_messages;
    //a std::map of classnames to a flag if we're logging this module.
    std::map<std::string, bool > logflags;
    //a std::map of classnames to a flag if we're logging error messages for this module
    std::map<std::string, bool > errorflags;
    //a std::map of classnames to a flag if we're logging info messages for this module
    std::map<std::string, bool > infoflags;
    //a std::map of classnames to a flag if we're logging debug messages for this module
    std::map<std::string, bool > debugflags;

    Log() {
        
       
    }
    ~Log() {
        
     
    }

};

typedef Log Log;
