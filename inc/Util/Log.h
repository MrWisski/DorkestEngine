#pragma once

#include <vector>
#include <string>
#include <map>

#include "ConsoleIMGUI.h"


//Make some global defines to make logging simple and painless.
#define error(msg) Log::Instance()->_error(msg, __FUNCTION__);
#define info(msg) Log::Instance()->_info(msg, __FUNCTION__);
#define debug(msg) Log::Instance()->_debug(msg, __FUNCTION__);


class Log
{
public:
   
    AppConsole* imguiconsole;

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

    void render();
    void show();
    void hide();
    void toggleShowHide();
    bool* open;

private:
    bool vis = false;
    std::string dir;
    std::string fName;
    //Does the actual heavy lifting of saving the log messages to our std::vector
    void log(log_type p_type, std::string caller,std::string p_msg);
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
        imguiconsole = new AppConsole();
        open = new bool(false);
    }
    ~Log() {
        delete imguiconsole;
        delete open;
    }

};

typedef Log Log;
