#pragma once
#include "Log.h"
#include <windows.h>
#include <profileapi.h>
#include <errhandlingapi.h>


class dorkestProfiler {
public:
    static dorkestProfiler* getInstance() {
        if (dorkestProfiler::instance == nullptr) {
            dorkestProfiler::instance = new dorkestProfiler();
        }
        return dorkestProfiler::instance;
    }

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
            ss << "Median Datapoint : " <<  medianT.QuadPart << " us" << std::endl;
            ss << "Largest Datapoint : " << maxT.QuadPart << " us" << std::endl;
            return ss.str();
        }
    }

private:
    std::map<std::string, std::vector<LARGE_INTEGER>> tagMap;
    static dorkestProfiler* instance;
    LARGE_INTEGER Frequency;

    dorkestProfiler() {
        QueryPerformanceFrequency(&Frequency);

    }
};

class dorkestProfile
{
public:
    dorkestProfile(std::string nameTag) {
        tag = nameTag;
        bool ret = QueryPerformanceCounter(&timeStart);
    };

    ~dorkestProfile()
    {
        bool ret = QueryPerformanceCounter(&timeEnd);
        dorkestProfiler::getInstance()->addDataPoint(timeStart, timeEnd, tag);
    }



private:
    LARGE_INTEGER timeStart,timeEnd;
    std::string tag;

};