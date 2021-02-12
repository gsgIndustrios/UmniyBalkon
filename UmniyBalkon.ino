/*
 Name:		UmniyBalkon.ino
 Created:	19.06.2020 23:23:33
 Author:	KENNY
*/
//Имена модулей***/run/setup
/*
Server
-Bluetooth
-NRF
-Display
BalkonBtPlayer
Sensors
PWMDrive
RelayDrive
*/
//#########################################################################################################################################################################//
//***************************************************************************Универсальные*********************************************************************************//
//#########################################################################################################################################################################//


#include <Wire.h> 

unsigned long MS;

int	
BMTt1,  //Температура BMP
DHTW,   //Влажность DHT
BMPP,   //Давление BMP
BMPH,   //Высота BMP
LuxPog, //Погода солнце
WtPog, //Погода дождь
LuxLux, //Освещенность
Smoke,  //Дым
Fire,   //Огонь
DV1,    //PWM 1
DV2,    //PWM 2
DV3,    //PWM 3
DV4,    //PWM 4
DV5,    //PWM 5
SIn,    //Источник звука
SVol,   //Уровень звука
DD,     //День
MM,     //Месяц
YY,     //Год
hh,     //Час
mm;     //Минута
bool RSw1, RSw2, RSw3, RSw4, RSw5, RSw6, RSw7, RSw8; //Статус реле
String RdStation; //Название радиостанции
String Rain_Sun; //Погода


//#########################################################################################################################################################################//
//******************************************************************Модуль распределения и обработки***********************************************************************//
//#########################################################################################################################################################################//
/*
Адрес устройства с пульта (AddressModule=Addr-NRF)
1. Звуковой контроллер/Радио
2. Свет балкон
3. Свет комната
4. Диско-шарик
5. Диско-голова
6. Доп1
7. Доп2
*/

int AddressModule;

#define AMBalkonBtPlayer 1
#define AMLedPwmDrive 2
#define AMRoomLight 3
#define AMDiscoBall 4
#define AMDiscoHead 5
#define AMDop1 6
#define AMDop2 7
#define AMRelayDrive 8
#define AMPWMDrive 9
#define AMProgPage 10

//*****************************************************************Модуль распределения и обработки (Типа "сервер")*********************************************************//

bool TestingMode = true;

void ServerSetup() {
	if (TestingMode) { Serial.begin(115200); }
	BluetoothSetup();
	NRFSPISETUP();
	//Nextion
	NextionSetup();
	BalkonBtPlayerSetup();
	//Reserve(Wifi)
	//Serial3.begin(115200);
	SetupRelayDrive();
	SetupPWMDrive();
	SensorsSetup();
}

//Пришла команда с пульта
bool MSG;
int InputDevInt;                    //Имя устройства
int InputNameInt;                   //Имя изменяемой переменной
int InputVarInt;                    //Значение переменной

