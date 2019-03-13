/*
Name:		Mqtt
Created:	11.01.17 22:04:42
Author:	Igor Shevchenko
*/

#include "Mqtt.h"
#include "Loger.h"
#include "Configuration.h"
#include "Sensor.h"

//extern Mqtt MqttClient;
extern Configuration Config;
EthernetClient EthClient;

void callbackFunc(char* topic, uint8_t* payload, unsigned int length) {
	Loger::SimpleLog("callbackFunc[" + String(topic) + "](" + String(length) +")"  );
	Config.GetMqttClient()->Callback(topic, payload, length);
}

Mqtt::Mqtt() : PubSubClient(Config.GetMqttCreds().ServerIP, Config.GetMqttCreds().Port, callbackFunc, EthClient) {
	mqttWaiting = MQTT_INITIAL_RETRY_DELAY;
	
}


bool Mqtt::mqttReconnect() {

	//	char topic[TOPIC_LENGTH];
	bool res = false;
	unsigned connectTry = 0;

	if (!connected()) {
		while (!res && connectTry <= MQTT_TRY_COUNT) {
			Loger::Debug("Mqtt connect attempt=" + String(connectTry));

			if (Config.GetMqttCreds().Login.length() == 0) {
				res = connect(Config.BoardName().c_str());
				Loger::Info("Mqtt connect (guest): " + String(res));
			}
			else {
				res = connect(Config.BoardName().c_str(), Config.GetMqttCreds().Login.c_str(), Config.GetMqttCreds().Password.c_str());
				Loger::Info("Mqtt connect (" + Config.GetMqttCreds().Login + "): " + String(res));
			}

			delay(MQTT_INITIAL_RETRY_DELAY);
			connectTry++;
		}
		if (!res) {
			Loger::Debug("MQTT: Too many reconnect errors");
		}
	}

	return res;
}

void Mqtt::Callback(char* topic, uint8_t* payLoad, unsigned int length) {

	if (length > 0) {
		String strTopic = String(topic);
		String strPayload = String((char*)payLoad).substring(0, length);
		//		char subscription[TOPIC_LENGTH];
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
//	long connectTry = 0;
	bool res = false;

	res = mqttReconnect();
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

void Mqtt::PublishLog(DebugLevel level, String message) {
	String topic = rootPath() + MQTT_LOG_PREFIX + MQTT_SEPARATOR + LOG_END[level];
	Publish(topic, message);
}

bool Mqtt::PublishDesiredTemp(float temp) {

	String topic;
	topic = rootPath() + MQTT_DESIRED_TEMP;
	String payLoad = String(temp);

	return Publish(topic, payLoad);
}

bool Mqtt::PublishRelay(Relay* dev) {
	String topic = rootPath();
	

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


/*
void Mqtt::WatchDog() {

	//uint8_t rnd = random(0, 1000);
	char topic[TOPIC_LENGTH];
	sprintf(topic, MQTT_WATCH_DOG, Config.BoardId);
	Publish(topic, String(Config.counter60).c_str());
}
*/

bool Mqtt::Publish(String topic, String payload) {
	bool res = false;
	if (Config.IsMqtt()) {
		if (mqttReconnect()) {
			Loger::Debug("Publish:[" + topic + "]" + payload);
			res =publish(topic.c_str(), payload.c_str());
		}
	}
	return res;
}

bool Mqtt::Publish(Sensor* dev) {
	String topic = rootPath();

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

void Mqtt::Subscribe(String topic) {
	if (Config.IsMqtt()) {
		Loger::Debug("Subscribe [" + topic + "]");
		if (mqttReconnect()) {
			subscribe(topic.c_str());
		}
	}
}

void Mqtt::subscribeTopics() {
	String topic0;


	// Subscribe for Special Temperatures
	topic0 = rootPath() + MQTT_DESIRED_TEMP;
	Subscribe(topic0);

}

String Mqtt::rootPath() { 
	return Config.GetMqttCreds().Root + MQTT_SEPARATOR + Config.BoardName() + MQTT_SEPARATOR; 
}
