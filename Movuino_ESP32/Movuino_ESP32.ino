#include "_MOVUINO_ESP32/_MPU9250.h"
#include "_MOVUINO_ESP32/_Recorder.h"
#include "_MOVUINO_ESP32/_WifiOSC.h"
#include "_MOVUINO_ESP32/_Button.h"

// example - imu sensor
MovuinoMPU9250 mpu = MovuinoMPU9250();

// example - wifi communication
char ssid[] = "my-wifi";
char pass[] = "password";
int port = 5555;
int ip[4] = {192, 168, 1, 18};
MovuinoWifiOSC osc = MovuinoWifiOSC(ssid, pass, ip, port);

// example - record data in a local file
MovuinoRecorder recorder = MovuinoRecorder();
long timerRecord0;

// example - Movuino button
MovuinoButton button = MovuinoButton();

void setup()
{
  Serial.begin(115200);
  
  // example - imu sensor
  mpu.begin();

  // example - create record file
  recorder.begin();
  recorder.newRecord("imu_data"); // create file and start record
  recorder.defineColumns("ax,ay,az,gx,gy,gz,mx,my,mz"); // define data columns
  timerRecord0 = millis();

  // example - wifi communication
  osc.begin();
  osc.send<String>("/movuino", "start recording");

  // example - Movuino button
  button.begin();
}

void loop()
{
  // example - write Movuino sensor data into a file
  if (recorder.isRecording())
  {
    if (millis() - timerRecord0 > 10)
    {
      // update imu sensor data
      mpu.update();
      
      // write data
      recorder.addRow();
      recorder.pushData<float>(mpu.ax);
      recorder.pushData<float>(mpu.ay);
      recorder.pushData<float>(mpu.az);
      recorder.pushData<float>(mpu.gx);
      recorder.pushData<float>(mpu.gy);
      recorder.pushData<float>(mpu.gz);
      recorder.pushData<float>(mpu.mx);
      recorder.pushData<float>(mpu.my);
      recorder.pushData<float>(mpu.mz);

      timerRecord0 = millis();
    }
  }

  // example stop recording by pressing the button
  if (button.isReleased())
  {
    osc.send<String>("/movuino", "stop recording and write theim on serial port");
    recorder.stop();
    delay(100);
    recorder.readAllRecords(); // write recorded data on serial port
  }
}