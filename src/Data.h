//
// Created by timothy on 3/3/20.
//

#ifndef NERF_DATA_H
#define NERF_DATA_H

template<typename V>
struct Triplet {
   V x, y, z;

   Triplet() {
       x = 0; y = 0; z = 0;
   }
   double magnitude() {
       return sqrt((double) (x*x + y*y + z*z));
   }
};

struct Data {
    Triplet<int16_t> axial_accel;
    Triplet<int16_t> axial_gyro;
    Triplet<int16_t> radial_accel;
    Triplet<int16_t> radial_gyro;

    Triplet<double> axial_accel_mapped;
    Triplet<double> radial_accel_mapped;

    double angularVelocity = 0;
    double netLinearAcc = 0;

    bool logging = false;
};

#endif //NERF_DATA_H
