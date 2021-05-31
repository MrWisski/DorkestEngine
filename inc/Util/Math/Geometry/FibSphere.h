#pragma once
#include <Util/Math/Vector3.h>
#include <Util/Math/Geometry/Ray.h>

/// <summary>
/// Generates a Fibonaci sphere 
/// </summary>
class FibSphere {
public:
    std::vector<Vector3f> points;
    

    FibSphere(int numPoints) {
        //directions = new Vector3[BoidHelper.numViewDirections];

        float goldenRatio = 1.618033988749895f;
        float angleIncrement = M_PI * 2 * goldenRatio;

        for (int i = 0; i < numPoints; i++) {
            float t = (float)i / numPoints;
            float inclination = std::acosf(1 - 2 * t);
            float azimuth = angleIncrement * i;

            float x = std::sinf(inclination) * std::cosf(azimuth);
            float y = std::sinf(inclination) * std::sinf(azimuth);
            float z = std::cosf(inclination);
            points.push_back( Vector3f(x, y, z));
        }
    }


};