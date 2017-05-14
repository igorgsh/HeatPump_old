#include "Configuration.h"



Configuration::Configuration()
{
}


Configuration::~Configuration()
{
}

void Configuration::loop1() {

}

void Configuration::loop5() {
	web.loop();
}

void Configuration::loop10() {

}

void Configuration::begin() {
	web.begin();
}