//Сортировка на беспроводные
void DriveNRF() {
			byte InputDevByte = byte(InputDevInt);
			byte InputNameByte = byte(InputNameInt);
			byte InputVarByte = byte(InputVarInt);
			NRFSPI(InputDevByte, InputNameByte, InputVarByte, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			if (TestingMode) { Serial.println(InputDevByte + ':' + InputDevByte + '=' + InputVarByte + ';'); }
			MSG == false;
			InputDevInt = 0;
}

#define AMBalkonBtPlayer 1
#define AMLedPwmDrive 2
#define AMRoomLight 3
#define AMDiscoBall 4
#define AMDiscoHead 5
#define AMDop1 6
#define AMDop2 7
#define AMRelayDrive 8
#define AMPWMDrive 9
#define AMProgPage 10



void Server() {
	MS = millis();
	SensorsDrive();
	BluetoothIn(); 
	NextionDrive();
	if (MSG) {
		switch (InputDevInt)
		{
		case AMBalkonBtPlayer:
			if (TestingMode) {
				Serial.println("BalkonBtPlayer");
				Serial.println(InputDevInt);
				Serial.println(InputNameInt);
				Serial.println(InputVarInt);
			}
			BalkonBtPlayer(InputNameInt, InputVarInt);
			break;
		case AMLedPwmDrive:
			break;
		case AMRoomLight:
			if (TestingMode) {
				Serial.println("RoomLight");
				Serial.println(InputDevInt);
				Serial.println(InputNameInt);
				Serial.println(InputVarInt);
			}
			DriveNRF();
			break;
		case AMDiscoBall:
			DriveNRF();
			if (TestingMode) {
				Serial.println("DiscoBall");
				Serial.println(InputDevInt);
				Serial.println(InputNameInt);
				Serial.println(InputVarInt);
			}
			break;
		case AMDiscoHead:
			DriveNRF();
			if (TestingMode) {
				Serial.println("DiscoHead");
				Serial.println(InputDevInt);
				Serial.println(InputNameInt);
				Serial.println(InputVarInt);
			}
			break;
		case AMDop1:
			break;
		case AMDop2:
			break;
		case AMRelayDrive:
			if (TestingMode) {
				Serial.println("Relay");
				Serial.println(InputDevInt);
				Serial.println(InputNameInt);
				Serial.println(InputVarInt);
			}
			RelayDrive(InputNameInt, InputVarInt);
			break;
		case AMPWMDrive:
			if (TestingMode) {
				Serial.println("PWM");
				Serial.println(InputDevInt);
				Serial.println(InputNameInt);
				Serial.println(InputVarInt);
			}
			PWMDrive(InputNameInt, InputVarInt);
			break;
		case AMProgPage:
			BluetoothOut("BMTt1",String(BMTt1));
			BluetoothOut("DHTW", String(DHTW));
			BluetoothOut("BMPP", String(BMPP));
			BluetoothOut("BMPH", String(BMPH));
			BluetoothOut("LuxPog", Rain_Sun);
			BluetoothOut("LuxLux", String(LuxLux));
			BluetoothOut("Smoke", String(Smoke));
			BluetoothOut("Fire", String(Fire));
			BluetoothOut("DV1", String(DV1));
			BluetoothOut("DV2", String(DV2));
			BluetoothOut("DV3", String(DV3));
			BluetoothOut("DV4", String(DV4));
			BluetoothOut("DV5", String(DV5));
			BluetoothOut("SIn", String(SIn));
			BluetoothOut("SVol", String(SVol));
			BluetoothOut("DD", String(DD));
			BluetoothOut("MM", String(MM));
			BluetoothOut("YY", String(YY));
			BluetoothOut("hh", String(hh));
			BluetoothOut("mm", String(mm));
			BluetoothOut("RSw1", BoolToString(RSw1));
			BluetoothOut("RSw2", BoolToString(RSw2));
			BluetoothOut("RSw3", BoolToString(RSw3));
			BluetoothOut("RSw4", BoolToString(RSw4));
			BluetoothOut("RSw5", BoolToString(RSw5));
			BluetoothOut("RSw6", BoolToString(RSw6));
			BluetoothOut("RSw7", BoolToString(RSw7));
			BluetoothOut("RSw8", BoolToString(RSw8));
			BluetoothOut("RdStation", RdStation);
			break;
		default:
			break;
		}
		MSG = false;
	}
	
}

//#########################################################################################################################################################################//
//**************************************************************************Управление Сенсоры********************************************************************************//
//#########################################################################################################################################################################//

/*------------------------------------------ЧАСЫ--------------------------------------------------------*/

//Библиотека часов
#include <iarduino_RTC.h>
iarduino_RTC Time(RTC_DS3231);

//интервал запроса времени
unsigned long PeriodClocks;
unsigned long IntervalClocks = 10000;

//установка времени
bool SetupTime = false;

//Запуск
void ClockSetup() {
	Time.begin();
}

//Установка
int SetYY, SetW, SetMM, SetDD, Sethh, Setmm, Setss;

void SetupTimeClock(int SetYY, int SetW, int SetMM, int SetDD, int Sethh, int Setmm, int Setss) {
	//день недели указывается в виде числа от 0-воскресенье до 6-суббота
	if (SetupTime) {
		Time.settime(Setss, Setmm, Sethh, SetDD, SetMM, SetYY, SetW);
		SetupTime = false;
	}
}

//Цикл чтения
void ReadClock() {
	if (PeriodClocks < MS) {
		Time.gettime();
		YY = Time.year;
		MM = Time.month;
		DD = Time.day;
		hh = Time.Hours;
		mm = Time.minutes;
		PeriodClocks = MS + IntervalClocks;
	}
}





/*-------------------------------------------Датчик солнца-------------------------------------------------*/
#define PinSunA A6
#define PinSunD 34
bool LuxPogD;
int LuxPogA;

void SunLuxSetup() {
	pinMode(PinSunA, INPUT);
	pinMode(PinSunD, INPUT);
}


void SunLuxDrive() {
	LuxPogD=digitalRead(PinSunD);
	LuxPogA = analogRead(PinSunA);
}


/*-------------------------------------------Датчик дождя-------------------------------------------------*/

#define PinHwlA A7
#define PinHwlD 35

//Переменные запроса
int RainA;
bool RainD;


void RainSetup() {
	pinMode(PinHwlA, INPUT);
	pinMode(PinHwlD, INPUT);
}

void RainDrive() {
	RainA = analogRead(PinHwlA);
	RainD = digitalRead(PinHwlD);
}

/*---------------------------------------------BMP------------------------------------------------------*/

//Библиотека 
//#include <Adafruit_BMP280.h>
#include <Adafruit_BMP085.h>
//Adafruit_BMP280 BMP;

Adafruit_BMP085 BMP;
//интервал запроса
unsigned long PeriodBMP;
unsigned long IntervalBMP = 10000;

//Переменные запроса
float temperature;
float pressure;
float altitude;
int BMPTemper;
int BMPPress;
int BMPAlt;

//Рабочие переменные
bool BMPON;

//Запуск
void BMPSetup() {
	if (!BMP.begin()) { BMPON = false; }
	else { BMPON = true; }
	BMP.begin();
}

//Цикл чтения
void BMPRead() {
	if (BMPON == true && PeriodBMP < MS) {
		temperature = BMP.readTemperature();
		pressure = BMP.readPressure() / 133.3;
		altitude = BMP.readAltitude(100838);
		PeriodBMP = MS + IntervalBMP;
		BMTt1 = int(temperature);
		BMPP = int(pressure);
		BMPH = int(altitude);
	}
}

/*-------------------------------------------Температура и влажность----------------------------------------------------*/

//Библиотека 

#include <DHT.h>

#define DHT1 38

#define DHTTYPE DHT11

DHT Dht1(DHT1, DHTTYPE);

//интервал запроса
unsigned long PeriodDHT;
unsigned long IntervalDHT = 10000;

//Переменные запроса
int dhtT1;     //Температура с датчиков
int dhtH1;	   //Влажность с датчиков

//Запуск
void TempHumDHTSetup() {
	pinMode(DHT1, INPUT);
	Dht1.begin();
}

//Цикл
void TempHumDHT() {
	if (PeriodDHT < MS) {
		DHTW = Dht1.readHumidity();
		PeriodDHT = MS + IntervalDHT;
	}
}


/*-------------------------------------------Погода------------------------------------------------*/


void WeatherSetup() {
	SunLuxSetup();
	RainSetup();
}

void WeatherDrive() {
	SunLuxDrive();
	RainDrive();
	Rain_Sun = "";
	if (LuxPogA > 500 && RainA<500) {
		Rain_Sun = "D/Sunny";
	}
	else if (300 > LuxPogA < 400 && RainA < 500) {
		Rain_Sun = "D/Cloudy";
	}
	else if (300 > LuxPogA < 400 && RainA > 500) {
		Rain_Sun = "D/Rainy";
	}
	else if (200 > LuxPogA < 300 && RainA < 500) {
		Rain_Sun = "N/Sunny";
	}
	else if (LuxPogA < 200 && RainA < 500) {
		Rain_Sun = "N/Cloudy";
	}
	else if ( LuxPogA < 200 && RainA > 500) {
		Rain_Sun = "N/Rainy";
	}
	else  {
		Rain_Sun = "";
	}
	//LuxPogD
	//LuxPogA
	//RainA
	//RainD
}

/*-------------------------------------------Датчик дыма-------------------------------------------------*/
#define PinSMA A8
#define PinSMD 22
bool SmokeD;


void SmokeSetup() {
	pinMode(PinSMA, INPUT);
	pinMode(PinSMD, INPUT);
}
void SmokeDrive() {
	SmokeD = digitalRead(PinSMD);
	Smoke = analogRead(PinSMA);
}

/*-------------------------------------------Датчик огня-------------------------------------------------*/
#define PinFA A9
#define PinFD 24
bool FireD;


void FireSetup() {
	pinMode(PinFA, INPUT);
	pinMode(PinFD, INPUT);
}

void FireDrive() {
	FireD = digitalRead(PinFD);
	Fire = analogRead(PinFA);
}

/*-------------------------------------------Датчик освещенности-------------------------------------------------*/
#define PinLuxA A10
#define PinLuxD 25
bool LuxLuxD;

void LuxSetup() {
	pinMode(PinFA, INPUT);
	pinMode(PinFD, INPUT);
}
void LuxDrive() {
	LuxLuxD = digitalRead(PinLuxD);
	LuxLux = analogRead(PinLuxA);
}


void SensorsSetup() {
	ClockSetup();
	BMPSetup();
	TempHumDHTSetup();
	SmokeSetup();
	FireSetup();
	LuxSetup();
	WeatherSetup();
	//MotionSetup();
}

void SensorsDrive() {
	ReadClock();
	BMPRead();
	TempHumDHT();
	SmokeDrive();
	FireDrive();
	LuxDrive();
	WeatherDrive();
	//MotionDrive();
}

//#########################################################################################################################################################################//
//**************************************************************************Управление Экран********************************************************************************//
//#########################################################################################################################################################################//



double IntervalNextion = 3000;
double PeriodNextion = 0;
//Функция посылки значения экрану
void NexSend(String Var, String Name) {
	String Msg = "P:";
	//Serial.print(Msg + Name + '.' + Var + ';');
	Serial2.print(Name + "." + "txt" + "=\"" + Var + "\"" + char(255) + char(255) + char(255)); delay(20);
}
//Конвертация Буль в Стринг
String BoolToString(bool Input) {
	String Output;
	if (Input == true) {
		Output = "ON";
		return Output;
	}
	else {
		Output = "OFF";
		return Output;
	}
}
//Конвертация Int, если 0 то ВЫКЛ в Стринг
String IntToString(int a) {
	String b;
	if (a == 0) { 
		b = "OFF"; 
		return b; 
	}
	else {
		b = String(a);
		return b; 
	}
}

void NextionSetup() {
	Serial2.begin(115200);
}

void NextionDrive() {
	if (PeriodNextion < MS) {
		NexSend(String(BMTt1), "BMTt1");
		NexSend(String(DHTW), "DHTW");
		NexSend(String(BMPP), "BMPP");
		NexSend(String(BMPH), "BMPH");
		NexSend(Rain_Sun, "LuxPog");
		NexSend(String(LuxLux), "LuxLux");
		NexSend(String(Smoke), "Smoke");
		NexSend(String(Fire), "Fire");
		NexSend(IntToString(DV1), "DV1");
		NexSend(IntToString(DV2), "DV2");
		NexSend(IntToString(DV3), "DV3");
		NexSend(IntToString(DV4), "DV4");
		NexSend(IntToString(DV5), "DV5");
		NexSend(String(SIn), "SIn");
		NexSend(String(SVol), "SVol");
		NexSend(BoolToString(RSw1), "RSw1");
		NexSend(BoolToString(RSw2), "RSw2");
		NexSend(BoolToString(RSw3), "RSw3");
		NexSend(BoolToString(RSw4), "RSw4");
		NexSend(BoolToString(RSw5), "RSw5");
		NexSend(BoolToString(RSw6), "RSw6");
		NexSend(BoolToString(RSw7), "RSw7");
		NexSend(BoolToString(RSw8), "RSw8");
		NexSend(String(DD), "DD");
		NexSend(String(MM), "MM");
		NexSend(String(YY), "YY");
		NexSend(RdStation, "RdSt");
		NexSend(String(hh), "hh");
		NexSend(String(mm), "mm");
		PeriodNextion = MS + IntervalNextion;
	}
}
//#########################################################################################################################################################################//
//**************************************************************************Управление Реле********************************************************************************//
//#########################################################################################################################################################################//

bool MotSwR1, MotSwR2, MotSwR3, MotSwR4, MotSwR5, MotSwR6, MotSwR7, MotSwR8;
//Верх
#define Relay1 30
#define Relay2 31
#define Relay3 32
#define Relay4 33
//Низ
#define Relay5 26
#define Relay6 27
#define Relay7 28
#define Relay8 29
//RSw1
void SetupRelayDrive() {
	pinMode(Relay1, OUTPUT);
	pinMode(Relay2, OUTPUT);
	pinMode(Relay3, OUTPUT);
	pinMode(Relay4, OUTPUT);
	pinMode(Relay5, OUTPUT);
	pinMode(Relay6, OUTPUT);
	pinMode(Relay7, OUTPUT);
	pinMode(Relay8, OUTPUT);
	digitalWrite(Relay1, LOW);
	digitalWrite(Relay2, LOW);
	digitalWrite(Relay3, LOW);
	digitalWrite(Relay4, LOW);
	digitalWrite(Relay5, LOW);
	digitalWrite(Relay6, LOW);
	digitalWrite(Relay7, LOW);
	digitalWrite(Relay8, LOW);
}

void MotSwRelay(bool SwRM, bool SwRP, int PinRelay) {
	if (SwRM) {
		digitalWrite(PinRelay,SwRP);
	}
	else {
		digitalWrite(PinRelay,SwRP);
	}
}

void RelayDrive(int Name, int Var) {
	switch (Name)
	{
	case 1:
		RSw1 = bool(Var);
		if (TestingMode) {
			Serial.print("RSw1");
			Serial.println(RSw1);
		}
		break;

	case 2:
		RSw2 = bool(Var);
		if (TestingMode) {
			Serial.print("RSw2");
			Serial.println(RSw2);
		}
		break;

	case 3:
		RSw3 = bool(Var);
		if (TestingMode) {
			Serial.print("RSw3");
			Serial.println(RSw3);
		}
		break;

	case 4:
		RSw4 = bool(Var);
		if (TestingMode) {
			Serial.print("RSw4");
			Serial.println(RSw4);
		}
		break;

	case 5:
		RSw5 = bool(Var);
		if (TestingMode) {
			Serial.print("RSw5");
			Serial.println(RSw5);
		}
		break;

	case 6:
		RSw6 = bool(Var);
		if (TestingMode) {
			Serial.print("RSw6");
			Serial.println(RSw6);
		}
		break;

	case 7:
		RSw7 = bool(Var);
		if (TestingMode) {
			Serial.print("RSw7");
			Serial.println(RSw7);
		}
		break;

	case 8:
		RSw8 = bool(Var);
		if (TestingMode) {
			Serial.print("RSw8");
			Serial.println(RSw8);
		}
		break;
	default:
		break;
	}
	if (TestingMode) {
		Serial.print("RSw-");
		Serial.println("Run");
	}
	MotSwRelay(MotSwR1, RSw1, Relay1);
	MotSwRelay(MotSwR2, RSw2, Relay2);
	MotSwRelay(MotSwR3, RSw3, Relay3);
	MotSwRelay(MotSwR4, RSw4, Relay4);
	MotSwRelay(MotSwR5, RSw5, Relay5);
	MotSwRelay(MotSwR6, RSw6, Relay6);
	MotSwRelay(MotSwR7, RSw7, Relay7);
	MotSwRelay(MotSwR8, RSw8, Relay8);
}

//#########################################################################################################################################################################//
//**************************************************************************Управление PWM*********************************************************************************//
//#########################################################################################################################################################################//
#define PWM1 9
#define PWM2 10
#define PWM3 11
#define PWM4 12
#define PWM5 13

bool MotSwPWM1, MotSwPWM2, MotSwPWM3, MotSwPWM4, MotSwPWM5;


void SetupPWMDrive() {
	pinMode(PWM1, OUTPUT);
	pinMode(PWM2, OUTPUT);
	pinMode(PWM3, OUTPUT);
	pinMode(PWM4, OUTPUT);
	pinMode(PWM5, OUTPUT);
	analogWrite(PWM1, 0);
	analogWrite(PWM2, 0);
	analogWrite(PWM3, 0);
	analogWrite(PWM4, 0);
	analogWrite(PWM5, 0);
}

void MotSwPWM(bool SwPWM, int PWMVl, int PinPWM) {
	if (SwPWM) {
		analogWrite(PinPWM, PWMVl);
	}
	else { 
		analogWrite(PinPWM, 0);
	}
}

void PWMDrive(int Name, int Var) {
	Var = map(Var, 0, 100, 0, 255);
	switch (Name){
	case 1 :
		DV1 = Var;
		if (DV1 > 0) { MotSwPWM1 = true; }
		if (TestingMode) {
			Serial.print("DV1");
			Serial.println(Var);
		}
		break;
	case 2:
		DV2 = Var;
		if (DV2 > 0) { MotSwPWM2 = true; }
		if (TestingMode) {
			Serial.print("DV2");
			Serial.println(Var);
		}
		break;
	case 3:
		DV3 = Var;
		if (DV3 > 0) { MotSwPWM3 = true; }
		if (TestingMode) {
			Serial.print("DV3");
			Serial.println(Var);
		}
		break;
	case 4:
		DV4 = Var;
		if (DV4 > 0) { MotSwPWM4 = true; }
		if (TestingMode) {
			Serial.print("DV4");
			Serial.println(Var);
		}
		break;
	case 5:
		DV5 = Var;
		if (DV5 > 0) { MotSwPWM5 = true; }
		if (TestingMode) {
			Serial.print("DV5");
			Serial.println(Var);
		}
		break;
	default:
		break;
	}
	if (TestingMode) {
		Serial.print("DV-");
		Serial.println("Run");
	}
	MotSwPWM(MotSwPWM1, DV1, PWM1);
	MotSwPWM(MotSwPWM2, DV2, PWM2);
	MotSwPWM(MotSwPWM3, DV3, PWM3);
	MotSwPWM(MotSwPWM4, DV4, PWM4);
	MotSwPWM(MotSwPWM5, DV5, PWM5);
}

//#########################################################################################################################################################################//
//************************************************************************Датчик движения********************************************************************************//
//#########################################################################################################################################################################//

#define PinMotD 38
bool MotD;

void MotionSetup() {
	pinMode(PinMotD, INPUT);
}
void MotionDrive() {
	MotD = digitalRead(PinMotD);
	if (LuxLux < 150) {
		MotSwR1 = MotD;
		MotSwPWM1 = MotD;
	}
	else {
		MotD = false;
	}
}
//#########################################################################################################################################################################//
//*****************************************************************************БЛЮПУП**************************************************************************************//
//#########################################################################################################################################################################//

//Чтение портов               
bool BtMesIN = false;					//Прилетело!


//Загрузка
void BluetoothSetup() {
	Serial1.begin(115200);
}

//Тело
void BluetoothIn() {
	if (Serial1.available() > 0) {
		BtMesIN = true;
		String input;
		while (Serial1.available() > 0 && BtMesIN==true) {
			char inputchar;
			inputchar = Serial1.read();
			if (inputchar == ':') {
				//if (TestingMode) { Serial.println(input); }
				InputDevInt = input.toInt();
				input = "";
				delay(20);
			}
			else if (inputchar == '=') {
				//if (TestingMode) { Serial.println(input); }
				InputNameInt = input.toInt();
				input = "";
				delay(20);
			}
			else if (inputchar == ';') {
				InputVarInt = input.toInt();
				//if (TestingMode) { Serial.println(input); }
				BtMesIN = false;
				MSG = true;
				input = "";
			}
			else {
				input += inputchar;
				delay(20);
			}
		}
	}
	if (!MSG) {
		InputDevInt = 0;
		InputNameInt = 0;
		InputVarInt = 0;
	}
}

void BluetoothOut(String Name, String Val) {
		Serial1.print(Name + '.' + Val + ';');
}


//#########################################################################################################################################################################//
//*************************************************************NRF24 Tranciever********************************************************************************************//
//#########################################################################################################################################################################//


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define Tr1 0xABCDABCD71LL
#define Tr2 0x544d52687CLL
#define Tr3 0x544d526832LL
#define Tr4 0x544d52683CLL
#define Tr5 0x544d526846LL
#define Tr6 0x544d526850LL
#define Tr7 0x544d52685ALL
#define Tr8 0x544d526820LL
#define Tr9 0x544d52686ELL
#define Tr10 0x544d52684BLL
#define Tr11 0x544d526841LL
#define Tr12 0x544d526855LL
#define Tr13 0x544d52685FLL
#define Tr14 0x544d526869LL

//Управляющие CE & CSN 
RF24 radio(48, 49);

//Возможные адреса
byte address[][14] = { Tr1, Tr2, Tr3, Tr4,Tr5, Tr6,Tr7, Tr8, Tr9, Tr10, Tr11, Tr12, Tr13, Tr14 };

//Массив исходящего сообщения
byte Message[16];

//Запуск модуля
void NRFSPISETUP() {
	radio.begin();
	radio.setAutoAck(0);
	radio.setRetries(0, 15);
	radio.enableAckPayload();
	radio.setPayloadSize(32);
	radio.openWritingPipe(address[0]);
	radio.setChannel(0x06);
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_250KBPS);
	radio.powerUp();
	radio.stopListening();
}

