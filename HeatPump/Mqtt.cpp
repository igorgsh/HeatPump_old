/*
Name:		Mqtt
Created:	11.01.17 22:04:42
Author:	Igor Shevchenko
*/

#include "Mqtt.h"
#include "Loger.h"
#include "Configuration.h"
#include "Sensor.h"

extern Configuration Config;
EthernetClient EthClient;

void callbackFunc(char* topic, uint8_t* payload, unsigned int length) {
	Config.GetMqttClient()->Callback(topic, payload, length);
}

Mqtt::Mqtt() : PubSubClient(Config.GetMqttCreds().ServerIP, Config.GetMqttCreds().Port, callbackFunc, EthClient) {
	mqttWaiting = MQTT_INITIAL_RETRY_DELAY;
	rootPath = Config.GetMqttCreds().Root + MQTT_SEPARATOR + Config.BoardName() + MQTT_SEPARATOR;
}


bool Mqtt::mqttReconnect() {

	bool res = connected();
	unsigned connectTry = 0;
		
	if (!res) {
		while (!res && connectTry <= MQTT_TRY_COUNT) {
			if (Config.GetMqttCreds().Login.length() == 0) {
				res = connect(Config.BoardName().c_str());
			}
			else {
				res = connect(Config.BoardName().c_str(), Config.GetMqttCreds().Login.c_str(), Config.GetMqttCreds().Password.c_str());
			}
			if (!res) {
				delay(MQTT_INITIAL_RETRY_DELAY);
				connectTry++;
			}
		}
	}

	return res;
}

void Mqtt::Callback(char* topic, uint8_t* payLoad, unsigned int length) {

	if (length > 0) {
		String strTopic = String(topic);
		String strPayload = String((char*)payLoad).substring(0, length);
		String str0;

		Loger::Debug("CallBack:[" + strTopic + "]:" + strPayload + "#");

		str0 = MQTT_DESIRED_TEMP;
		if (strTopic.endsWith(str0)) { //Desired Temperature
			Loger::Debug("Set Desired temp:" + strPayload);
			Config.SetDesiredTemp(strPayload.toFloat());
		}
		else /*if (strTopic.endsWith(MQTT_CONTROL_TEMP))*/ {
			//Config.GetControlTemperature()
		}
	}
}



bool Mqtt::setup(void) {
	Loger::Debug("Init MQTT");
	bool res = false;

	res = mqttReconnect();
	String topic0 = rootPath + MQTT_LOG_PREFIX + MQTT_SEPARATOR;
	for (unsigned i = 0; i < 7; i++) {
		LogTopic[i] = topic0 + LOG_END[i];
	}
	return res;
}

void Mqtt::MqttLoop() {

	if (Config.IsMqtt()) {
		static long lastConnected = 0;

		if (connected()) {
			bool res = loop();
			if (!res) {
				Loger::Error("Failed loop");
			}
			lastConnected = millis();
		}
		else {
			if (lastConnected + millis() <= MQTT_RETRY_TIME) {
				Loger::Debug("Trying to reconnect MQTT");
				mqttReconnect();
			}
		}
	}
}

void Mqtt::PublishLog(DebugLevel level, const String &message) {
	if (Config.IsMqtt()) {
		publish(LogTopic[level].c_str(), message.c_str());
	}
}

bool Mqtt::PublishDesiredTemp(float temp) {

	String topic;
	topic = rootPath + MQTT_DESIRED_TEMP;
	String payLoad = String(temp);

	return Publish(topic, payLoad);
}

bool Mqtt::PublishRelay(Relay* dev) {
	String topic = rootPath;
	

	switch (dev->GetType()) {
	case UnitType::UT_Pump_C1:
	case UnitType::UT_Pump_C2:
	case UnitType::UT_Pump_Geo:
	case UnitType::UT_Pump_F1:
	case UnitType::UT_Pump_F2: {
		topic += MQTT_PUMPS_PREFIX;
		break;
	}
	case UnitType::UT_Compressor: {
		topic += MQTT_COMPRESSOR_PREFIX;
		break;
	}
	default:
		return false;
	}
	topic += dev->GetLabel();
	String payLoad = String(dev->status);
	return Publish(topic, payLoad);
}


bool Mqtt::Publish(const String &topic, const String &payload) {
	bool res = false;
	
	if (Config.IsMqtt()) {
		if (topic.length() != 0 && payload.length() != 0) {
			res = publish(topic.c_str(), payload.c_str());
			Loger::SimpleLog("Publish:[" + topic + "]" + payload);
		}
	}
	return res;
}

bool Mqtt::Publish(Sensor* dev) {
	String topic = rootPath;

	switch (dev->GetType()) {
	case UnitType::UT_Thermometer: {
		topic += MQTT_THERMOMETERS_PREFIX;
		break;
	}
	case UnitType::UT_Contactor: {
		topic += MQTT_CONTACTORS_PREFIX;
		break;
	}
	default:
		break;
	}
	topic += dev->GetLabel();
	String payLoad = String(dev->GetValue());
	return Publish(topic, payLoad);
}

void Mqtt::InitialActions() {
	subscribeTopics();
	PublishDesiredTemp(Config.GetDesiredTemp());
}

void Mqtt::Subscribe(const String &topic) {
	if (Config.IsMqtt()) {
		Loger::Debug("Subscribe [" + topic + "]");
		if (Config.IsMqtt()) {
			subscribe(topic.c_str());
		}
	}
}

void Mqtt::subscribeTopics() {
	String topic0;
	// Subscribe for Special Temperatures
	topic0 = rootPath + MQTT_DESIRED_TEMP;
	Subscribe(topic0);
}

