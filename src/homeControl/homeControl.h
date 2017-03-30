/*
 * homeControl.h
 *
 *  Created on: 24 mars 2017
 *      Author: Bertrand
 */

#ifndef HOMECONTROL_HOMECONTROL_H_
#define HOMECONTROL_HOMECONTROL_H_

#define HOMECONTROL_IP "192.168.1.26:8080"

#define HC_HEATER_CUISINE		0
#define HC_HEATER_SALON 		1
#define HC_HEATER_BARNABE 		2
#define HC_HEATER_DAPHNEE		3
#define HC_HEATER_HOMECINEMA	4
#define HC_LAST_HEATER			5
#define HC_TEMP_EXTERIEUR		5
#define HC_TEMP_GARAGE			6
#define HC_TEMP_VICTOR			7
#define HC_TEMP_PARENT			8
#define HC_LAST					9

struct stHomeControl
{
	char* niceName;
	char* httpName;
	float temp;
	float targetTemp;
};

float hcGetTemp(int ii);
float hcGetTargetTemp(int ii);
char* hcGetName(int ii);
float hcGetCourant();
int homeControlLaunch(void);

class homeControl {
public:
	homeControl();
	virtual ~homeControl();

	stHomeControl _HomeSensor[HC_LAST];
	float _current;

	void refreshData(void);
	float readTempfromRPI(char* name);
	float readTargetTempfromRPI(char* name);
	float readCurrentfromRPI();
};

#endif /* HOMECONTROL_HOMECONTROL_H_ */