//Отправление сообщения
void NRFSPI(byte Addr, byte NameFunc, byte VarFunc, byte Com1, byte Com2, byte Com3, byte Com4, byte Com5, byte Com6, byte Com7, byte Com8, byte Com9, byte Com10, byte Com11, byte Com12, byte Com13) {
	radio.powerUp();
	byte Message[16];
	Message[0] = Addr; Message[1] = NameFunc; Message[2] = VarFunc; Message[3] = Com1; Message[4] = Com2; Message[5] = Com3; Message[6] = Com4; Message[7] = Com5; Message[8] = Com6;
	Message[9] = Com7; Message[10] = Com8; Message[11] = Com9; Message[12] = Com10; Message[13] = Com11; Message[14] = Com12; Message[15] = Com13;
	radio.write(&Message, 15);
	radio.powerDown();
}

//#########################################################################################################################################################################//
//****************************************************************************Плеер Блюпуп*********************************************************************************//
//#########################################################################################################################################################################//

//*************************************************************Управление плеером******************************************************************************************//
//Команды на усилок Кнопы
#define SndBt 1
#define RdBt 2
#define RdSt 3
//#define Snd 4

//Команды на усилок Кнопы
#define VolumePlus 1             //Громче
#define VolumeMinus 2            //Тише
#define BassPlus 3               //Бас+
#define BassMinus 4              //Бас-
#define TreblePlus 5             //ВЧ+
#define TrebleMinus 6            //ВЧ-
#define Input1Stereo 7           //Вход 1 стерео
#define Input2Stereo 8           //Вход 2 стерео
#define Input1Surround 9         //Вход 1 сюрраунд
#define Input2Surround 10        //Вход 2 сюрраунд
#define BalanseLeft 11           //Баланс левее
#define BalanseRight 12          //Баланс правее

