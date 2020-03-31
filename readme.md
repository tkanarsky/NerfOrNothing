# Nerf or Nothing

Firmware for a smart Nerf ball prototype. Features:

* Calculates spin rate and distance thrown (wip) from acceleration data
* Logs acceleration data to SPIFFS as .csv files
* Hosts a webpage at 192.168.1.1 that displays current acceleration readings
* Hosts an ad-hoc wifi network for access to aforementioned page
* Hosts an FTP server to pull log data from file storage

To compile, initialize a PlatformIO project using the platformio.ini file in 
the repo, do `platformio uploadfs` to upload html and js data to the filesystem, 
then do `platformio upload` with a NodeMCU connected to the computer
to flash the ESP8266 chip and begin logging data.

Hardware:

* A Nerf Vortex football -- cheap and stable. We cut it in half and carved
  notches for the electronics.
* The setup runs on a NodeMCU with an ESP8266 processor. We chose this microcontroller because of its low profile (no through-hole components or headers) and built-in WiFi modem.
    * The NodeMCU has a 3.3 volt logic level, unlike the Arduino Uno’s 5 volts. Fortunately, all our components ran fine on a reduced voltage, though the beeper was quieter.
* Two MPU6050 digital accelerometers attached to the I2C bus on the NodeMCU.
    * The accelerometers have a default address of 0x68, which would’ve caused an address conflict, so we shorted the AD0 pin to 3.3V on one of the accelerometers to switch its address to 0x69.
    * These accelerometers are configured, by default, to a range of +/- 2 g (~20m/s^2) of acceleration per axis, but are capable of measuring +/- 16 g (~160 m/s^2) of acceleration. As our project would deal with abrupt changes in velocity, we switched the accelerometers to the latter range.
* A piezo beeper attached to a digital output
* A button attached to a digital output to trigger logging start/stop.
* A 9-volt battery powers the MCU through a switching regulator to convert the ~9v from the battery to a steady 3.3v.
