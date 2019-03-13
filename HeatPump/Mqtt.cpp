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

	if (!connected()) {
		Loger::Debug("MqttReconnect");
		if (Config.GetMqttCreds().Login.length() == 0) {
			res = connect(Config.BoardName().c_str());
			Loger::Info("Mqtt connect (guest): " + String(res));
		}
		else {
			res = connect(Config.BoardName().c_str(), Config.GetMqttCreds().Login.c_str(), Config.GetMqttCreds().Password.c_str());
			Loger::Info("Mqtt connect (" + Config.GetMqttCreds().Login + "): " + String(res));
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



void Mqtt::InitMqtt(void) {
	Loger::Debug("Init MQTT");
	long connectTry = 0;
	bool res = false;
	
	while (!res && connectTry <= MQTT_TRY_COUNT) {
		Loger::Debug("Mqtt connect attempt=" + String(connectTry));
		res = mqttReconnect();
		delay(MQTT_INITIAL_RETRY_DELAY);
		connectTry++;
	}
	if (!res) {
		Loger::Debug("Too many attempt of MQTT reconnect");
	}
	else {
		subscribeTopics();
	}
}

void Mqtt::MqttLoop() {

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

void Mqtt::PublishLog(DebugLevel level, String message) {
	if (connected()) {
		String topic = rootPath() + MQTT_LOG_PREFIX + MQTT_SEPARATOR + LOG_END[level];
		//Loger::SimpleLog("LogTopic:" + topic);
		publish(topic.c_str(), message.c_str());
	}
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
	if (connected()) {
		Loger::Debug("Publish:[" + topic + "]" + payload);
		return publish(topic.c_str(), payload.c_str());
	}
	else {
		return false;
	}
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


void Mqtt::Subscribe(String topic) {
	Loger::Debug("Subscribe [" + topic + "]");
	if (connected()) {
		subscribe(topic.c_str());
	}
}

void Mqtt::subscribeTopics() {
	String topic0;


	// Subscribe for Special Temperatures
	topic0 = rootPath() + MQTT_DESIRED_TEMP;
	Subscribe(topic0);

	PublishDesiredTemp(Config.GetDesiredTemp());

//	topic0 = rootPath() + MQTT_CONTROL_TEMP;
//	Subscribe(topic0);
/*
	// Subscribe for compressor

	topic0 = rootPath() + MQTT_COMPRESSOR_PREFIX;
	Subscribe(topic0);

// Subscribe for Thermometers
	topic0 = rootPath() + MQTT_THERMOMETERS_PREFIX;
	for (int i = 0; i < Config.DevMgr.getNumberTemp(); i++) {
		String topic = topic0 + Config.DevMgr.tempSensors[i].getLabel();
//		Loger::Debug("TOPIC:" + topic);
		Subscribe(topic);
	}

	// Subscribe for Contactors
	topic0 = rootPath() + MQTT_CONTACTORS_PREFIX;
	for (int i = 0; i < Config.DevMgr.getNumberCont(); i++) {
		String topic = topic0 + Config.DevMgr.contacts[i].getLabel();
		Subscribe(topic);
	}

	// Subscribe for Pumps
	topic0 = rootPath() + MQTT_PUMPS_PREFIX;
	for (int i = 0; i < Config.DevMgr.getNumberPump(); i++) {
		String topic = topic0 + Config.DevMgr.pumps[i].getLabel();
		Subscribe(topic);
	}
*/

}

String Mqtt::rootPath() { 
	return Config.GetMqttCreds().Root + MQTT_SEPARATOR + Config.BoardName() + MQTT_SEPARATOR; 
}
