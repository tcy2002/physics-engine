#pragma once

#include "phys/phys_general.h"

namespace pe_phys_shape {

#   define PE_BOX_DEFAULT_MESH {\
            {\
                    {{pe::Real(-0.5), pe::Real(pe::Real(0.5)), pe::Real(-0.5)}, {0, 1, 0}},\
                    {{pe::Real(-0.5), pe::Real(0.5), pe::Real(0.5)}, {0, 1,  0}},\
                    {{pe::Real(0.5), pe::Real(0.5), pe::Real(0.5)}, {0, 1, 0}},\
                    {{pe::Real(0.5), pe::Real(0.5), pe::Real(-0.5)}, {0, 1, 0}},\
                    {{pe::Real(-0.5), pe::Real(-0.5), pe::Real(0.5)}, {0,  -1, 0}},\
                    {{pe::Real(-0.5), pe::Real(-0.5), pe::Real(-0.5)}, {0, -1, 0}},\
                    {{pe::Real(0.5), pe::Real(-0.5), pe::Real(-0.5)}, {0, -1, 0}},\
                    {{pe::Real(0.5), pe::Real(-0.5), pe::Real(0.5)}, {0, -1, 0}},\
                    {{pe::Real(-0.5), pe::Real(0.5), pe::Real(0.5)}, {0, 0, 1}},\
                    {{pe::Real(-0.5), pe::Real(-0.5), pe::Real(0.5)}, {0, 0, 1}},\
                    {{pe::Real(0.5), pe::Real(-0.5), pe::Real(0.5)}, {0, 0, 1}},\
                    {{pe::Real(0.5), pe::Real(0.5), pe::Real(0.5)}, {0, 0, 1}},\
                    {{pe::Real(0.5), pe::Real(0.5), pe::Real(-0.5)}, {0, 0, -1}},\
                    {{pe::Real(0.5), pe::Real(-0.5), pe::Real(-0.5)}, {0, 0, -1}},\
                    {{pe::Real(-0.5), pe::Real(-0.5), pe::Real(-0.5)}, {0, 0, -1}},\
                    {{pe::Real(-0.5), pe::Real(0.5), pe::Real(-0.5)}, {0, 0, -1}},\
                    {{pe::Real(-0.5), pe::Real(0.5), pe::Real(-0.5)}, {-1, 0, 0}},\
                    {{pe::Real(-0.5), pe::Real(-0.5), pe::Real(-0.5)}, {-1, 0, 0}},\
                    {{pe::Real(-0.5), pe::Real(-0.5), pe::Real(0.5)}, {-1, 0, 0}},\
                    {{pe::Real(-0.5), pe::Real(0.5), pe::Real(0.5)}, {-1, 0, 0}},\
                    {{pe::Real(0.5), pe::Real(0.5), pe::Real(0.5)}, {1, 0, 0}},\
                    {{pe::Real(0.5), pe::Real(-0.5), pe::Real(0.5)}, {1, 0, 0}},\
                    {{pe::Real(0.5), pe::Real(-0.5), pe::Real(-0.5)}, {1, 0, 0}},\
                    {{pe::Real(0.5), pe::Real(0.5), pe::Real(-0.5)}, {1, 0, 0}}\
            },\
            {\
                    {{0, 1, 2, 3}, {0, 1, 0}},\
                    {{4, 5, 6, 7}, {0, -1, 0}},\
                    {{8, 9, 10, 11}, {0, 0, 1}},\
                    {{12, 13, 14, 15}, {0, 0, -1}},\
                    {{16, 17, 18, 19}, {-1, 0, 0}},\
                    {{20, 21, 22, 23}, {1, 0, 0}}\
            }\
    }

    const pe::Array<pe::KV<pe::Vector3, pe::Vector3>> _box_unique_edges = { //NOLINT
            {pe::Vector3(-0.5, 0.5, 0.5), pe::Vector3(0.5, 0.5, 0.5)},
            {pe::Vector3(-0.5, -0.5, 0.5), pe::Vector3(0.5, -0.5, 0.5)},
            {pe::Vector3(-0.5, -0.5, -0.5), pe::Vector3(0.5, -0.5, 0.5)},
            {pe::Vector3(-0.5, 0.5, -0.5), pe::Vector3(0.5, 0.5, -0.5)},
            {pe::Vector3(0.5, -0.5, 0.5), pe::Vector3(0.5, 0.5, 0.5)},
            {pe::Vector3(-0.5, -0.5, 0.5), pe::Vector3(-0.5, 0.5, 0.5)},
            {pe::Vector3(-0.5, -0.5, -0.5), pe::Vector3(-0.5, 0.5, -0.5)},
            {pe::Vector3(0.5, -0.5, -0.5), pe::Vector3(0.5, 0.5, -0.5)},
            {pe::Vector3(0.5, 0.5, -0.5), pe::Vector3(0.5, 0.5, -0.5)},
            {pe::Vector3(-0.5, 0.5, -0.5), pe::Vector3(-0.5, 0.5, -0.5)},
            {pe::Vector3(-0.5, -0.5, -0.5), pe::Vector3(-0.5, -0.5, -0.5)},
            {pe::Vector3(0.5, -0.5, -0.5), pe::Vector3(0.5, -0.5, -0.5)}
    };