//Команды на Радиво Кнопы
#define RDFrb  1             //Радиво поиск вперед
#define RDFrf  2             //Радиво поиск назад
#define RDBack 3             //Радиво частота +
#define RDForw 4             //Радиво частота -
#define RDVolb 5             //Радиво громче
#define RDVolf 6             //Радиво тише
#define RDCHb 7              //Радиво канал +
#define RDCHf 8              //Радиво канал -


void BalkonBtPlayerSetup() {
	TDA8425TransmitSetup();
	RadioFMSetup();
}

void BalkonBtPlayer(int Name,int Var) {
		TDA8425Driving(Name, Var);
		RadioFM(Name, Var);
}
//******************************************************************TDA8425************************************************************************************************//

byte VOLL = 0b11101101; // уровни при включении
byte VOLR = 0b11101101;
byte BASS = 0b11110111;
byte TREBLE = 0b11110101;
byte STEREOINPUT1 = 0b11001110;
byte STEREOINPUT2 = 0b11001111;
byte SURROUNDINPUT1 = 0b11011110;
byte SURROUNDINPUT2 = 0b11011111;


#define ADDRESS   0x41   
#define MAD_VOLL      0x00
#define MAD_VOLR      0x01
#define MAD_BASS      0x02
#define MAD_TREBLE    0x03
#define MAD_SWITCH    0x08

