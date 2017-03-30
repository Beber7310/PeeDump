/*
 * homeControl.cpp
 *
 *  Created on: 24 mars 2017
 *      Author: Bertrand
 */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "http_fetcher.h"

#include "homeControl.h"


homeControl hc;

static pthread_mutex_t hcLock;

//  http://192.168.1.26:8080/hc_thermostat?Salon/status
//  http://192.168.1.26:8080/hc_tmp?Cuisine/value
//	http://192.168.1.26:8080/hc_amp

homeControl::homeControl() {
	// TODO Auto-generated constructor stub
	_HomeSensor[HC_HEATER_CUISINE].httpName="Cuisine";
	_HomeSensor[HC_HEATER_CUISINE].niceName="Cuisine";
	_HomeSensor[HC_HEATER_CUISINE].temp=0.0f;

	_HomeSensor[HC_HEATER_SALON].httpName="Salon";
	_HomeSensor[HC_HEATER_SALON].niceName="Salon";
	_HomeSensor[HC_HEATER_SALON].temp=0.0f;

	_HomeSensor[HC_HEATER_BARNABE].httpName="Barnabe";
	_HomeSensor[HC_HEATER_BARNABE].niceName="Barnabe";
	_HomeSensor[HC_HEATER_BARNABE].temp=0.0f;

	_HomeSensor[HC_HEATER_DAPHNEE].httpName="Daphnee";
	_HomeSensor[HC_HEATER_DAPHNEE].niceName="Daphnee";
	_HomeSensor[HC_HEATER_DAPHNEE].temp=0.0f;

	_HomeSensor[HC_HEATER_HOMECINEMA].httpName="Homecinema";
	_HomeSensor[HC_HEATER_HOMECINEMA].niceName="HomeCinema";
	_HomeSensor[HC_HEATER_HOMECINEMA].temp=0.0f;

	_HomeSensor[HC_TEMP_EXTERIEUR].httpName="Exterior";
	_HomeSensor[HC_TEMP_EXTERIEUR].niceName="Exterieur";
	_HomeSensor[HC_TEMP_EXTERIEUR].temp=0.0f;

	_HomeSensor[HC_TEMP_GARAGE].httpName="Garage";
	_HomeSensor[HC_TEMP_GARAGE].niceName="Garage";
	_HomeSensor[HC_TEMP_GARAGE].temp=0.0f;

	_HomeSensor[HC_TEMP_VICTOR].httpName="Victor";
	_HomeSensor[HC_TEMP_VICTOR].niceName="Victor";
	_HomeSensor[HC_TEMP_VICTOR].temp=0.0f;

	_HomeSensor[HC_TEMP_PARENT].httpName="Parent";
	_HomeSensor[HC_TEMP_PARENT].niceName="Parent";
	_HomeSensor[HC_TEMP_PARENT].temp=0.0f;
}

homeControl::~homeControl() {
	// TODO Auto-generated destructor stub
}

void homeControl::refreshData(void)
{
	for(int ii=0;ii<HC_LAST;ii++)
	{
		_HomeSensor[ii].temp=readTempfromRPI(_HomeSensor[ii].httpName);
	}
	for(int ii=0;ii<HC_LAST_HEATER;ii++)
	{
		_HomeSensor[ii].targetTemp=readTargetTempfromRPI(_HomeSensor[ii].httpName);
	}
	_current=readCurrentfromRPI();
}

float hcGetTemp(int ii)
{
	if(ii<HC_LAST)
		return  hc._HomeSensor[ii].temp;
	else
		return 0;
}
float hcGetTargetTemp(int ii)
{
	if(ii<HC_LAST)
		return  hc._HomeSensor[ii].targetTemp;
	else
		return 0;
}
char* hcGetName(int ii)
{
	if(ii<HC_LAST)
		return   hc._HomeSensor[ii].niceName;
	else
		return 0;
}

float hcGetCourant()
{
	return hc._current;
}



 float  homeControl::readTempfromRPI(char* name)
{
	char* szResultat;
	char* pch;
	char szCmd[512];
	float temp=0.0;

	sprintf(szCmd,"http://%s/hc_tmp?%s/value",HOMECONTROL_IP,name);
	//printf(szCmd);
	http_fetch(szCmd,&szResultat);



	pch = strtok(szResultat,":");
	if(!pch){printf("Error in readTempfromRPI");return -100;}
	pch = strtok(NULL,":");
	if(!pch){printf("Error in readTempfromRPI");return -100;}
	temp = atof(pch);

	if(szResultat)
		free(szResultat);

	return temp;

}

float  homeControl::readTargetTempfromRPI(char* name)
{
	char* szResultat;
	char* pch;
	char szCmd[512];
	float temp=0.0;

	sprintf(szCmd,"http://%s/hc_thermostat?%s/status",HOMECONTROL_IP,name);
	//printf(szCmd);
	http_fetch(szCmd,&szResultat);



	pch = strtok(szResultat,":");
	if(!pch){printf("Error in readTargetTempfromRPI");return -100;}
	pch = strtok(NULL,":");
	if(!pch){printf("Error in readTargetTempfromRPI");return -100;}
	pch = strtok(NULL,":");
	if(!pch){printf("Error in readTargetTempfromRPI");return -100;}
	temp = atof(pch);

	if(szResultat)
		free(szResultat);

	return temp;

}

float  homeControl::readCurrentfromRPI()
{
	char* szResultat;
	char* pch;
	char szCmd[512];
	float temp=0.0;

	sprintf(szCmd,"http://%s/hc_amp",HOMECONTROL_IP);
	//printf(szCmd);
	http_fetch(szCmd,&szResultat);



	pch = strtok(szResultat,":");
	if(!pch){printf("Error in readTargetTempfromRPI");return -100;}
	pch = strtok(NULL,":");

	temp = atof(pch);

	if(szResultat)
		free(szResultat);

	return temp;
}


 void* hcThread(void * p)
 {


	 while(1)
	 {
		 hc.refreshData();
		 sleep(1);
	 }


 }

 int homeControlLaunch(void)
 {
 	pthread_t my_hcThread;
 	int ret;


 	if (pthread_mutex_init(&hcLock, NULL) != 0)
 	{
 		printf("\n hcLock  mutex init failed\n");
 		return 1;
 	}


 	ret =  pthread_create(&my_hcThread, NULL, &hcThread,NULL);
 	if(ret != 0) {
 		printf("Error: pthread_create() failed\n");
 		exit(EXIT_FAILURE);
 	}

 	return 0;
 }
