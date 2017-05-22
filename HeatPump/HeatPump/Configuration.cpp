#include "Configuration.h"
#include "Definitions.h"



Configuration::Configuration()
{
}


Configuration::~Configuration()
{
}

void Configuration::loop1() {
	//Debug("Loop1");
	DevMgr.loop1(counter1);
	ScenMgr.loop1(counter1);
}

void Configuration::loop5() {
	//Debug("Loop5");
#ifdef WEB_ENABLED
	web.loop();

#endif // WEB_ENABLED

	DevMgr.loop5(counter5);
	ScenMgr.loop5(counter5);
}

void Configuration::loop10() {
	//Debug("Loop10");
	DevMgr.loop10(counter10);
	ScenMgr.loop10(counter10);
}

void Configuration::begin() {
	DevMgr.begin();
	//ScenMgr.begin();

#ifdef WEB_ENABLED
	web.begin();
#endif
}