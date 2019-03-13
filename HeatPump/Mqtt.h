// mqtt.h

#pragma once 

#include <PubSubClient.h>
//#include "definitions.h"
//#include "Ñonfiguration.h"
#include "Loger.h"
#include <EthernetClient.h>
#include "Sensor.h"
#include "Relay.h"



#define MQTT_INITIAL_RETRY_DELAY	1000 //delay between INITIAL reconnection retry
#define MQTT_RETRY_TIME				10000 //delay between mqtt reconnect in loop
//#define TOPIC_LENGTH	100
//#define PAYLOAD_LENGTH	100
#define MQTT_TRY_COUNT 5
#define MQTT_WAITING_RESPONSE 10000
#define MQTT_RESUBSCRIBE_TRY_COUNT 1
#define MQTT_RESUBSCRIPTION_DELAY	50
//#define MQTT_ETHERNET_TIME (5*1000)


//#define BOARD_ID "Board_%02d"
#define MQTT_SEPARATOR "/"

//#define CHECK_MQTT	"info"

//#define MQTT_WATCH_DOG "Config" MQTT_SEPARATOR BOARD_ID MQTT_SEPARATOR "WatchDog"

#define MQTT_LOG_PREFIX "Logs"
#define MQTT_EQUIPMENT_PREFIX "Equipment"
#define MQTT_THERMOMETERS_PREFIX MQTT_EQUIPMENT_PREFIX MQTT_SEPARATOR "Thermometers" MQTT_SEPARATOR
#define MQTT_CONTACTORS_PREFIX MQTT_EQUIPMENT_PREFIX MQTT_SEPARATOR "Contactors" MQTT_SEPARATOR
#define MQTT_PUMPS_PREFIX MQTT_EQUIPMENT_PREFIX MQTT_SEPARATOR "Pumps" MQTT_SEPARATOR
#define MQTT_COMPRESSOR_PREFIX MQTT_EQUIPMENT_PREFIX MQTT_SEPARATOR "Compressor" MQTT_SEPARATOR
#define MQTT_DESIRED_TEMP "Desired_Temperature"
#define MQTT_CONTROL_TEMP "Controllable_Temperature"

void callbackFunc(char* topic, uint8_t* payload, unsigned int length);

class Mqtt : public PubSubClient
{
	public:
		//EthernetClient EthClient;


		unsigned long mqttWaiting;
		bool setup();
		void MqttLoop();
		void PublishLog(DebugLevel level, String message);
		void Subscribe(String topic);
		bool Publish(String topic, String payload);
		bool Publish(Sensor* dev);
		bool PublishDesiredTemp(float temp);
		bool PublishRelay(Relay* dev);

		Mqtt();
		void Callback(char* topic, uint8_t* payload, unsigned int length);
//		void WatchDog();
		void InitialActions();


private:
		//char *boardId = (char*)BOARD_ID;
		String LOG_END[7] = { "OFF", "FATAL","ERROR","WARN","INFO","DEBUG","ALL" };
		
		bool mqttReconnect();
		void subscribeTopics();
		String rootPath();
};

