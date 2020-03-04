//
// Created by timothy on 3/3/20.
//

#ifndef NERF_ACCELHANDLER_H
#define NERF_ACCELHANDLER_H

#include <MPU6050.h>
#include "Data.h"
#include "Wire.h"


#define RADIAL_ADDR 0x69
#define AXIAL_ADDR 0x68

#define RPX 1686
#define RNX -2430
#define RPY 2030
#define RNY -2050
#define RPZ 2020
#define RNZ -2180
#define APX 2220
#define ANX -1875
#define APY 1996
#define ANY -2080
#define APZ 1835
#define ANZ -2360

#define RADIUS 0.025

class AccelHandler {
public:
    static void init();
    static void deinit();
    static void mapAccel(Data* data);
    static void tick(Data* data);
};


#endif //NERF_ACCELHANDLER_H