void TDA8425TransmitSetup() {
	Wire.begin();
	Wire.beginTransmission(ADDRESS);
	Wire.write(MAD_VOLL);
	Wire.write(VOLL);
	Wire.endTransmission();

	Wire.beginTransmission(ADDRESS);
	Wire.write(MAD_VOLR);
	Wire.write(VOLR);
	Wire.endTransmission();

	Wire.beginTransmission(ADDRESS);
	Wire.write(MAD_BASS);
	Wire.write(BASS);
	Wire.endTransmission();

	Wire.beginTransmission(ADDRESS);
	Wire.write(MAD_TREBLE);
	Wire.write(TREBLE);
	Wire.endTransmission();

	Wire.beginTransmission(ADDRESS);
	Wire.write(MAD_SWITCH);
	Wire.write(STEREOINPUT2);
	Wire.endTransmission();
}

void TDA8425Transmit(int ADRESS, int FUNC, int DATA) {
	Wire.beginTransmission(ADRESS);
	Wire.write(FUNC);
	Wire.write(DATA);
	Wire.endTransmission();
}

void TDA8425Driving(int Name, int Var) {
		if (Name == 1) {
			if (TestingMode) { Serial.println("TDA8425Driving"); }
			switch (Var)
			{
			case VolumePlus:
				if (TestingMode) { Serial.println("VolumePlus"); }
				if (VOLL < 0b11110011)  VOLL = VOLL + 0b00000001;
				if (VOLR < 0b11110011)  VOLR = VOLR + 0b00000001;
				TDA8425Transmit(ADDRESS, MAD_VOLL, VOLL);
				TDA8425Transmit(ADDRESS, MAD_VOLR, VOLR);

				break;
			case VolumeMinus:
				if (TestingMode) { Serial.println("VolumeMinus"); }
				if (VOLL > 0b11011010) VOLL = VOLL - 0b00000001;
				if (VOLR > 0b11011010) VOLR = VOLR - 0b00000001;
				TDA8425Transmit(ADDRESS, MAD_VOLL, VOLL);
				TDA8425Transmit(ADDRESS, MAD_VOLR, VOLR);

				break;
			case BassPlus:
				if (TestingMode) { Serial.println("BassPlus"); }
				if (BASS < 0b11111011) BASS++;
				TDA8425Transmit(ADDRESS, MAD_BASS, BASS);

				break;
			case BassMinus:
				if (TestingMode) { Serial.println("BassMinus"); }
				if (BASS > 0b11110010) BASS--;
				TDA8425Transmit(ADDRESS, MAD_BASS, BASS);

				break;
			case TreblePlus:
				if (TestingMode) { Serial.println("TreblePlus"); }
				if (TREBLE < 0b11111011) TREBLE++;
				TDA8425Transmit(ADDRESS, MAD_TREBLE, TREBLE);

				break;
			case TrebleMinus:
				if (TestingMode) { Serial.println("TrebleMinus"); }
				if (TREBLE > 0b11110010) TREBLE--;
				TDA8425Transmit(ADDRESS, MAD_TREBLE, TREBLE);

				break;
			case Input1Stereo:
				if (TestingMode) { Serial.println("Input1Stereo"); }
				TDA8425Transmit(ADDRESS, MAD_SWITCH, STEREOINPUT1);

				break;
			case Input2Stereo:
				if (TestingMode) { Serial.println("Input2Stereo"); }
				TDA8425Transmit(ADDRESS, MAD_SWITCH, STEREOINPUT2);

				break;
			case Input1Surround:
				if (TestingMode) { Serial.println("Input1Surround"); }
				TDA8425Transmit(ADDRESS, MAD_SWITCH, SURROUNDINPUT1);

				break;
			case Input2Surround:
				if (TestingMode) { Serial.println("Input2Surround"); }
				TDA8425Transmit(ADDRESS, MAD_SWITCH, SURROUNDINPUT2);

				break;
			case BalanseLeft:
				if (TestingMode) { Serial.println("BalanseLeft"); }
				if (VOLL < 0b11110011) VOLL = VOLL + 0b00000001;
				if (VOLR > 0b11011010) VOLR = VOLR - 0b00000001;
				TDA8425Transmit(ADDRESS, MAD_VOLL, VOLL);
				TDA8425Transmit(ADDRESS, MAD_VOLR, VOLR);

				break;
			case BalanseRight:
				if (TestingMode) { Serial.println("BalanseRight"); }
				if (VOLR < 0b11110011)  VOLR = VOLR + 0b00000001;
				if (VOLL > 0b11011010) VOLL = VOLL - 0b00000001;
				TDA8425Transmit(ADDRESS, MAD_VOLL, VOLL);
				TDA8425Transmit(ADDRESS, MAD_VOLR, VOLR);
				break;
			default:
				break;
			}
			
		}
}

