//
// Created by timothy on 3/3/20.
//

#include "AccelHandler.h"

MPU6050 radial(RADIAL_ADDR);
MPU6050 axial(AXIAL_ADDR);


void AccelHandler::init() {
    Serial.println("1");
    Wire.begin();
    Serial.println("2");
    Serial.println("3");

    axial.initialize();
    Serial.println("4");

    radial.initialize();
    Serial.println("5");

    axial.setFullScaleGyroRange(3);
    axial.setFullScaleAccelRange(3);
    radial.setFullScaleGyroRange(3);
    radial.setFullScaleAccelRange(3);
    Serial.println("6");


}

void AccelHandler::mapAccel(Data* m_data) {
    m_data->radial_accel_mapped.y = (double) map((long) m_data->radial_accel.y, RNY, RPY, -10000, 10000) / 10000. * 9.81;
    m_data->radial_accel_mapped.x = (double) map((long) m_data->radial_accel.x, RNX, RPX, -10000, 10000) / 10000. * 9.81;
    m_data->radial_accel_mapped.z = (double) map((long) m_data->radial_accel.z, RNZ, RPZ, -10000, 10000) / 10000. * 9.81;
    m_data->axial_accel_mapped.y =  (double) map((long) m_data->axial_accel.y, ANY, APY, -10000, 10000) / 10000. * 9.81;
    m_data->axial_accel_mapped.x  =  (double) map((long) m_data->axial_accel.x, ANX, APX, -10000, 10000) / 10000. * 9.81;
    m_data->axial_accel_mapped.z  =  (double) map((long) m_data->axial_accel.z, ANZ, APZ, -10000, 10000) / 10000. * 9.81;
}

void AccelHandler::tick(Data* m_data) {
    axial.getMotion6(&m_data->axial_accel.x,
                     &m_data->axial_accel.y,
                     &m_data->axial_accel.z,
                     &m_data->axial_gyro.x,
                     &m_data->axial_gyro.y,
                     &m_data->axial_gyro.z);
    radial.getMotion6(&m_data->radial_accel.x,
                      &m_data->radial_accel.y,
                      &m_data->radial_accel.z,
                      &m_data->radial_gyro.x,
                      &m_data->radial_gyro.y,
                      &m_data->radial_gyro.z);

    mapAccel(m_data);
    double totalRadAccel = m_data->radial_accel_mapped.magnitude();
    double totalAxialAccel = m_data->axial_accel_mapped.magnitude();
    double netRadAccel = totalRadAccel - 9.81;
    double netAxialAccel = totalAxialAccel - 9.81;
    m_data->netLinearAcc = netAxialAccel;
    double radialAcceleration = netRadAccel - netAxialAccel;
    if (radialAcceleration < 0) radialAcceleration = -radialAcceleration;
    m_data->angularVelocity = sqrt(radialAcceleration/RADIUS);

}

void AccelHandler::deinit() {
}
