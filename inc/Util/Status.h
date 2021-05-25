/*
 * Status.h
 *
 *  Created on: Dec 3, 2020
 *      Author: USERNAME
 */

#ifndef SRC_UTIL_STATUS_H_
#define SRC_UTIL_STATUS_H_

#include <string>
#include <sstream>

using namespace std;

class Status
{
public:
    enum State
    {
        ERRORED = -1,
        UNINIT = 0,
        INIT = 1,
        OK = 2,
        DISPOSED = 3
    };

    State getState()
    {
        return curStatus;
    }
    string toString(State status)
    {
        if(status == State::DISPOSED)
        {
            return "Disposed";
        }
        if(status == State::ERRORED)
        {
            return "Errored";
        }
        if(status == State::INIT)
        {
            return "Initialized";
        }
        if(status == State::OK)
        {
            return "OK";
        }
        if(status == State::UNINIT)
        {
            return "Uninitialized";
        }
        return "";
    }

    string wrongState(State desiredState)
    {
        std::stringstream ss;
        ss << "Require state " << toString(desiredState) << ", but currently in state " << toString(curStatus);
        return ss.str();
    }
    void Allocate()
    {
        curStatus = State::UNINIT;
    }
    void Initialize()
    {
        curStatus = State::INIT;
    }
    void Ready()
    {
        curStatus = State::OK;
    }
    void Dispose()
    {
        curStatus = State::DISPOSED;
    }
    void Error(int code, string msg)
    {
        curStatus = State::ERRORED;
        eCode = code;
        eMsg = msg;
    }

    int getErrorCode()
    {
        return eCode;
    }
    string getErrorMessage()
    {
        return eMsg;
    }
private:
    State curStatus = Status::UNINIT;
    long eCode = 0;
    string eMsg = "OK";

};




#endif /* SRC_UTIL_STATUS_H_ */
