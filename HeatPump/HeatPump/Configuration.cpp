#include "Configuration.h"



Configuration::Configuration()
{
}


Configuration::~Configuration()
{
}

void Configuration::loop1() {
	DevManager.loop1(counter1);
}

void Configuration::loop5() {
	web.loop();
	DevManager.loop5(counter5);
}

void Configuration::loop10() {
	DevManager.loop10(counter10);

}

void Configuration::begin() {
	DevManager.begin();
	web.begin();
}