//********************************************************************RADIO*************************************************************************************************//
/*//Команды на Радиво Станции
#define RPr1 1                // -1----87.50	  Дорожное радио
#define RPr2 2                // -2----88.00	  Ретро FM
#define RPr3 3                // -3----88.40	  Авторадио
#define RPr4 4                // -4----88.90	  Радио Юмор FM
#define RPr5 5                // -5----89.30	  Радио Вести FM
#define RPr6 6                // -6----89.70	  Радио Зенит
#define RPr7 7                // -7----90.10	  Радио Эрмитаж
#define RPr8 8                // -8----90.60	  Радио для двоих
#define RPr9 9                // -9----91.10	  Новое Радио
#define RPr10 10              //-10----91.50	  Радио Эхо Петербурга
#define RPr11 11              //-11----92.00	  Комсомольская правда   (план)
#define RPr12 12              //-12----92.40	  Радио Хит FM   (план)
#define RPr13 13              //-13----92.90	  IZ.ru (звук)
#define RPr14 14              //-14----95.00	  Радио Energy
#define RPr15 15              //-15----95.50	  Studio 21
#define RPr16 16              //-16----95.90	  Comedy Radio
#define RPr17 17              //-17----97.00	  Радио Дача
#define RPr18 18              //-18----98.60	  Royal Radio
#define RPr19 19              //-19----99.00	  Радио России С.-Петербург FM
#define RPr20 20              //-20----100.10	  Радио Популярная классика
#define RPr21 21              //-21----100.50	  Радио Европа Плюс
#define RPr22 22              //-22----100.90	  Радио Питер FM
#define RPr23 23              //-23----101.40	  Эльдорадио
#define RPr24 24              //-24----102.00	  Радио Страна FM
#define RPr25 25              //-25----102.40	  Radio METRO
#define RPr26 26              //-26----102.80	  Радио Максимум
#define RPr27 27              //-27----103.40	  Радио DFM
#define RPr28 28              //-28----103.70	  Детское радио
#define RPr29 29              //-29----104.00	  НАШЕ Радио
#define RPr30 30              //-30----104.40	  Радио Шансон
#define RPr31 31              //-31----104.80	  IZ.ru (звук)
#define RPr32 32              //-32----105.30	  Love Радио
#define RPr33 33              //-33----105.90	  Радио Монте-Карло
#define RPr34 34              //-34----106.30	  Радио Рекорд
#define RPr35 35              //-35----107.00	  Радио Маяк
#define RPr36 36              //-36----107.40	  Радио Business FM
#define RPr37 37              //-37----107.80	  Русское Радио
*/

