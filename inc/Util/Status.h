/*
    Name : Dorkest Status System
    Created : 5/26/2021
    Author : MrWisski
    License : 
    (WISSKI-1)

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
