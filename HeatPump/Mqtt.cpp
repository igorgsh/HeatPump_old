// 
// 
// 
/*
Name:		Mqtt
Created:	11.01.17 22:04:42
Author:	Igor Shevchenko
*/

#include "Mqtt.h"
#include "Loger.h"
#include "definitions.h"
#include "Configuration.h"

extern Configuration Config;

//extern Mqtt MqttClient;

EthernetClient ethClientMqtt;

void callbackFunc(char* topic, uint8_t* payload, unsigned int length) {
	Config.MqttClient()->Callback(topic, payload, length);
}

Mqtt::Mqtt() : PubSubClient(Config.GetMqttCredentials().ServerIP, Config.GetMqttCredentials().Port, callbackFunc, ethClientMqtt ) {
	mqttWaiting = MQTT_INITIAL_RETRY_DELAY;

	Loger::Debug("IP=" + Config.PrintIP(Config.GetMqttCredentials().ServerIP));
	Loger::Debug("Port=" + String(Config.GetMqttCredentials().Port));
	//this->setServer(Config.GetMqttCredentials().ServerURL.c_str(), Config.GetMqttCredentials().Port);
	//this->setCallback(callbackFunc);
}


bool Mqtt::MqttReconnect() {

	//char topic[TOPIC_LENGTH];
	bool res = false;

	if (Config.IsEthernetConnection()) {
		if (!connected()) {
			Loger::Debug("MqttReconnect");
			if (connect(Config.BoardName().c_str())) {
				res = true;
			}
			else {
				Loger::Error("Mqtt Reconnection Failed!");
				res = false;
			}
		}
	}
	return res;
}

void Mqtt::Callback(char* topic, uint8_t* payLoad, unsigned int length) {
/*
	if (length > 0) {
		String strTopic = String(topic);
		String strPayload = String((char*)payLoad).substring(0, length);
		char subscription[TOPIC_LENGTH];

		Loger::Debug("[" + strTopic + "]:" + strPayload + "#");
		sprintf(subscription, MQTT_CONFIG_RESPONSE, Config.BoardId);
		if (strcmp(topic, subscription) == 0) {
			if (Config.isConfigRequested) {
				Config.UpdateConfig(strPayload);
			}
		}
		else {
			sprintf(subscription, MQTT_ACTIONS_RESPONSE, Config.BoardId);

			if (strcmp(topic, subscription) == 0) {
				if (Config.isActionRequested) {
					Config.UpdateActions(strPayload);
				}
			}
			else {
				if (strTopic.startsWith(MQTT_BUTTONS)) {
					Loger::Debug("Update Button");

					Config.UpdateButton(strTopic.substring(strlen(MQTT_BUTTONS) + 2), strPayload);
				}
				else {
					if (strTopic.startsWith(MQTT_RELAYS)) {
						//Loger::Debug("Update Relay");

						Config.UpdateRelay(strTopic.substring(strlen(MQTT_RELAYS) + 2), strPayload);
					}
					else {
						if (strTopic.startsWith(MQTT_1WIREBUS)) {
							//Loger::Debug("Update 1-wire bus");

							Config.UpdateOneWireBus(strTopic.substring(strlen(MQTT_1WIREBUS) + 2), strPayload);
						}
						else {
							if (strTopic.startsWith(MQTT_1WIRETHERMO)) {
								//Loger::Debug("Update 1-wire thermo");

								Config.UpdateOneWireThermo(strTopic.substring(strlen(MQTT_1WIRETHERMO) + 2), strPayload);
							}
							else {
								if (strTopic.startsWith(MQTT_POWERMETER)) {
									//Loger::Debug("Update Powermeter");
									Config.UpdatePowerMeter(strTopic.substring(strlen(MQTT_POWERMETER) + 2), strPayload);
								}
								else {
									if (strTopic.startsWith(MQTT_CONTACTOR)) {
										//Loger::Debug("Update Contactor");
										Config.UpdateContactor(strTopic.substring(strlen(MQTT_CONTACTOR) + 2), strPayload);
									}
									else {
										if (strTopic.startsWith(MQTT_RESET_BOARD)) {
											sprintf(subscription, MQTT_RESET_BOARD, Config.BoardId);
											Loger::Debug("Reset");

											if (strTopic.startsWith((String)subscription) && strPayload != NULL && strPayload[0] >= '0') {
												Board::Reset(10000);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	*/
}