//Библиотеки
#include <radio.h>
#include <RDA5807M.h>

#define FIX_BAND     RADIO_BAND_FM   //< The band that will be tuned by this sketch is FM.
#define FIX_STATION  10280            //< The station that will be tuned by this sketch is 102.80 MHz.
#define FIX_VOLUME   4               //< The volume that will be set by this sketch is level 4.

RDA5807M FMTuner;    // Create an instance of Class for RDA5807M Chip

					 //Частота
int FMchannel = 1;
//Уровень звука

int FMvolume = 4;

//Частота
int FMFreqF = 10280;

int RadioStations[] = { 8750,8800,8840,8890,8930,8970,9010,9060,9110,9150,9200,9240,9290,9500,9550,9590,9700,9860,9900,10010,10050,10090,10140,10200,10240,10280,10340,10370,10400,10440,10480,10530,10590,10630,10700,10740,10780 };

String RadiostationNextion(int RadioStation){
	String Strung;
	switch (RadioStation)
	{
	case 0:
		Strung = "Dorojnoe";
		break; 
	case 1:
		Strung = "Retro FM";
		break;
	case 2:
		Strung = "Avtoradio";
		break;
	case 3:
		Strung = "Humor FM";
		break;
	case 4:
		Strung = "Vesti FM";
		break;
	case 5:
		Strung = "Zenith";
		break;
	case 6:
		Strung = "Hermitage";
		break;
	case 7:
		Strung = "Dlja dvoih";
		break;
	case 8:
		Strung = "Novoe radio";
		break;
	case 9:
		Strung = "Echo S-Pb";
		break;
	case 10:
		Strung = "Komsom radio";
		break;
	case 11:
		Strung = "Hit FM";
		break;
	case 12:
		Strung = "IZ.ru";
		break;
	case 13:
		Strung = "Energy";
		break;
	case 14:
		Strung = "Studio 21";
		break;
	case 15:
		Strung = "Comedy Radio";
		break;
	case 16:
		Strung = "Dacha";
		break;
	case 17:
		Strung = "Royal Radio";
		break;
	case 18:
		Strung = "Radio Rossii";
		break;
	case 19:
		Strung = "Pop. klassika";
		break;
	case 20:
		Strung = "Europa+";
		break;
	case 21:
		Strung = "Piter FM";
		break;
	case 22:
		Strung = "Eldoradio";
		break;
	case 23:
		Strung = "Strana FM";
		break;
	case 24:
		Strung = "METRO";
		break;
	case 25:
		Strung = "Maksimum";
		break;
	case 26:
		Strung = "DFM";
		break;
	case 27:
		Strung = "Detskoe radio";
		break;
	case 28:
		Strung = "Nashe radio";
		break;
	case 29:
		Strung = "Shanson";
		break;
	case 30:
		Strung = "IZ.ru";
		break;
	case 31:
		Strung = "Love Radio";
		break;
	case 32:
		Strung = "Monte-Carlo";
		break;
	case 33:
		Strung = "Record";
		break;
	case 34:
		Strung = "Mayak";
		break;
	case 35:
		Strung = "Business FM";
		break;
	case 36:
		Strung = "Russkoe radio";
		break;
	default:
		Strung = "HZ";
		break;
	}
	return Strung;

}