#   define PE_CYLINDER_DEFAULT_MESH {\
            {\
                    {{pe::Real(0.5), pe::Real(0.5), 0}, {0, 1, 0}},\
                    {{pe::Real(0.475528), pe::Real(0.5), pe::Real(-0.154508)}, {0, 1, 0}},\
                    {{pe::Real(0.404508), pe::Real(0.5), pe::Real(-0.293893)}, {0, 1, 0}},\
                    {{pe::Real(0.293893), pe::Real(0.5), pe::Real(-0.404508)}, {0, 1, 0}},\
                    {{pe::Real(0.154508), pe::Real(0.5), pe::Real(-0.475528)}, {0, 1, 0}},\
                    {{0, pe::Real(0.5), pe::Real(-0.5)}, {0, 1, 0}},\
                    {{pe::Real(-0.154508), pe::Real(0.5), pe::Real(-0.475528)}, {0, 1, 0}},\
                    {{pe::Real(-0.293893), pe::Real(0.5), pe::Real(-0.404508)}, {0, 1, 0}},\
                    {{pe::Real(-0.404508), pe::Real(0.5), pe::Real(-0.293893)}, {0, 1, 0}},\
                    {{pe::Real(-0.475528), pe::Real(0.5), pe::Real(-0.154508)}, {0, 1, 0}},\
                    {{pe::Real(-0.5), pe::Real(0.5), 0}, {0, 1, 0}},\
                    {{pe::Real(-0.475528), pe::Real(0.5), pe::Real(0.154508)}, {0, 1, 0}},\
                    {{pe::Real(-0.404508), pe::Real(0.5), pe::Real(0.293893)}, {0, 1, 0}},\
                    {{pe::Real(-0.293893), pe::Real(0.5), pe::Real(0.404508)}, {0, 1, 0}},\
                    {{pe::Real(-0.154508), pe::Real(0.5), pe::Real(0.475528)}, {0, 1, 0}},\
                    {{0, pe::Real(0.5), pe::Real(0.5)}, {0, 1, 0}},\
                    {{pe::Real(0.154508), pe::Real(0.5), pe::Real(0.475528)}, {0, 1, 0}},\
                    {{pe::Real(0.293893), pe::Real(0.5), pe::Real(0.404508)}, {0, 1, 0}},\
                    {{pe::Real(0.404508), pe::Real(0.5), pe::Real(0.293893)}, {0, 1, 0}},\
                    {{pe::Real(0.475528), pe::Real(0.5), pe::Real(0.154508)}, {0, 1, 0}},\
                    {{pe::Real(0.493844), pe::Real(-0.5), pe::Real(-0.078217)}, {0, -1, 0}},\
                    {{pe::Real(0.445503), pe::Real(-0.5), pe::Real(-0.226995)}, {0, -1, 0}},\
                    {{pe::Real(0.353553), pe::Real(-0.5), pe::Real(-0.353553)}, {0, -1, 0}},\
                    {{pe::Real(0.226995), pe::Real(-0.5), pe::Real(-0.445503)}, {0, -1, 0}},\
                    {{pe::Real(0.078217), pe::Real(-0.5), pe::Real(-0.493844)}, {0, -1, 0}},\
                    {{pe::Real(-0.078217), pe::Real(-0.5), pe::Real(-0.493844)}, {0, -1, 0}},\
                    {{pe::Real(-0.226995), pe::Real(-0.5), pe::Real(-0.445503)}, {0, -1, 0}},\
                    {{pe::Real(-0.353553), pe::Real(-0.5), pe::Real(-0.353553)}, {0, -1, 0}},\
                    {{pe::Real(-0.445503), pe::Real(-0.5), pe::Real(-0.226995)}, {0, -1, 0}},\
                    {{pe::Real(-0.493844), pe::Real(-0.5), pe::Real(-0.078217)}, {0, -1, 0}},\
                    {{pe::Real(-0.493844), pe::Real(-0.5), pe::Real(0.078217)}, {0, -1, 0}},\
                    {{pe::Real(-0.445503), pe::Real(-0.5), pe::Real(0.226995)}, {0, -1, 0}},\
                    {{pe::Real(-0.353553), pe::Real(-0.5), pe::Real(0.353553)}, {0, -1, 0}},\
                    {{pe::Real(-0.226995), pe::Real(-0.5), pe::Real(0.445503)}, {0, -1, 0}},\
                    {{pe::Real(-0.078217), pe::Real(-0.5), pe::Real(0.493844)}, {0, -1, 0}},\
                    {{pe::Real(0.078217), pe::Real(-0.5), pe::Real(0.493844)}, {0, -1, 0}},\
                    {{pe::Real(0.226995), pe::Real(-0.5), pe::Real(0.445503)}, {0, -1, 0}},\
                    {{pe::Real(0.353553), pe::Real(-0.5), pe::Real(0.353553)}, {0, -1, 0}},\
                    {{pe::Real(0.445503), pe::Real(-0.5), pe::Real(0.226995)}, {0, -1, 0}},\
                    {{pe::Real(0.493844), pe::Real(-0.5), pe::Real(0.078217)}, {0, -1, 0}},\
                    {{pe::Real(0.5), pe::Real(0.5), 0}, {1, 0, 0}},\
                    {{pe::Real(0.475528), pe::Real(0.5), pe::Real(-0.154508)}, {pe::Real(0.951056), 0, pe::Real(-0.309017)}},\
                    {{pe::Real(0.404508), pe::Real(0.5), pe::Real(-0.293893)}, {pe::Real(0.809017), 0, pe::Real(-0.587785)}},\
                    {{pe::Real(0.293893), pe::Real(0.5), pe::Real(-0.404508)}, {pe::Real(0.587785), 0, pe::Real(-0.809017)}},\
                    {{pe::Real(0.154508), pe::Real(0.5), pe::Real(-0.475528)}, {pe::Real(0.309017), 0, pe::Real(-0.951056)}},\
                    {{0, pe::Real(0.5), pe::Real(-0.5)}, {0, 0, -1}},\
                    {{pe::Real(-0.154508), pe::Real(0.5), pe::Real(-0.475528)}, {pe::Real(-0.309017), 0, pe::Real(-0.951056)}},\
                    {{pe::Real(-0.293893), pe::Real(0.5), pe::Real(-0.404508)}, {pe::Real(-0.587785), 0, pe::Real(-0.809017)}},\
                    {{pe::Real(-0.404508), pe::Real(0.5), pe::Real(-0.293893)}, {pe::Real(-0.809017), 0, pe::Real(-0.587785)}},\
                    {{pe::Real(-0.475528), pe::Real(0.5), pe::Real(-0.154508)}, {pe::Real(-0.951056), 0, pe::Real(-0.309017)}},\
                    {{pe::Real(-0.5), pe::Real(0.5), 0}, {-1, 0, 0}},\
                    {{pe::Real(-0.475528), pe::Real(0.5), pe::Real(0.154508)}, {pe::Real(-0.951056), 0, pe::Real(0.309017)}},\
                    {{pe::Real(-0.404508), pe::Real(0.5), pe::Real(0.293893)}, {pe::Real(-0.809017), 0, pe::Real(0.587785)}},\
                    {{pe::Real(-0.293893), pe::Real(0.5), pe::Real(0.404508)}, {pe::Real(-0.587785), 0, pe::Real(0.809017)}},\
                    {{pe::Real(-0.154508), pe::Real(0.5), pe::Real(0.475528)}, {pe::Real(-0.309017), 0, pe::Real(0.951056)}},\
                    {{0, pe::Real(0.5), pe::Real(0.5)}, {0, 0, 1}},\
                    {{pe::Real(0.154508), pe::Real(0.5), pe::Real(0.475528)}, {pe::Real(0.309017), 0, pe::Real(0.951056)}},\
                    {{pe::Real(0.293893), pe::Real(0.5), pe::Real(0.404508)}, {pe::Real(0.587785), 0, pe::Real(0.809017)}},\
                    {{pe::Real(0.404508), pe::Real(0.5), pe::Real(0.293893)}, {pe::Real(0.809017), 0, pe::Real(0.587785)}},\
                    {{pe::Real(0.475528), pe::Real(0.5), pe::Real(0.154508)}, {pe::Real(0.951056), 0, pe::Real(0.309017)}},\
                    {{pe::Real(0.493844), pe::Real(-0.5), pe::Real(-0.078217)}, {pe::Real(0.987688), 0, pe::Real(-0.156434)}},\
                    {{pe::Real(0.445503), pe::Real(-0.5), pe::Real(-0.226995)}, {pe::Real(0.891006), 0, pe::Real(-0.453990)}},\
                    {{pe::Real(0.353553), pe::Real(-0.5), pe::Real(-0.353553)}, {pe::Real(0.707107), 0, pe::Real(-0.707107)}},\
                    {{pe::Real(0.226995), pe::Real(-0.5), pe::Real(-0.445503)}, {pe::Real(0.453990), 0, pe::Real(-0.891006)}},\
                    {{pe::Real(0.078217), pe::Real(-0.5), pe::Real(-0.493844)}, {pe::Real(0.156434), 0, pe::Real(-0.987688)}},\
                    {{pe::Real(-0.078217), pe::Real(-0.5), pe::Real(-0.493844)}, {pe::Real(-0.156434), 0, pe::Real(-0.987688)}},\
                    {{pe::Real(-0.226995), pe::Real(-0.5), pe::Real(-0.445503)}, {pe::Real(-0.453990), 0, pe::Real(-0.891006)}},\
                    {{pe::Real(-0.353553), pe::Real(-0.5), pe::Real(-0.353553)}, {pe::Real(-0.707107), 0, pe::Real(-0.707107)}},\
                    {{pe::Real(-0.445503), pe::Real(-0.5), pe::Real(-0.226995)}, {pe::Real(-0.891006), 0, pe::Real(-0.453990)}},\
                    {{pe::Real(-0.493844), pe::Real(-0.5), pe::Real(-0.078217)}, {pe::Real(-0.987688), 0, pe::Real(-0.156434)}},\
                    {{pe::Real(-0.493844), pe::Real(-0.5), pe::Real(0.078217)}, {pe::Real(-0.987688), 0, pe::Real(0.156434)}},\
                    {{pe::Real(-0.445503), pe::Real(-0.5), pe::Real(0.226995)}, {pe::Real(-0.891006), 0, pe::Real(0.453990)}},\
                    {{pe::Real(-0.353553), pe::Real(-0.5), pe::Real(0.353553)}, {pe::Real(-0.707107), 0, pe::Real(0.707107)}},\
                    {{pe::Real(-0.226995), pe::Real(-0.5), pe::Real(0.445503)}, {pe::Real(-0.453990), 0, pe::Real(0.891006)}},\
                    {{pe::Real(-0.078217), pe::Real(-0.5), pe::Real(0.493844)}, {pe::Real(-0.156434), 0, pe::Real(0.987688)}},\
                    {{pe::Real(0.078217), pe::Real(-0.5), pe::Real(0.493844)}, {pe::Real(0.156434), 0, pe::Real(0.987688)}},\
                    {{pe::Real(0.226995), pe::Real(-0.5), pe::Real(0.445503)}, {pe::Real(0.453990), 0, pe::Real(0.891006)}},\
                    {{pe::Real(0.353553), pe::Real(-0.5), pe::Real(0.353553)}, {pe::Real(0.707107), 0, pe::Real(0.707107)}},\
                    {{pe::Real(0.445503), pe::Real(-0.5), pe::Real(0.226995)}, {pe::Real(0.891006), 0, pe::Real(0.453990)}},\
                    {{pe::Real(0.493844), pe::Real(-0.5), pe::Real(0.078217)}, {pe::Real(0.987688), 0, pe::Real(0.156434)}},\
            },\
            {\
                    {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,\
                      11, 12, 13, 14, 15, 16, 17, 18, 19},\
                     {0, 1, 0}},\
                    {{39, 38, 37, 36, 35, 34, 33, 32, 31, 30,\
                      29, 28, 27, 26, 25, 24, 23, 22, 21, 20},\
                     {0, -1, 0}},\
                    {{60, 41, 40}, {pe::Real(0.987670), pe::Real(0.006156), pe::Real(-0.156432)}},\
                    {{61, 42, 41}, {pe::Real(0.890990), pe::Real(0.006156), pe::Real(-0.453982)}},\
                    {{62, 43, 42}, {pe::Real(0.707093), pe::Real(0.006156), pe::Real(-0.707093)}},\
                    {{63, 44, 43}, {pe::Real(0.453982), pe::Real(0.006156), pe::Real(-0.890990)}},\
                    {{64, 45, 44}, {pe::Real(0.156432), pe::Real(0.006156), pe::Real(-0.987670)}},\
                    {{65, 46, 45}, {pe::Real(-0.156432), pe::Real(0.006156), pe::Real(-0.987670)}},\
                    {{66, 47, 46}, {pe::Real(-0.493982), pe::Real(0.006156), pe::Real(-0.890990)}},\
                    {{67, 48, 47}, {pe::Real(-0.707093), pe::Real(0.006156), pe::Real(-0.707093)}},\
                    {{68, 49, 48}, {pe::Real(-0.890990), pe::Real(0.006156), pe::Real(-0.453982)}},\
                    {{69, 50, 49}, {pe::Real(-0.987670), pe::Real(0.006156), pe::Real(-0.156432)}},\
                    {{70, 51, 50}, {pe::Real(-0.987670), pe::Real(0.006156), pe::Real(0.156432)}},\
                    {{71, 52, 51}, {pe::Real(-0.890990), pe::Real(0.006156), pe::Real(0.453982)}},\
                    {{72, 53, 52}, {pe::Real(-0.707093), pe::Real(0.006156), pe::Real(0.707093)}},\
                    {{73, 54, 53}, {pe::Real(-0.453982), pe::Real(0.006156), pe::Real(0.890990)}},\
                    {{74, 55, 54}, {pe::Real(-0.156432), pe::Real(0.006156), pe::Real(0.987670)}},\
                    {{75, 56, 55}, {pe::Real(0.156432), pe::Real(0.006156), pe::Real(0.987670)}},\
                    {{76, 57, 56}, {pe::Real(0.453982), pe::Real(0.006156), pe::Real(0.890990)}},\
                    {{77, 58, 57}, {pe::Real(0.707093), pe::Real(0.006156), pe::Real(0.707093)}},\
                    {{78, 59, 58}, {pe::Real(0.890990), pe::Real(0.006156), pe::Real(0.453982)}},\
                    {{79, 40, 59}, {pe::Real(0.987670), pe::Real(0.006156), pe::Real(0.156432)}},\
                    {{40, 79, 60}, {pe::Real(0.999981), pe::Real(-0.006156), 0}},\
                    {{41, 60, 61}, {pe::Real(0.951038), pe::Real(-0.006156), pe::Real(-0.309011)}},\
                    {{42, 61, 62}, {pe::Real(0.809002), pe::Real(-0.006156), pe::Real(-0.587774)}},\
                    {{43, 62, 63}, {pe::Real(0.587774), pe::Real(-0.006156), pe::Real(-0.809002)}},\
                    {{44, 63, 64}, {pe::Real(0.309011), pe::Real(-0.006156), pe::Real(-0.951038)}},\
                    {{45, 64, 65}, {0, pe::Real(-0.006156), pe::Real(-0.999981)}},\
                    {{46, 65, 66}, {pe::Real(-0.309011), pe::Real(-0.006156), pe::Real(-0.951038)}},\
                    {{47, 66, 67}, {pe::Real(-0.587774), pe::Real(-0.006156), pe::Real(-0.809002)}},\
                    {{48, 67, 68}, {pe::Real(-0.809002), pe::Real(-0.006156), pe::Real(-0.587774)}},\
                    {{49, 68, 69}, {pe::Real(-0.951038), pe::Real(-0.006156), pe::Real(-0.309011)}},\
                    {{50, 69, 70}, {pe::Real(-0.999981), pe::Real(-0.006156), 0}},\
                    {{51, 70, 71}, {pe::Real(-0.951038), pe::Real(-0.006156), pe::Real(0.309011)}},\
                    {{52, 71, 72}, {pe::Real(-0.809002), pe::Real(-0.006156), pe::Real(0.587774)}},\
                    {{53, 72, 73}, {pe::Real(-0.587774), pe::Real(-0.006156), pe::Real(0.809002)}},\
                    {{54, 73, 74}, {pe::Real(-0.309011), pe::Real(-0.006156), pe::Real(0.951038)}},\
                    {{55, 74, 75}, {0, pe::Real(-0.006156), pe::Real(0.999981)}},\
                    {{56, 75, 76}, {pe::Real(0.309011), pe::Real(-0.006156), pe::Real(0.951038)}},\
                    {{57, 76, 77}, {pe::Real(0.587774), pe::Real(-0.006156), pe::Real(0.809002)}},\
                    {{58, 77, 78}, {pe::Real(0.809002), pe::Real(-0.006156), pe::Real(0.587774)}},\
                    {{59, 78, 79}, {pe::Real(0.951038), pe::Real(-0.006156), pe::Real(0.309011)}},\
            }\
    }

    const pe::Array<pe::KV<pe::Vector3, pe::Vector3>> _cylinder_unique_edges = { //NOLINT
            
    };

} // namespace pe_phys_fracture