void Mqtt::begin(void) {
	Loger::Debug("Init MQTT");
	long connectTry = 0;
	bool res = false;

	while (!res && connectTry <= MQTT_TRY_COUNT) {
		Loger::Debug("Mqtt connect attempt=" + String(connectTry));
		res = MqttReconnect();
		delay(MQTT_INITIAL_RETRY_DELAY);
		connectTry++;
	}

	if (!res) {
		Loger::Debug("Too many attempt of MQTT reconnect");
	}
	else {
		SubscribeDevices();
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
			MqttReconnect();
		}
	}

}

void Mqtt::PublishLog(DebugLevel level, String message) {
	if (connected()) {
		char topic[TOPIC_LENGTH];
		sprintf(topic, MQTT_LOG, Config.GetId(), LOG_END[level]);
		publish(topic, message.c_str());
	}
}

bool Mqtt::Publish(Device* dev, String value) {
	char topic[TOPIC_LENGTH];
	Loger::Debug("Point 10");
	MqttDeviceTopicName(topic, dev);
	Loger::Debug("Topic:" + String(topic) + "; Value=" + value);
	return Publish(topic, value.c_str());
}



bool Mqtt::Publish(const char* topic, const char* payload) {
	if (connected()) {
		Loger::Debug("Publish:[" + String(topic) + "]" + String(payload));
		return publish(topic, payload);
	}
	else {
		return false;
	}
}


void Mqtt::Subscribe(const char* topic) {
	Loger::Debug("Subscribe [" + String(topic) + "]");
	if (connected()) {
		subscribe(topic);
	}
}

void Mqtt::SubscribeDevices() {
	char topic[TOPIC_LENGTH];
/*
	for (int i = 0; i < NUMBER_OF_CONTACTOR; i++) {
		MqttDeviceTopicName(topic, &(Config.DevMgr.contacts[i]));
		Subscribe(topic);
	}

	for (int i = 0; i < NUMBER_OF_TEMP; i++) {
		MqttDeviceTopicName(topic, &(Config.DevMgr .tempSensors[i]));
		Subscribe(topic);
	}
	
	for (int i = 0; i < NUMBER_OF_PUMP; i++) {
		MqttDeviceTopicName(topic, &(Config.DevMgr.pumps[i]));
		Subscribe(topic);
	}
*/
	MqttSpecialTopic(topic, 0);
	Subscribe(topic);
	MqttSpecialTopic(topic, 1);
	Subscribe(topic);

}

char* Mqtt::MqttDeviceTopicName(char* topic, Device* dev) {

	//char topic0[TOPIC_LENGTH];

	//topic[0] = 0;
//	sprintf(topic, "%s%s%s%s", Config.GetMqttCredentials().Root.c_str(), MQTT_SEPARATOR, Config.BoardName().c_str(), MQTT_SEPARATOR);
	//strcat(topic, Config.GetMqttCredentials().Root.c_str());
	//strcat(topic)
	//strcat(topic, MQTT_SEPARATOR);

	MQTT_HEADER;

	switch (dev->getType()) {
	case DeviceType::CONTACT:
	{
		strcat(topic, MQTT_CONTACTOR);
		break;
	}
	case DeviceType::THERMOMETER: {
		strcat(topic, MQTT_THERMO);
		break;
	}
	case DeviceType::COMPRESSOR: {
		strcat(topic, MQTT_COMPRESSOR);
		break; 
	}
	case DeviceType::PUMP: {
		strcat(topic, MQTT_PUMPS);
		break;
	}
	default: {
		strcat(topic, "Unknown");
		break;
	}
	}
	strcat(topic, dev->getLabel().c_str());
	return topic;
}

char* Mqtt::MqttSpecialTopic(char* topic, int spec) {

	//char topic0[TOPIC_LENGTH];
//	sprintf(topic, "%s%s%s%s", Config.GetMqttCredentials().Root.c_str(), MQTT_SEPARATOR, Config.BoardName().c_str(), MQTT_SEPARATOR);
	MQTT_HEADER;

//	topic[0] = 0;
//	strcat(topic, Config.GetMqttCredentials().Root.c_str());
//	strcat(topic, MQTT_SEPARATOR);

	switch (spec) {
	case 0: {//Desired temp
		strcat(topic, MQTT_DESIRED_TEMP);
		break;
	} case 1: {//Controllable temp
		strcat(topic, MQTT_CONTROL_TEMP);
		break;
	}
	}
	return topic;
}