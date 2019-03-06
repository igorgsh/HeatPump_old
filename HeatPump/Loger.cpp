#include "Loger.h"
#include "Configuration.h"
#include "mqtt.h"
#include "definitions.h"

extern Mqtt MqttClient;
extern Configuration Config;

DebugLevel dLevel = DebugLevel::DEBUG;


void Loger::Log(DebugLevel level, String message) {
	if (level <= dLevel) {
		if (Config.IsMqttAvailable()) {
			//Config.MqttClient()->PublishLog(level, message);
		}
		Serial.println(message);
	}
}