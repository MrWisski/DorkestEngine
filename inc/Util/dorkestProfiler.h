/*
	Name : Dorkest Profiler System
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
#include "Log.h"
#include <windows.h>
#include <profileapi.h>
#include <errhandlingapi.h>

/// <summary>
/// The main profiling class. 
/// This singleton contains all the machinery to recognize,
///catalog, and report on DataPoint tags you've created. The tags persist, so every
///time execution loops through your datapoint constructor and destructor, a new
///datapoint is collected!
/// 
/// NOTE : I really won't make any assurances as to the
///speed of the profiler.Really, I don't nest datapoints, or I take them with a LARGE
///grain of sugar - profiler is NOT optimized, and only the area between construction of
///the datapoint, and the destruction of the datapoint, will be even marginally accurate.
/// </summary>
class dorkestProfiler {
public:

	/// <summary>
	/// Singleton machinery. Call this to get a pointer to the global singleton.
	/// </summary>
	/// <returns>dorkestProfiler* to the global singleton instance</returns>
	static dorkestProfiler* getInstance() {
		if (dorkestProfiler::instance == nullptr) {
			dorkestProfiler::instance = new dorkestProfiler();
		}
		return dorkestProfiler::instance;
	}

    /// <summary>
    /// Collates the data of the datapoint with the name tagName. 
    /// Will return a text readout of various statistical paramters.
    /// </summary>
    /// <param name="tagName">The name of the data points to process</param>
    /// <returns>a std::string containing a kinda-sorta formatted statistical data</returns>
    std::string viewDataPoint(std::string tagName) {
        if (tagMap.count(tagName) == 0) {
            error("No data points for tag name " + tagName);
			return "ERROR";
		}
		else {
			LARGE_INTEGER medianT, minT, maxT, avgT, count;
			minT.QuadPart = MAXLONGLONG;

			maxT.QuadPart = 0;
			avgT.QuadPart = 0;
			count.QuadPart = 0;
			std::vector<long long> median;

			for (LARGE_INTEGER li : tagMap.at(tagName)) {
				if (minT.QuadPart > li.QuadPart) { minT = li; }
				if (maxT.QuadPart < li.QuadPart) maxT = li;
				median.push_back(li.QuadPart);
				avgT.QuadPart += li.QuadPart;
				count.QuadPart++;
			}

			std::sort(median.begin(), median.end());
			medianT.QuadPart = median.at(int(median.size() / 2));

			avgT.QuadPart /= count.QuadPart;
			std::stringstream ss;
			ss << "Details for tag : " << tagName << " : " << count.QuadPart << " Datapoints recorded." << std::endl;
			ss << "Smallest Datapoint : " << minT.QuadPart << " us" << std::endl;
			ss << "Mean Average Datapoint : " << avgT.QuadPart << " us" << std::endl;
			ss << "Median Datapoint : " << medianT.QuadPart << " us" << std::endl;
			ss << "Largest Datapoint : " << maxT.QuadPart << " us" << std::endl;
			return ss.str();
		}
	}

protected:
	//Called by the Datapoint's destructor. Records the data for the datapoint. FOR INTERNAL USE ONLY!
	void addDataPoint(LARGE_INTEGER startingTime, LARGE_INTEGER endingTime, std::string tagname) {
		LARGE_INTEGER ElapsedMicroseconds;
		ElapsedMicroseconds.QuadPart = endingTime.QuadPart - startingTime.QuadPart;
		ElapsedMicroseconds.QuadPart *= 1000000;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

		if (tagMap.count(tagname) == 0) {

			tagMap.emplace(std::pair<std::string, std::vector<LARGE_INTEGER>>(tagname, std::vector<LARGE_INTEGER>()));

		}
		tagMap.at(tagname).push_back(ElapsedMicroseconds);
	}


	friend class dorkestDataPoint;

private:
	std::map<std::string, std::vector<LARGE_INTEGER>> tagMap;
	static dorkestProfiler* instance;
	LARGE_INTEGER Frequency;

	dorkestProfiler() {
		QueryPerformanceFrequency(&Frequency);
	}
};


/// <summary>
/// dorkestDataPoint is basically a scope exploit. /n Just construct it before the area of code you want to profile,
/// and let it fall out of scope when you're done. Then call the profiler's viewDataPoint member with the name
/// of your datapoint set! These tags persist, so every time the constructor is called, it's a new point of data in the set.
/// 
/// BEWARE : Not thread safe. Not optimized. THIS IS THE ONLY PART OF THIS CODE THAT IS REASONABLY ACCURATE. NOT VERY. 
/// USE AT OWN RISK. IN CASE OF UNANTICPATED SWELLING, LOSS OF VISION, SEIZURE, OR CESSATION OF LIFE, you have DEFINATELY used
/// this class wrong.
/// </summary>
class dorkestDataPoint
{
public:
	/// <summary>
	/// Constructor for a datapoint. Simply call this in a scope you want to profile (surround my if(true) block if needed)
	/// and when it falls out of scope, it will report back to the profiler singleton!
	/// </summary>
	/// <param name="nameTag">The identifier of this set of datapoints</param>
	dorkestDataPoint(std::string nameTag) {
		tag = nameTag;
		bool ret = QueryPerformanceCounter(&timeStart);
	};

	~dorkestDataPoint()
	{
		bool ret = QueryPerformanceCounter(&timeEnd);
		dorkestProfiler::getInstance()->addDataPoint(timeStart, timeEnd, tag);
	}



private:
	LARGE_INTEGER timeStart, timeEnd;
	std::string tag;

};