void RadioFMSetup() {
	FMTuner.init();
	FMTuner.setBandFrequency(FIX_BAND, FMFreqF);
	FMTuner.setVolume(FMvolume);
	FMTuner.setMono(false);
	FMTuner.setMute(false);
}

void RadioFM(int Name, int Var) {
	if (TestingMode) { Serial.println("Radio"); }
		if (Name == 2) {
			if (TestingMode) { Serial.println("RadioFM"); }
			switch (Var)
			{
			case RDFrb:
				if (TestingMode) {Serial.println("RDFrb");}
				FMFreqF = FMFreqF - 10;
				if (FMFreqF < 870) {
					FMFreqF = 10800;
				}
				FMTuner.setBandFrequency(FIX_BAND, FMFreqF);
				break;

			case RDFrf:
				if (TestingMode) { Serial.println("RDFrf"); }
				FMFreqF = FMFreqF + 10;
				if (FMFreqF > 1080) {
					FMFreqF = 870;
				}
				FMTuner.setBandFrequency(FIX_BAND, FMFreqF);
				break;

			case RDBack:
				if (TestingMode) { Serial.println("RDBack"); }
				FMTuner.seekDown();
				break;
			case RDForw:
				if (TestingMode) { Serial.println("RDForw"); }
				FMTuner.seekUp();
				break;
			case RDVolb:
				if (TestingMode) { Serial.println("RDVolb"); }
				FMvolume = FMvolume - 1;
				if (FMvolume < 0) {
					FMvolume = 0;
				}
				FMTuner.setVolume(FMvolume);
				break;
			case RDVolf:
				if (TestingMode) { Serial.println("RDVolf"); }
				FMvolume = FMvolume + 1;
				if (FMvolume > 10) {
					FMvolume = 10;
				}
				FMTuner.setVolume(FMvolume);
				break;
			case RDCHb:
				if (TestingMode) { Serial.println("RDCHb"); }
				if (FMchannel > 1) {
					FMchannel = FMchannel - 1;
				}
				FMFreqF = RadioStations[FMchannel];
				FMTuner.setBandFrequency(FIX_BAND, FMFreqF);
				break;
			case RDCHf:
				if (TestingMode) { Serial.println("RDCHf"); }
				if (FMchannel <= 36) {
					FMchannel = FMchannel + 1;
				}
				FMFreqF = RadioStations[FMchannel];
				FMTuner.setBandFrequency(FIX_BAND, FMFreqF);
				break;
			default:
				InputVarInt = 0;
				break;
			}
		}
		else if (Name == 3) {
			if (TestingMode) { Serial.println("Radiostation"); }
			FMchannel = Var;
			FMFreqF = RadioStations[FMchannel];
			FMTuner.setBandFrequency(FIX_BAND, FMFreqF);
			RdStation =RadiostationNextion(FMchannel);
		}
		
}

//#########################################################################################################################################################################//
//***************************************************************ОСНОВНОЕ ТЕЛО*********************************************************************************************//
//#########################################################################################################################################################################//
//*****************************************************ЗАГРУЗКА*******************************************************//
void setup()
{
	ServerSetup();
}

//*****************************************************ЦИКЛ***********************************************************//
void loop()
{
	Server();
}
