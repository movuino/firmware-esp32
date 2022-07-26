#if !defined(_MOVUINOESP32_WIFIOSC_)
#define _MOVUINOESP32_WIFIOSC_

#include "WiFi.h"
#include <OSCMessage.h>

class MovuinoWifiOSC
{
private:
    WiFiUDP udp;
    IPAddress outIp; // OSCAR

    // Network settings
    char *ssid = "my_box_id";
    char *pass = "my_box_pass";
    unsigned int port = 5555;

    // void receiveMessage();
    // static void callbackOSC(OSCMessage &msg);

public:
    MovuinoWifiOSC();
    MovuinoWifiOSC(char *ssid_, char *pass_, int *ip_, unsigned int port_);
    ~MovuinoWifiOSC();

    void begin();
    void update();

    template <typename DataType>
    void send(const char * addr_, DataType data_);
};

MovuinoWifiOSC::MovuinoWifiOSC(char *ssid_, char *pass_, int *ip_, unsigned int port_)
{
    this->outIp = IPAddress(ip_[0], ip_[1], ip_[2], ip_[3]);
    this->ssid = ssid_;
    this->pass = pass_;
    this->port = port_;
}

MovuinoWifiOSC::~MovuinoWifiOSC()
{
}

void MovuinoWifiOSC::begin()
{
    Serial.println("Wait for WiFi... ");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    udp.begin(this->port);
}

void MovuinoWifiOSC::update()
{
}

template <typename DataType>
void MovuinoWifiOSC::send(const char * addr_, DataType data_)
{
    // Create message
    OSCMessage msg_(addr_);
    msg_.add(data_);

    // Send message
    udp.beginPacket(this->outIp, this->port);
    msg_.send(udp);
    udp.endPacket();
    msg_.empty();
}

// void MovuinoWifiOSC::receiveMessage() {
//   // Need to be called in the main loop()
//   OSCMessage msg_;
//   int size = udp.parsePacket();
 
//   if (size > 0) {
//     while (size--) {
//       msg_.fill(udp.read());
//     }
//     if (!msg_.hasError()) {
//      msg_.dispatch("/printInt", this->callbackOSC);
//     }
//   }
// }

// static void MovuinoWifiOSC::callbackOSC(OSCMessage &msg) {
//   Serial.print("Receive message: ");
//   Serial.println(msg.getInt(0));
// }

#endif // _MOVUINOESP32_WIFIOSC_