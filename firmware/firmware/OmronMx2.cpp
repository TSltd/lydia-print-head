#include "OmronMx2.h"
#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

#define MODBUSMASTER_DEBUG
#undef MODBUS_FREERTOS

//#include <ModbusMaster.h> // Using ModbusMaster modified for OMRON MX2 https://github.com/4-20ma/ModbusMaster
#include "ModbusMaster.h" // Using ModbusMaster modified for OMRON MX2 https://github.com/4-20ma/ModbusMaster

const char *nameOmron = {" Omron MX2 "}; //   Name, brand of inverter
const char *cYes = {" Yes "};
const char *cNo = {" No "};
const char *cOne = {" 1 "};
const char *cZero = {" 0 "};
const char *cOk = {" Ok "};
const char *cError = {" error "};
const char *cInvalid = {" invalid "};
const char *cStrEnd = {" \ n "};
const char *cErrorRS485 = {" % s : Read error % s , code = % d repeat ... \ n "}; // name, function, code
const char *cErrorMutex = {" Function % s : % s , mutex is buzy \ n "};           // function, mutex
const char *cAddHeat = {" + "};

__attribute__((always_inline)) inline void _delay(int t)
{
  delay(t);
}

const char *noteFC_OK = {" Modbus communication established "}; // All right
const char *noteFC_NO = {" Modbus communication lost, the inverter is locked "};
const char *noteFC_NONE = {" missing in this configuration "};

const char *fc_ON_OFF = {" ON_OFF "};            // Flag for turning on / off (frequency control)
const char *fc_INFO = {" INFO "};                // Get information from the inverter (table !!)
const char *fc_NAME = {" NAME "};                // Inverter name
const char *fc_NOTE = {" NOTE "};                // Get a description of the frequency converter. Line 80 + 1
const char *fc_PIN = {" PIN "};                  // Get the pin number where analog FC is attached
const char *fc_PRESENT = {" PRESENT "};          // The presence of FC in the configuration.
const char *fc_STATE = {" STATE "};              // Status of the inverter (read)
const char *fc_FC = {" FC "};                    // The target frequency of the inverter is 0.01
const char *fc_cFC = {" cFC "};                  // Current inverter frequency (read)
const char *fc_cPOWER = {" cPOWER "};            // Current power (read)
const char *fc_INFO1 = {" INFO1 "};              // The first line below the inverter picture in the circuit
const char *fc_cCURRENT = {" cCURRENT "};        // Current current (read)
const char *fc_AUTO_RESET_FAULT = {" ARSTFLT "}; // Flag to automatically reset non-critical inverter error
const char *fc_LogWork = {" LOGW "};             // flag log while running
const char *fc_ANALOG = {" AN "};                // Flag of analog control
const char *fc_DAC = {" DAC "};                  // Get the current DAC value
const char *fc_LEVEL0 = {" L0 "};                // Frequency level 0 in the DAC samples
const char *fc_LEVEL100 = {" L100 "};            // Frequency level 100% in the DAC samples
const char *fc_LEVELOFF = {" LOFF "};            // Minimum power at which the chastotnik turns off (minimum power limit)
const char *fc_BLOCK = {" BLOCK "};              // flag inverter global error - inverter operation is disabled; blocking can be reset by setting 0
const char *fc_ERROR = {" ERROR "};              // Get the error code
const char *fc_UPTIME = {" UPTIME "};            // Pid controller update algorithm time (ms) The main control loop
const char *fc_PID_STOP = {" PID_STOP "};        // Percentage of the level of protection (power, current, pressure, temperature) at which the frequency increase is blocked by the pid
const char *fc_PID_FREQ_STEP = {" PID_STEP "};   // The maximum step (to increase) the frequency change with PID control of 0.01 Hz It is necessary that the ERV be in time
const char *fc_START_FREQ = {" FRQ "};           // Inverter starting frequency (cm compressor) at 0.01 Hz
const char *fc_START_FREQ_BOILER = {" FRQB "};   // Starting frequency of the inverter (cm compressor) in 0.01 GHz of hot water
const char *fc_MIN_FREQ = {" MIN "};             // Minimum inverter frequency (cm compressor) in 0.01 Hz
const char *fc_MIN_FREQ_COOL = {" MINC "};       // The minimum frequency of the inverter during cooling at 0.01 Hz
const char *fc_MIN_FREQ_BOILER = {" MINB "};     // The minimum frequency of the inverter when heating the hot water in 0.01 Hz
const char *fc_MIN_FREQ_USER = {" MINU "};       // Minimum inverter frequency MANUAL MODE (cm compressor) at 0.01 Hz
const char *fc_MAX_FREQ = {" MAX "};             // Maximum inverter frequency (cm compressor) at 0.01 Hz
const char *fc_MAX_FREQ_COOL = {" MAXC "};       // Maximum inverter frequency in cooling mode at 0.01 Hz
const char *fc_MAX_FREQ_BOILER = {" MAXB "};     // The maximum frequency of the inverter in the DHW mode at 0.01 Hz the absorption of the boiler is usually less than CO
const char *fc_MAX_FREQ_USER = {" MAXU "};       // Maximum inverter frequency MANUAL MODE (cm compressor) at 0.01 Hz
const char *fc_STEP_FREQ = {" STEP "};           // Step to decrease the inverter when reaching the maximum temperature, power and current (cm compressor) at 0.01 Hz
const char *fc_STEP_FREQ_BOILER = {" STEPB "};   // Step to decrease the inverter when reaching the maximum temperature, power and dhw current of 0.01 Hz
const char *fc_DT_COMP_TEMP = {" DTC "};         // Protection by compressor temperature - how many degrees does not reach the maximum (TCOMP) and at the same time there is a decrease in frequency
const char *fc_DT_TEMP = {" DT "};               // Exceeding the temperature from the settings (feed) at which the protection is triggered (frequency decreases) in hundredths of a degree
const char *fc_DT_TEMP_BOILER = {" DTB "};       // Exceeding the temperature from the settings (supply) at which the DHW protection in hundredths of a degree is triggered
const char *fc_MB_ERR = {" MB_ERR "};            // Modbus errors
const char *fc_FC_TIME_READ = {" TR "};          // Poll time
const char *fc_fFC_RetOil = {" FRO "};           // Flag oil return
const char *fc_FC_RETOIL_FREQ = {" FRF "};       // Frequency
const char *fc_ReturnOilPeriod = {" ROP "};      // Oil return time
const char *fc_ReturnOilPerDivHz = {" ROPH "};   // The frequency at which oil returns
const char *fc_ReturnOilEEV = {" ROE "};         // Steps of the EDS at which oil is returned
const boolean DEVICEFC = false;                  // Наличие Частотного преобразователя в конфигурации
#define fModbus 1                                // флаг наличие modbus
typedef QueueHandle_t SemaphoreHandle_t;
SemaphoreHandle_t xModbusSemaphore;   // Семафор Modbus, инвертор запас на счетчик
#define MODBUS_PORT_NUM Serial3       // Аппаратный порт куда прицеплен Modbus
#define MODBUS_PORT_SPEED 19200       // Скорость порта куда прицеплен частотник и счетчик
#define MODBUS_PORT_CONFIG SERIAL_8N1 // Конфигурация порта куда прицеплен частотник и счетчик
#define MODBUS_TIME_WAIT 2000         // Время ожидания захвата мютекса для modbus мсек

inline void SemaphoreGive(QueueHandle_t xSemaphore)
{
  //if(xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) xSemaphoreGive(xSemaphore);
}
unsigned int i10toa(int value, char *string, unsigned int zero_pad)
{
  char *pbuffer = string;
  unsigned char negative;
  if (value < 0)
  {
    negative = 1;
    value = -value;
  }
  else
    negative = 0;
  do
  {
    *(pbuffer++) = '0' + value % 10;
    value /= 10;
  } while (value > 0);
  for (unsigned int i = (pbuffer - string); i < zero_pad; i++)
    *(pbuffer++) = '0';
  if (negative)
    *(pbuffer++) = '-';
  *(pbuffer) = '\0';
  unsigned int len = (pbuffer - string);
  for (unsigned int i = 0; i < len / 2; i++)
  {
    char j = string[i];
    string[i] = string[len - i - 1];
    string[len - i - 1] = j;
  }
  return len;
}
uint8_t _ftoa(char *outstr, float val, unsigned char precision)
{
  while (*outstr)
    outstr++;
  char *instr = outstr;

  // compute the rounding factor and fractional multiplier
  float roundingFactor = 0.5f;
  unsigned long mult = 1;
  unsigned char padding = precision;
  while (precision--)
  {
    roundingFactor /= 10.0f;
    mult *= 10;
  }
  if (val < 0.0f)
  {
    *outstr++ = '-';
    val = -val;
  }
  val += roundingFactor;
  outstr += i10toa((long)val, outstr, 0);
  if (padding > 0)
  {
    *(outstr++) = '.';
    outstr += i10toa((val - (long)val) * mult, outstr, padding);
  }
  return outstr - instr;
}
char *_itoa(int value, char *string)
{
  char *ret = string;
  while (*string)
    string++;

  char *pbuffer = string;
  unsigned char negative = 0;

  if (value < 0)
  {
    negative = 1;
    value = -value;
  }

  /* This builds the string back to front ... */
  do
  {
    *(pbuffer++) = '0' + value % 10;
    value /= 10;
  } while (value > 0);

  if (negative)
    *(pbuffer++) = '-';

  *(pbuffer) = '\0';

  /* ... now we reverse it (could do it recursively but will
	 * conserve the stack space) */
  uint32_t len = (pbuffer - string);
  for (uint32_t i = 0; i < len / 2; i++)
  {
    char j = string[i];
    string[i] = string[len - i - 1];
    string[len - i - 1] = j;
  }

  return ret;
}

static void jprintf(const char *format, ...)
{
  Serial.println(format);
}
int8_t set_Error(int8_t _err, char *nam)
{
  Serial.println("error");
  Serial.println(_err);
  Serial.println(nam);

  return OK;
}
// End class
static uint8_t Modbus_Entered_Critical = 0;
static inline void idle() // задержка между чтениями отдельных байт по Modbus
{
  //		delay(1);		// Не отдает время другим задачам
  _delay(1); // Отдает время другим задачам
}
static inline void preTransmission() // Функция вызываемая ПЕРЕД началом передачи
{
#ifdef PIN_MODBUS_RSE
  digitalWriteDirect(PIN_MODBUS_RSE, HIGH);
#endif
  _delay(10); // что бы слейв не терял первый бит
  //    Modbus_Entered_Critical = TaskSuspendAll(); // Запрет других задач во время передачи по Modbus

  //Serial.println("critical");
}
static inline void postTransmission() // Функция вызываемая ПОСЛЕ окончания передачи
{
  if (Modbus_Entered_Critical)
  {
    //	xTaskResumeAll();
    Serial.println("critical postTransmission");
    Modbus_Entered_Critical = 0;
  }
#ifdef PIN_MODBUS_RSE
#if MODBUS_TIME_TRANSMISION != 0
  _delay(MODBUS_TIME_TRANSMISION); // Минимальная пауза между командой и ответом 3.5 символа
#endif
  digitalWriteDirect(PIN_MODBUS_RSE, LOW);
#endif
}

// Инициализация Modbus без проверки связи связи
int8_t devModbus::initModbus()
{
#ifdef MODBUS_PORT_NUM

  flags = 0x00;
  SETBIT1(flags, fModbus); // модбас присутствует
#ifdef PIN_MODBUS_RSE
  pinMode(PIN_MODBUS_RSE, OUTPUT); // Подготовка управлением полудуплексом
  digitalWriteDirect(PIN_MODBUS_RSE, LOW);
#endif
  // Controllino_RS485Init();
  MODBUS_PORT_NUM.begin(MODBUS_PORT_SPEED, MODBUS_PORT_CONFIG); // SERIAL_8N1 - настройки по умолчанию
  RS485.begin(1, MODBUS_PORT_NUM);      

  // Назначение функций обратного вызова
  RS485.preTransmission(preTransmission);
  RS485.postTransmission(postTransmission);
  RS485.idle(idle);
  err = OK; // Связь есть
#else
  flags = 0x00;
  SETBIT0(flags, fModbus); // модбас отсутвует
  err = ERR_NO_MODBUS;
#endif
  return err;
}

int8_t devModbus::LinkTestOmronMX2()
{
  uint16_t result, ret;
  err = OK;
  RS485.set_slave(FC_MODBUS_ADR);


  result = RS485.LinkTestOmronMX2Only(TEST_NUMBER); // Послать команду проверки связи
  Serial.println("test : LinkTestOmronMX2");
  Serial.println(result);
  if (result == RS485.ku8MBSuccess)
    ret = RS485.getResponseBuffer(0); // Получить данные с ответа
  else
    return err = ERR_485_INIT; // Ошибка инициализации
  if (TEST_NUMBER != ret)
    return err = ERR_MODBUS_MX2_0x05; // Контрольные данные не совпали
  return err;
}
static devModbus Modbus;
// ------------------------------------------------ ------------------------------------------
// FREQUENCY CONVERTER ONLY ONE PIECE ALWAYS (not an array) ---------------------------
// class initialization
int8_t devOmronMX2::initFC()
{

  err = OK;                   // ошибка частотника (работа) при ошибке останов ТН
  numErr = 0;                 // число ошибок чтение по модбасу для статистики
  number_err = 0;             // Число ошибок связи при превышении FC_NUM_READ блокировка инвертора
  FC = 0;                     // Целевая частота частотика
  freqFC = 0;                 // текущая частота инвертора
  power = 0;                  // Тееущая мощность частотника
  current = 0;                // Текуший ток частотника
  startCompressor = 0;        // время старта компрессора
  state = ERR_LINK_FC;        // Состояние - нет связи с частотником
  dac = 0;                    // Текущее значение ЦАП
  testMode = NORMAL;          // Значение режима тестирования
  name = (char *)nameOmron;   // Имя
  note = (char *)noteFC_NONE; // Описание инвертора   типа нет его
  // Настройки по умолчанию
  _data.Uptime = DEF_FC_UPTIME;                     // Время обновления алгоритма пид регулятора (мсек) Основной цикл управления
  _data.PidFreqStep = DEF_FC_PID_FREQ_STEP;         // Максимальный шаг (на увеличение) изменения частоты при ПИД регулировании в 0.01 Гц Необходимо что бы ЭРВ успевал
  _data.PidStop = DEF_FC_PID_STOP;                  // Проценты от уровня защит (мощность, ток, давление, темпеартура) при которой происходит блокировка роста частоты пидом
  _data.dtCompTemp = DEF_FC_DT_COMP_TEMP;           // Защита по температуре компрессора - сколько градусов не доходит до максимальной (TCOMP) и при этом происходит уменьшение частоты
  _data.startFreq = DEF_FC_START_FREQ;              // Стартовая скорость инвертора (см компрессор) в 0.01
  _data.startFreqBoiler = DEF_FC_START_FREQ_BOILER; // Стартовая скорость инвертора (см компрессор) в 0.01 ГВС
  _data.minFreq = DEF_FC_MIN_FREQ;                  // Минимальная  скорость инвертора (см компрессор) в 0.01
  _data.minFreqCool = DEF_FC_MIN_FREQ_COOL;         // Минимальная  скорость инвертора при охлаждении в 0.01
  _data.minFreqBoiler = DEF_FC_MIN_FREQ_BOILER;     // Минимальная  скорость инвертора при нагреве ГВС в 0.01
  _data.minFreqUser = DEF_FC_MIN_FREQ_USER;         // Минимальная  скорость инвертора РУЧНОЙ РЕЖИМ (см компрессор) в 0.01
  _data.maxFreq = DEF_FC_MAX_FREQ;                  // Максимальная скорость инвертора (см компрессор) в 0.01
  _data.maxFreqCool = DEF_FC_MAX_FREQ_COOL;         // Максимальная скорость инвертора в режиме охлаждения  в 0.01
  _data.maxFreqBoiler = DEF_FC_MAX_FREQ_BOILER;     // Максимальная скорость инвертора в режиме ГВС в 0.01 Гц поглощение бойлера обычно меньше чем СО
  _data.maxFreqUser = DEF_FC_MAX_FREQ_USER;         // Максимальная скорость инвертора РУЧНОЙ РЕЖИМ (см компрессор) в 0.01
  _data.stepFreq = DEF_FC_STEP_FREQ;                // Шаг уменьшения инвертора при достижении максимальной температуры, мощности и тока (см компрессор) в 0.01
  _data.stepFreqBoiler = DEF_FC_STEP_FREQ_BOILER;   // Шаг уменьшения инвертора при достижении максимальной температуры, мощности и тока ГВС в 0.01
  _data.dtTemp = DEF_FC_DT_TEMP;                    // Привышение температуры от уставок (подача) при которой срабатыват защита (уменьшается частота) в сотых градуса
  _data.dtTempBoiler = DEF_FC_DT_TEMP_BOILER;       // Привышение температуры от уставок (подача) при которой срабатыват защита ГВС в сотых градуса
#ifdef FC_ANALOG_CONTROL
  _data.level0 = 0;      // Отсчеты ЦАП соответсвующие 0   мощности
  _data.level100 = 4096; // Отсчеты ЦАП соответсвующие 100 мощности
  _data.levelOff = 10;   // Минимальная мощность при котором частотник отключается (ограничение минимальной мощности)
#endif

  if (Modbus.initModbus() == OK)
    jprintf(" modbus OK\r\n"); //  выводим сообщение об установлении связи
  else
  {
    jprintf(" modbus not present config\r\n");
  }

  flags = 0x00;             // флаги  0 - наличие FC
  _data.setup_flags = 0x00; // флаги
  Serial.println("mb presetn test");
  if (!Modbus.get_present()) // modbus отсутствует
  {
    SETBIT0(flags, fFC); // Инвертор не рабоатет
    jprintf("%s, modbus not found, block.\n", name);
    err = ERR_NO_MODBUS;
    return err;
  }
  else if (DEVICEFC == true)
  {
    SETBIT1(flags, fFC); // наличие частотника в текушей конфигурации
  }

  if (get_present())
  {
    jprintf("Invertor %s: present config\r\n", name);
  }
  else
  {
    jprintf("Invertor %s: none config\r\n", name);
    //  return err;
  } // выходим если нет инвертора

  note = (char *)noteFC_OK;        // Описание инвертора есть
  err = Modbus.LinkTestOmronMX2(); // проверка связи с инвертором  xModbusSemaphore не используем так как в один поток
  check_blockFC();
  if (err != OK)
  {
    Serial.println("link test:");
    Serial.println(note);
    Serial.println(err);
    return err; // связи нет выходим
  }

  jprintf("Test link Modbus %s: OK\r\n", name); // Тест пройден

  return;
  // Если частотник работает то остановить его
  get_readState(); //  Получить состояние частотника
  switch (state)   // В зависимости от состояния
  {
  case 0:
  case 2:
    break; // ОСТАНОВКА ничего не делаем
  case 3:
    stop_FC(); // ВРАЩЕНИЕ Послать команду стоп и ждать остановки
    while ((state != 2) || (state != 4))
    {
      get_readState();
      // jprintf("Wait stop %s . . .\r\n", name);
      _delay(3000);
    }
    break;
  case 4: // ОСТАНОВКА С ВЫБЕГОМ  ждать остановки
    break;
  case 5:
    stop_FC(); // ТОЛЧОВЫЙ ХОД Послать команду стоп и ждать остановки
    while ((state != 2) || (state != 4))
    {
      get_readState();
      // jprintf("Wait stop %s . . .\r\n", name);
      _delay(3000);
    }
    break;
  case 6: // ТОРМОЖЕНИЕ ПОСТОЯННЫМ ТОКОМ
  case 7:
    err = ERR_MODBUS_STATE;
    set_Error(err, name);
    break; // ВЫПОЛНЕНИЕ ПОВТОРНОЙ ПОПЫТКИ Подъем ошибки на верх и останов ТН
  case 8:
    break; // АВАРИЙНОЕ ОТКЛЮЧЕНИЕ
  case 9:
    break; // ПОНИЖЕНОЕ ПИТАНИЕ
  case -1:
    break;
  default:
    err = ERR_MODBUS_STATE;
    set_Error(err, name);
    break; // Подъем ошибки на верх и останов ТН
  }
  if (err != OK)
    return err;

  // Установить стартовую частоту
  set_target(_data.startFreq, true, _data.minFreqUser, _data.maxFreqUser); // режим н знаем по этому границы развигаем
  return err;
}

#define progOK " Register% s to% d \ r \ n "          // Line for displaying a message about successful register programming
#define progErr " Error setting register% s \ r \ n " // Line for displaying a message about failed register programming

#ifndef FC_ANALOG_CONTROL // NOT ANALOGUE MANAGEMENT
// Program a separate inverter register
// adrReg - register address
// nameReg - register name
// valReg - register value
int8_t devOmronMX2::progReg16(uint16_t adrReg, char *nameReg, uint16_t valReg)
{
  _delay(50);
  if ((err = write_0x06_16(adrReg, valReg)) == OK)
  {
    jprintf(progOK, nameReg, valReg);
  }
  else
  {
    jprintf(progErr, nameReg);
  }
  return err;
}
int8_t devOmronMX2 ::progReg32(uint16_t adrReg, char *nameReg, uint32_t valReg)
{
  _delay(50);
  if ((err = write_0x10_32(adrReg, valReg)) == OK)
  {
    jprintf(progOK, nameReg, valReg);
  }
  else
  {
    jprintf(progErr, nameReg);
  }
  return err;
}
#endif // NOT ANALOGUE MANAGEMENT

// Programming the inverter for a specific compressor
int8_t devOmronMX2 ::progFC()
{
#ifndef FC_ANALOG_CONTROL // NOT ANALOGUE MANAGEMENT \
                          //  jprintf(" Programming% s.. \ r \ n ", name);
  // Setting the inverter for a specific compressor Registers Hxxx Permanent magnet motor (PM motor)
  progReg16(MX2_b171, (char *)"b171", 0x03); // b171 IF mode selection b171 read / write 0 (off), 1 (IM mode), 2 (high frequency mode), 3 (PM mode) = 03
  progReg16(MX2_b180, (char *)"b180", 0x01); // b180 Starting the initialization process
                                             // 	while (read_0x03_16 (MX2_H102) == 1) _delay (100); // Delay on initialization of the inverter - wait until the P102 register appears
  //jprintf ( " Wait initialization.. \ r \ n " );
  _delay(7000);
  progReg16(MX2_H102, (char *)"H102", valH102); // H102 Setting the PM motor code 00 (standard Omron data) 01 (auto-tuning data) = 1
  progReg16(MX2_H103, (char *)"H103", valH103); // H103 PM engine power (0.1 / 0.2 / 0.4 / 0.55 / 0.75 / 1.1 / 1.5 / 2.2 / 3.0 / 3.7 / 4, 0 / 5.5 / 7.5 / 11.0 / 15.0 / 18.5) = 7
  progReg16(MX2_H104, (char *)"H104", valH104); // H104 Setting the number of poles of the PM motor = 4
  progReg16(MX2_H105, (char *)"H105", valH105); // H105 Rated current of the PM motor = 1000 (this is 11A)
  progReg16(MX2_H106, (char *)"H106", valH106); // H106 PM motor constant R From 0.001 to 65.535 ohms = 0.55 * 1000
  progReg16(MX2_H107, (char *)"H107", valH107); // H107 Ld constant of the PM engine From 0.01 to 655.35 mH = 2.31 * 100
  progReg16(MX2_H108, (char *)"H108", valH108);
  progReg16(MX2_H109, (char *)"H109", valH109);
  progReg32(MX2_H110, (char *)"H110", valH110);   // H110 Constant J PM-engine From 0.001 to 9999,000 kg / m² = 0.01
  progReg16(MX2_H119, (char *)"H119", valH119);   // H119 Engine PM stabilization constant 0 to 120% s = 100
  progReg16(MX2_H121, (char *)"H121", valH121);   // H121 Minimum frequency of the PM motor From 0.0 to 25.5% = 10 (default)
  progReg16(MX2_H122, (char *)"H122", valH122);   // H122 Idling current PM of the motor From 0.00 to 100.00% = 50 (default)
  progReg16(MX2_C001, (char *)"C001", valC001);   // C001 Input function [1] 0 (FW: go forward) = 0
  progReg16(MX2_C004, (char *)"C004", valC004);   // C004 Input function [4] 18 (RS: reset) = 18
#ifndef DEMO                                      // for the demo you do not need to configure thermal protection otherwise the eternal error E35.1
  progReg16(MX2_C005, (char *)" C005 ", valC005); // C005 Input function [5] [also input “PTC”] = 19 PTC Thermistor with positive TCS for thermal protection (only C005)
#endif
  progReg16(MX2_C026, (char *)" C026 ", valC026);              // C026 Relay output function 5 (AL: error signal) = 05
  progReg16(MX2_b091, (char *)" b091 ", valb091);              // b091 Choice of stopping method 0 (braking to a complete stop), 1 (coasting stop) = 1
  progReg16(MX2_b021, (char *)" b021 ", valb021);              // b021 Operating mode with overload limitation = 1
  progReg16(MX2_b022, (char *)" b022 ", valb022);              // b022 Overload restriction level 200 ... 2000 (0.1%) =
  progReg16(MX2_b023, (char *)" b023 ", valb023);              // b023 Braking time with overload limitation (0.1 sec) = 10
  progReg16(MX2_A001, (char *)" A001 ", valA001);              // A001 Frequency reference source = 03
  progReg16(MX2_A002, (char *)" A002 ", valA002);              // A002 Source of the “Move” command
  progReg16(MX2_A003, (char *)" A003 ", FC_BASE_FREQ / 10);    // A003 fundamental frequency
  progReg16(MX2_A004, (char *)" A004 ", DEF_FC_MAX_FREQ / 10); // A004 setting the maximum frequency
  progReg32(MX2_F002, (char *)" F002 ", FC_ACCEL_TIME);        // F002 Acceleration Time
  progReg32(MX2_F002, (char *)" F003 ", FC_DEACCEL_TIME);      // F003 Acceleration Braking
  jprintf(" ... OK \ r \ n ");
#else
  jprintf(" Analog control - no support programm Omron MX2 \ r \ n ");
#endif // NOT ANALOGUE MANAGEMENT
  return err;
}

// Set Target Frequency
// parameter to show message message or not, two remaining border parameters
int8_t devOmronMX2::set_target(int16_t x, boolean show, int16_t _min, int16_t _max)
{
  err = OK;
#ifdef DEMO
  if ((x > = _ min) && (x <= _ max)) // Check the range of allowed frequencies
  {
    FC = x;
    if (show)
      jprintf(" Set% s:% .2f [Hz] \ r \ n ", name, FC / 100.0);
    return err;
  } // set the frequency OK - display a message if necessary
  else
  {
    jprintf(" % s: Wrong frequency% .2f \ n ", name, x / 100.0);
    return WARNING_VALUE;
  }
#else                     // Battle option
  uint16_t hWord, lWord;
  uint8_t i;
  if ((!get_present()) || (GETBIT(flags, fErrFC)))
    return err;                   // exit if there is no inverter or it is blocked by mistake
  if ((x >= _min) && (x <= _max)) // Check the range of allowed frequencies
  {
#ifndef FC_ANALOG_CONTROL // Non-analog control
    // Write to the inverter registers the set frequency
    for (i = 0; i < FC_NUM_READ; i++) // Make FC_NUM_READ attempts
    {
      err = write_0x10_32(MX2_TARGET_FR, x);
      if (err == OK)
        break; // Command completed
      _delay(100);
      jprintf(" % s: repeat set frequency \ n ", name); // Display a message about the second command
    }

    if (err == OK)
    {
      FC = x;
      if (show)
        jprintf(" Set% s:% .2f [Hz] \ r \ n ", name, FC / 100.0);
      return err;
    } // set the frequency OK - display a message if necessary
    else
    {
      err = ERR_LINK_FC;
      SETBIT1(flags, fErrFC);
      set_Error(err, name);
      return err;
    } // error generation
    // Check for frequency setting and inverter adequacy
    if (x != read_0x03_32(MX2_TARGET_FR))
    {
      err = ERR_FC_ERROR;
      SETBIT1(flags, fErrFC);
      set_Error(err, name);
      return err;
    }
#else                     // Analog Control
    FC = x;
    dac = ((level100 - level0) * FC - 0 * level100) / (100 - 0);
    switch (testMode) // REAL Actions depending on the mode
    {
    case NORMAL:
      analogWrite(pin, dac);
      break; //   Operation mode is not a test, we turn everything on
    case SAFE_TEST:
      break; //   Do not include anything
    case TEST:
      break; //   Turn on everything except the compressor
    case HARD_TEST:
      analogWrite(pin, dac);
      break; //   Turn on everything and the compressor too
    }
    if (show)
      jprintf(" Set% s:% .2f [Hz] \ r \ n ", name, FC / 100.0); // frequency setting OK - display a message if necessary
#endif
    return err;
  } // if ((x> = _ min) && (x <= _ max))
  else
  {
    jprintf(" % s: Wrong frequency% .2f \ n ", name, x / 100.0);
    return WARNING_VALUE;
  }
#endif // DEMO
}

// Set DAC readings corresponding to 0 power
int8_t devOmronMX2::set_level0(int16_t x)
{
  if ((x >= 0) && (x <= 4096))
  {
    level0 = x;
    return OK;
  } // Only valid values
  return WARNING_VALUE;
}
// Set DAC readings corresponding to 100 power
int8_t devOmronMX2::set_level100(int16_t x)
{
  if ((x >= 0) && (x <= 4096))
  {
    level100 = x;
    return OK;
  } // Only valid values
  return WARNING_VALUE;
}
// Set the minimum power at which the chastotnik turns off (minimum power limit)
int8_t devOmronMX2::set_levelOff(int16_t x)
{
  if ((x >= 0) && (x <= 100))
  {
    levelOff = x;
    return OK;
  } // Only valid values
  return WARNING_VALUE;
}

// Set the ban on the use of the inverter if the error limit is reached
void devOmronMX2::check_blockFC(){
#ifndef FC_ANALOG_CONTROL // Non-analog control
/*
  if ((xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) && (err ! = OK)) // if free rtos is not running then block it the first time
  {
    SETBIT1(flags, fErrFC); // Set flag
    note = (char *)noteFC_NO;
    set_Error(err, (char *)name); // Raise the error to the top and stop the VT
    return statement;
  }

  if (err ! = OK)
    number_err++;
  else
  {
    number_err = 0;
    return;
  }                             // Increase the error counter
  if (number_err > FC_NUM_READ) // if the number of errors is increased, then the lock
  {
    SemaphoreGive(xModbusSemaphore); // unlock semaphore
    SETBIT1(flags, fErrFC);          // Set flag
    note = (char *)noteFC_NO;
    set_Error(err, (char *)name); // Raise the error to the top and stop the VT
  }
  */
#endif
}

// Read (internal variables are updated) the state of the Inverter, returns either OK or an error
// The period FC_TIME_READ is called from the task of reading sensors
int8_t devOmronMX2::get_readState()
{
  /*
  uint8_t i;
  if ((!get_present()) || (GETBIT(flags, fErrFC)))
    return err; // exit if there is no inverter or it is blocked by mistake
  err = OK;
#ifndef FC_ANALOG_CONTROL // Non-analog control
  // Read the state of the inverter, if an error occurs, generate a common VT error and stop
  for (i = 0; i < FC_NUM_READ; i++) // do FC_NUM_READ attempts to read
  {
    state = read_0x03_16(MX2_STATE); // read the state
    err = Modbus.get_err();          // Copy error
    if (err == OK)                   // Read correctly
    {
      if ((GETBIT(flags, fOnOff)) && (state ! = 3))
        continue;
      else
        break; // VT turned on the compressor and the inverter does not have the correct state, we try to read one more time in the opposite case everything is ok
    }
    _delay(FC_DELAY_REPEAT);
//    jprintf(cErrorRS485, name, __FUNCTION__, err); // Display a message about re-reading
    numErr++;                                                // number of errors reading by modbass
                                                             //        jprintf_time (cErrorRS485, name, err); // Output the error code to the log
  }
  if (err ! = OK) // Modbass error
  {
    state = ERR_LINK_FC;    // sign of loss of communication with the inverter
    SETBIT1(flags, fErrFC); // Inverter block
    set_Error(err, name);   // error generation
    return err;             // Return
  }
  //   else if ((testMode == NORMAL) || (testMode == HARD_TEST)) // Operation mode and hard test, analyze the state,
  //         if ((GETBIT (flags, fOnOff)) && (state! = 3)) // Invalid state
  //          {
  //          err = ERR_MODBUS_STATE; // Error Invalid Inverter Status
  //          jprintf ("% s: Compressor ON and wrong read state:% d \ n", name, state);
  //          set_Error (err, name);
  //          return err; // Return
  //          }
  // Read the state and it’s correct; we read everything else
  _delay(FC_DELAY_READ);
  freqFC = read_0x03_32(MX2_CURRENT_FR); // read the current frequency
  err = Modbus.get_err();                // Copy error
  if (err ! = OK)
  {
    state = ERR_LINK_FC;
  } // We exit the error

  _delay(FC_DELAY_READ);
  power = read_0x03_16(MX2_POWER); // read power
  err = Modbus.get_err();          // Copy error
  if (err ! = OK)
  {
    state = ERR_LINK_FC;
  } // We exit the error

  _delay(FC_DELAY_READ);
  current = read_0x03_16(MX2_AMPERAGE); // read current
  err = Modbus.get_err();               // Copy error
  if (err ! = OK)
  {
    state = ERR_LINK_FC;
  }   // We exit the error
#else // Analog Control
  freqFC = FC;
  power = 0;
  current = 0;
#endif
  return err;
  */
}

// Run command to the inverter (target frequency DOES NOT SET)
// A command can be issued through the relay and through modbas depending on the compilation keys
int8_t devOmronMX2::start_FC()
{
  /*
  if (((testMode == NORMAL) || (testMode == HARD_TEST)) && (((FC < _data.minFreq) || (FC > _data.maxFreq))))
  {
    jprintf(" % s: Wrong frequency, ignore start \ n ", name);
    return err;
  } // checking the frequency is not in test mode
  err = OK;
#ifndef FC_ANALOG_CONTROL // Non-analog control
#ifdef DEMO

#else               // DEMO
  // Warhead
  if (((testMode == NORMAL) || (testMode == HARD_TEST)) && (((!get_present()) || (GETBIT(flags, fErrFC)))))
    return err; // exit if there is no inverter or it is blocked by mistake

  // set_target (startFreq, true); // Writing the start frequency to the inverter register is NOT always the start frequency - superboiler

  err = OK;
  if ((testMode == NORMAL) || (testMode == HARD_TEST)) //    Operation mode and hard test, turn everything on,
  {
#ifdef FC_USE_RCOMP // Use a separate wire for the run / stop command
    HP dRelay[RCOMP].set_ON(); // BAD through a global variable
#else               // give a run / stop command through modbas
    err = write_0x05_bit(MX2_START, true);  // Command Move
#endif
  }
  if (err == OK)
  {
    SETBIT1(flags, fOnOff);
    startCompressor = rtcSAM3X8.unixtime();
    jprintf(" % s ON \ n ", name);
  }
  else
  {
    state = ERR_LINK_FC;
    SETBIT1(flags, fErrFC);
    set_Error(err, name);
  } // error generation
#endif
#else //   FC_ANALOG_CONTROL
#ifdef DEMO
#ifdef FC_USE_RCOMP // Use a separate wire for the run / stop command
  HP dRelay[RCOMP].set_ON(); // BAD through a global variable
#endif              // FC_USE_RCOMP
  SETBIT1(flags, fOnOff);
  startCompressor = rtcSAM3X8.unixtime();
  jprintf(" % s ON \ n ", name);
#else               // DEMO \
                    // Warhead
  if (((testMode == NORMAL) || (testMode == HARD_TEST)) && (((!get_present()) || (GETBIT(flags, fErrFC)))))
    return err; // exit if there is no inverter or it is blocked by mistake
  err = OK;
  if ((testMode == NORMAL) || (testMode == HARD_TEST)) //    Operation mode and hard test, turn everything on,
  {
#ifdef FC_USE_RCOMP // Use a separate wire for the run / stop command
    HP dRelay[RCOMP].set_ON(); // BAD through a global variable
#else
    state = ERR_LINK_FC;
    err = ERR_FC_CONF_ANALOG;
    SETBIT1(flags, fErrFC);
    set_Error(err, name); // configuration error
#endif
  }
  SETBIT1(flags, fOnOff);
  startCompressor = rtcSAM3X8.unixtime();
  jprintf(" % s ON \ n ", name);
#endif
#endif
*/
  return err;
}

// Stop command to inverter Return error code
int8_t devOmronMX2::stop_FC()
{
  // uint8_t i;
  err = OK;
#ifndef FC_ANALOG_CONTROL // Non-analog control
#ifdef DEMO
#ifdef FC_USE_RCOMP           // Use a separate wire for the run / stop command
  HP dRelay[RCOMP].set_OFF(); // BAD through a global variable
#endif                        // FC_USE_RCOMP
  if (err == OK)
  {
    SETBIT0(flags, fOnOff);
    startCompressor = 0;
    jprintf(" % s OFF \ n ", name);
  }
  else
  {
    state = ERR_LINK_FC;
    SETBIT1(flags, fErrFC);
    set_Error(err, name);
  }                 // error generation
#else               // not DEMO
  if (!get_present())
    return err; // if there is no inverter, exit
  // if (((testMode == NORMAL) || (testMode == HARD_TEST)) && (((! get_present ()) || (GETBIT (flags, fErrFC))))) return err; // exit if not inverter or it is blocked by mistake
  err = OK;
  if ((testMode == NORMAL) || (testMode == HARD_TEST)) // Operation mode and hard test, turn everything on,
  {
#ifdef FC_USE_RCOMP // Use a separate wire for the run / stop command with verification of progress

#else // give a run / stop command through modbas
    err = write_0x05_bit(MX2_START, false); // Stop command
#endif
  }
  if (err == OK)
  {
    SETBIT0(flags, fOnOff);
    startCompressor = 0;
    jprintf(" % s OFF \ n ", name);
  }
  else
  {
    state = ERR_LINK_FC;
    SETBIT1(flags, fErrFC);
    set_Error(err, name);
  } // error generation
#endif
#else // FC_ANALOG_CONTROL
#ifdef DEMO
#ifdef FC_USE_RCOMP // Use a separate wire for the run / stop command
  HP dRelay[RCOMP].set_OFF(); // BAD through a global variable
#endif              // FC_USE_RCOMP
  SETBIT0(flags, fOnOff);
  startCompressor = 0;
  jprintf(" % s OFF \ n ", name);
#else               // not DEMO
  if (((testMode == NORMAL) || (testMode == HARD_TEST)) && (((!get_present()) || (GETBIT(flags, fErrFC)))))
    return err;                                        // exit if there is no inverter or it is blocked by mistake
  if ((testMode == NORMAL) || (testMode == HARD_TEST)) // Operation mode and hard test, turn everything on,
  {
#ifdef FC_USE_RCOMP // Use a separate wire for the run / stop command
    HP dRelay[RCOMP].set_OFF(); // BAD through a global variable
#else               // give a run / stop command through modbas
    state = ERR_LINK_FC;
    err = ERR_FC_CONF_ANALOG;
    SETBIT1(flags, fErrFC);
    set_Error(err, name); // configuration error
#endif
  }
  SETBIT0(flags, fOnOff);
  startCompressor = 0;
  jprintf(" % s OFF \ n ", name);
#endif
#endif // FC_ANALOG_CONTROL
  return err;
}

// Get the inverter parameter as a string, the result is ADDED to ret
void devOmronMX2::get_paramFC(char *var, char *ret)
{
  if (strcmp(var, fc_ON_OFF) == 0)
  {
    if (GETBIT(flags, fOnOff))
      strcat(ret, (char *)cOne);
    else
      strcat(ret, (char *)cZero);
  }
  else if (strcmp(var, fc_INFO) == 0)
  {
#ifndef FC_ANALOG_CONTROL
    get_infoFC(ret);
#else
    strcat(ret, " | Data not available, work through analog input |; ");
#endif
  }
  else if (strcmp(var, fc_NAME) == 0)
  {
    strcat(ret, name);
  }
  else if (strcmp(var, fc_NOTE) == 0)
  {
    strcat(ret, note);
  }
  else if (strcmp(var, fc_PIN) == 0)
  {
    _itoa(pin, ret);
  }
  else if (strcmp(var, fc_PRESENT) == 0)
  {
    if (GETBIT(flags, fFC))
      strcat(ret, (char *)cOne);
    else
      strcat(ret, (char *)cZero);
  }
  else if (strcmp(var, fc_STATE) == 0)
  {
    _itoa(state, ret);
  }
  else if (strcmp(var, fc_FC) == 0)
  {
    _ftoa(ret, (float)FC / 100.0, 2);
  }
  else if (strcmp(var, fc_cFC) == 0)
  {
    _ftoa(ret, (float)freqFC / 100.0, 2);
  }
  else if (strcmp(var, fc_cPOWER) == 0)
  {
    _ftoa(ret, (float)power / 10.0, 1);
  }
  else if (strcmp(var, fc_INFO1) == 0)
  {
    _ftoa(ret, (float)power / 10.0, 1);
    strcat(ret, " kW ");
  }
  else if (strcmp(var, fc_cCURRENT) == 0)
  {
    _ftoa(ret, (float)current / 100.0, 2);
  }
  else if (strcmp(var, fc_AUTO_RESET_FAULT) == 0)
  {
    strcat(ret, (char *)(GETBIT(_data.setup_flags, fAutoResetFault) ? cOne : cZero));
  }
  else if (strcmp(var, fc_LogWork) == 0)
  {
    strcat(ret, (char *)(GETBIT(_data.setup_flags, fLogWork) ? cOne : cZero));
  }
  else if (strcmp(var, fc_ANALOG) == 0)
  { // Analog control flag
#ifdef FC_ANALOG_CONTROL
    strcat(ret, (char *)cOne);
#else
    strcat(ret, (char *)cZero);
#endif
  }
  else if (strcmp(var, fc_DAC) == 0)
  {
    _itoa(dac, ret);
  }
  else
#ifdef FC_ANALOG_CONTROL

#endif
      if (strcmp(var, fc_BLOCK) == 0)
  {
    if (GETBIT(flags, fErrFC))
      strcat(ret, (char *)cOne);
    else
      strcat(ret, (char *)cZero);
  }
  else if (strcmp(var, fc_ERROR) == 0)
  {
    _itoa(err, ret);
  }
  else if (strcmp(var, fc_UPTIME) == 0)
  {
    _itoa(_data.Uptime, ret);
  }
  else // output in seconds
      if (strcmp(var, fc_PID_STOP) == 0)
  {
    _itoa(_data.PidStop, ret);
  }
  else if (strcmp(var, fc_DT_COMP_TEMP) == 0)
  {
    _ftoa(ret, (float)_data.dtCompTemp / 100.0, 2);
  }
  else // degrees
      if (strcmp(var, fc_PID_FREQ_STEP) == 0)
  {
    _ftoa(ret, (float)_data.PidFreqStep / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_START_FREQ) == 0)
  {
    _ftoa(ret, (float)_data.startFreq / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_START_FREQ_BOILER) == 0)
  {
    _ftoa(ret, (float)_data.startFreqBoiler / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_MIN_FREQ) == 0)
  {
    _ftoa(ret, (float)_data.minFreq / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_MIN_FREQ_COOL) == 0)
  {
    _ftoa(ret, (float)_data.minFreqCool / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_MIN_FREQ_BOILER) == 0)
  {
    _ftoa(ret, (float)_data.minFreqBoiler / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_MIN_FREQ_USER) == 0)
  {
    _ftoa(ret, (float)_data.minFreqUser / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_MAX_FREQ) == 0)
  {
    _ftoa(ret, (float)_data.maxFreq / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_MAX_FREQ_COOL) == 0)
  {
    _ftoa(ret, (float)_data.maxFreqCool / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_MAX_FREQ_BOILER) == 0)
  {
    _ftoa(ret, (float)_data.maxFreqBoiler / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_MAX_FREQ_USER) == 0)
  {
    _ftoa(ret, (float)_data.maxFreqUser / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_STEP_FREQ) == 0)
  {
    _ftoa(ret, (float)_data.stepFreq / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_STEP_FREQ_BOILER) == 0)
  {
    _ftoa(ret, (float)_data.stepFreqBoiler / 100.0, 2);
  }
  else // Hz
      if (strcmp(var, fc_DT_TEMP) == 0)
  {
    _ftoa(ret, (float)_data.dtTemp / 100.0, 2);
  }
  else // degrees
      if (strcmp(var, fc_DT_TEMP_BOILER) == 0)
  {
    _ftoa(ret, (float)_data.dtTempBoiler / 100.0, 2);
  }
  else // degrees
      if (strcmp(var, fc_MB_ERR) == 0)
  {
    _itoa(numErr, ret);
  }
  else if (strcmp(var, fc_FC_TIME_READ) == 0)
  {
    _itoa(FC_TIME_READ, ret);
  }
  else
    strcat(ret, (char *)cInvalid);
}

// Set the inverter parameter from the string
boolean devOmronMX2::set_paramFC(char *var, float x)
{
  if (strcmp(var, fc_ON_OFF) == 0)
  {
    if (x == 0)
      stop_FC();
    else
      start_FC();
    return true;
  }
  else if (strcmp(var, fc_FC) == 0)
  {
    if ((x * 100 >= _data.minFreqUser) && (x * 100 <= _data.maxFreqUser))
    {
      set_target(x * 100, true, _data.minFreqUser, _data.maxFreqUser);
      return true;
    }
    else
      return false;
  }
  else if (strcmp(var, fc_AUTO_RESET_FAULT) == 0)
  {
    if (x == 0)
      SETBIT0(_data.setup_flags, fAutoResetFault);
    else
      SETBIT1(_data.setup_flags, fAutoResetFault);
    return true;
  }
  else if (strcmp(var, fc_LogWork) == 0)
  {
    _data.setup_flags = (_data.setup_flags & ~(1 << fLogWork)) | ((x != 0) << fLogWork);
    return true;
  }
  else

#ifdef FC_ANALOG_CONTROL

#endif
      if (strcmp(var, fc_BLOCK) == 0)
  {
    SemaphoreGive(xModbusSemaphore); // give the semaphore ALWAYS
    if (x == 0)
    {
      SETBIT0(flags, fErrFC);
      note = (char *)noteFC_OK;
    }
    else
    {
      SETBIT1(flags, fErrFC);
      note = (char *)noteFC_NO;
    }
    return true;
  }
  else if (strcmp(var, fc_UPTIME) == 0)
  {
    if ((x >= 3) && (x < 600))
    {
      _data.Uptime = x;
      return true;
    }
    else
      return false;
  }
  else // storage in sec
      if (strcmp(var, fc_PID_STOP) == 0)
  {
    if ((x >= 50) && (x <= 100))
    {
      _data.PidStop = x;
      return true;
    }
    else
      return false;
  }
  else // % of the target
      if (strcmp(var, fc_DT_COMP_TEMP) == 0)
  {
    if ((x >= 1) && (x <= 25))
    {
      _data.dtCompTemp = x * 100;
      return true;
    }
    else
      return false;
  }
  else // degrees

      if (strcmp(var, fc_PID_FREQ_STEP) == 0)
  {
    if ((x > 0) && (x <= 5))
    {
      _data.PidFreqStep = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_START_FREQ) == 0)
  {
    if ((x >= 20) && (x <= 120))
    {
      _data.startFreq = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_START_FREQ_BOILER) == 0)
  {
    if ((x >= 20) && (x <= 150))
    {
      _data.startFreqBoiler = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_MIN_FREQ) == 0)
  {
    if ((x >= 20) && (x <= 80))
    {
      _data.minFreq = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_MIN_FREQ_COOL) == 0)
  {
    if ((x >= 20) && (x <= 80))
    {
      _data.minFreqCool = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_MIN_FREQ_BOILER) == 0)
  {
    if ((x >= 20) && (x <= 80))
    {
      _data.minFreqBoiler = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_MIN_FREQ_USER) == 0)
  {
    if ((x >= 20) && (x <= 80))
    {
      _data.minFreqUser = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_MAX_FREQ) == 0)
  {
    if ((x >= 40) && (x <= 240))
    {
      _data.maxFreq = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_MAX_FREQ_COOL) == 0)
  {
    if ((x >= 40) && (x <= 240))
    {
      _data.maxFreqCool = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_MAX_FREQ_BOILER) == 0)
  {
    if ((x >= 40) && (x <= 240))
    {
      _data.maxFreqBoiler = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_MAX_FREQ_USER) == 0)
  {
    if ((x >= 40) && (x <= 240))
    {
      _data.maxFreqUser = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_STEP_FREQ) == 0)
  {
    if ((x >= 0.2) && (x <= 10))
    {
      _data.stepFreq = x * 100;
      return true;
    }
    else
      return false;
  }
  else // Hz
      if (strcmp(var, fc_STEP_FREQ_BOILER) == 0)
  {
    if ((x >= 0.2) && (x <= 10))
    {
      _data.stepFreqBoiler = x * 100;
      return true;
    }
    else
      return false;
  } // Hz

  if (strcmp(var, fc_DT_TEMP) == 0)
  {
    if ((x > 0) && (x < 10))
    {
      _data.dtTemp = x * 100;
      return true;
    }
    else
      return false;
  }
  else // degrees
      if (strcmp(var, fc_DT_TEMP_BOILER) == 0)
  {
    if ((x > 0) && (x < 10))
    {
      _data.dtTempBoiler = x * 100;
      return true;
    }
    else
      return false;
  }
  else // degrees
    return false;
}

// Get information about the chastotnik, information is added to buf
char *devOmronMX2::get_infoFC(char *buf)
{
  // #ifndef FC_ANALOG_CONTROL // NOT ANALOGUE MANAGEMENT
  /*
  if (!HP.dFC.get_present())
  {
    strcat(buf, " | Data not available (no inverter) |; ");
    return buf;
  } // The inverter is not in the configuration
  if (HP.dFC.get_blockFC())
  {
    strcat(buf, " | Data not available (no Modbus communication, inverter locked) |; ");
    return buf;
  } // Inverter locked
  int8_t i;
  strcat(buf, " - | Inverter state [0: Initial state, 2: Stop 3: Turn 4: Coast stop 5: Jog 6: DC braking ");
  strcat(buf, " 7: Retrying 8: Failing 9: Undervoltage -1: Blocking] | ");
  _itoa(read_0x03_16(MX2_STATE), buf);
  strcat(buf, " ; ");
  _delay(FC_DELAY_READ);
  strcat(buf, " d001 | Output frequency control (Hz) | ");
  _ftoa(buf, (float)read_0x03_32(MX2_CURRENT_FR) / 100.0, 2);
  strcat(buf, " ; ");
  _delay(FC_DELAY_READ);
  strcat(buf, " d003 | Output current control (A) | ");
  _ftoa(buf, (float)read_0x03_16(MX2_AMPERAGE) / 100.0, 2);
  strcat(buf, " ; ");
  _delay(FC_DELAY_READ);
  strcat(buf, " d014 | Power control (kW) | ");
  _ftoa(buf, (float)read_0x03_16(MX2_POWER) / 10.0, 1);
  strcat(buf, " ; ");
  _delay(FC_DELAY_READ);
  strcat(buf, " d013 | Monitoring the output voltage (V) | ");
  _ftoa(buf, (float)read_0x03_16(MX2_VOLTAGE) / 10.0, 1);
  strcat(buf, " ; ");
  _delay(FC_DELAY_READ);
  strcat(buf, " d015 | Watt-hour control (kW / h) | ");
  _ftoa(buf, (float)read_0x03_32(MX2_POWER_HOUR) / 10.0, 1);
  strcat(buf, " ; ");
  _delay(FC_DELAY_READ);
       strcat (buf, " d016 | Runtime monitoring in \" Run \ " mode (h) | " ); _itoa ( read_0x03_32 (MX2_HOUR), buf); strcat (buf, " ; " );
       _delay (FC_DELAY_READ);
       strcat (buf, " d017 | Runtime monitoring with power on (h) | " ); _itoa ( read_0x03_32 (MX2_HOUR1), buf); strcat (buf, " ; " );
       _delay (FC_DELAY_READ);
       strcat (buf, " d018 | Radiator temperature control (° C) | " ); _ftoa (buf, ( float ) read_0x03_16 (MX2_TEMP) / 10.0 , 2 ); strcat (buf, " ; " );
       _delay (FC_DELAY_READ);
       strcat (buf, " d102 | DC voltage control (V) | " ); _ftoa (buf, ( float ) read_0x03_16 (MX2_VOLTAGE_DC) / 10.0 , 1 ); strcat (buf, " ; " );
       _delay (FC_DELAY_READ);
       strcat (buf, " d080 | Counter of emergency shutdowns (pcs) | " ); _itoa ( read_0x03_16 (MX2_NUM_ERR), buf); strcat (buf, " ; " );
       for (i = 0 ; i < 6 ; i ++)   // Scan By Errors
          {
    strcat(buf, " d0 ");
    _itoa(81 + i, buf);
    strcat(buf, " | State at the time of the error ");
    read_0x03_error(MX2_ERROR1 + i * 0x0a);
    // Formation of the answer in line
    strcat(buf, " [F: ");
    _ftoa(buf, (float)error.MX2.fr / 100.0, 2);
    strcat(buf, " I: ");
    _ftoa(buf, (float)error.MX2.cur / 100.0, 2);
    strcat(buf, " V: ");
    _ftoa(buf, (float)error.MX2.vol / 10.0, 2);
    strcat(buf, " T1: ");
    _itoa(error.MX2.time1, buf);
    strcat(buf, " T2: ");
    _itoa(error.MX2.time2, buf);
    strcat(buf, " ] Error code: | ");
    if (error.MX2.code < 10)
      strcat(buf, " E0 ");
    else
      strcat(buf, " E ");
    _itoa(error.MX2.code, buf);
    strcat(buf, " . ");
    _itoa(error.MX2.status, buf);
    strcat(buf, " ; ");   
          }
#endif          
return buf;
*/
}
// Reset inverter errors by modbass
boolean devOmronMX2 ::reset_errorFC()
{
#ifndef FC_ANALOG_CONTROL            // NOT ANALOGUE MANAGEMENT
  write_0x06_16(MX2_INIT_DEF, 0x01); // set the initialization mode - erasing errors
  _delay(FC_DELAY_READ);
  if ((read_0x03_16(MX2_INIT_DEF) == 0x01) && (err = OK)) // issue an initialization command if only errors have been erased
  {
    write_0x06_16(MX2_INIT_RUN, 0x01);
    //  jprintf(" Reset error% s \ r \ n ", name);
  }
  else
  // jprintf(" $ WARNING: bad read from MX2_INIT_DEF, no reset error \ r \ n ");
#endif
      if (err == OK)
    return true;
  else
    return false;
}

// Reset the inverter via modbas
boolean devOmronMX2::reset_FC()
{
#ifndef FC_ANALOG_CONTROL          // NOT ANALOGUE MANAGEMENT
  write_0x05_bit(MX2_RESET, true); // send a command to reset by modbas
//   jprintf ("Reset% s use Modbus \ r \ n", name);
#endif
  if (err == OK)
    return true;
  else
    return false;
}

// Current state of the inverter
//
int16_t devOmronMX2::read_stateFC()
{
#ifndef FC_ANALOG_CONTROL          // NOT ANALOGUE MANAGEMENT
  state = read_0x03_16(MX2_STATE); // read the state
  if (GETBIT(_data.setup_flags, fLogWork) && GETBIT(flags, fOnOff))
  {
    // jprintf_time(" FC:% Xh,% .2fHz,% .2fA,% .2fkW \ n ", state, (float)freqFC / 100.0, (float)current / 100.0, (float)get_power() / 1000.0);
  }
  return state;
#else
  return 0;
#endif
}

// Radiator temperature
int16_t devOmronMX2::read_tempFC()
{
#ifndef FC_ANALOG_CONTROL // NOT ANALOGUE MANAGEMENT
  return read_0x03_16(MX2_TEMP);
#else
  return 0;
#endif
}
// Communication functions on the inverter modbas Read registers
#ifndef FC_ANALOG_CONTROL // NOT ANALOGUE MANAGEMENT
// Reading a single bit into the cmd register returns a code, the error is updated
// Implemented FC_NUM_READ read / write attempts to the inverter
boolean devOmronMX2 ::read_0x01_bit(uint16_t cmd)
{
  uint8_t i;
  boolean result;
  err = OK;
  if ((!get_present()) || (GETBIT(flags, fErrFC)))
    return false;                   // exit if there is no inverter or it is blocked by mistake
  for (i = 0; i < FC_NUM_READ; i++) // make FC_NUM_READ attempts to read Read the inverter status, if an error occurs, generate a common VT error and stop
  {
    err = Modbus.readCoil(FC_MODBUS_ADR, cmd - 1, &result); // send a request, Numbering of registers MX2 from ZERO !!!!
    if (err == OK)
      break; // Read successfully
    _delay(FC_DELAY_REPEAT);
    //     jprintf(cErrorRS485, name, __ FUNCTION __, err); // Display a message about re-reading
    numErr++; // number of errors reading by modbass
              //         jprintf_time (cErrorRS485, name, err); // Output the error code to the log
  }

  check_blockFC(); // check for blocking
  return result;
}
// Function 0x03 read 2 bytes, returns a value, the error is updated
// Implemented FC_NUM_READ read / write attempts to the inverter
int16_t devOmronMX2::read_0x03_16(uint16_t cmd)
{
  uint8_t i;
  uint16_t result;
  err = OK;
  if ((!get_present()) || (GETBIT(flags, fErrFC)))
    return 0; // exit if there is no inverter or it is blocked by mistake

  for (i = 0; i < FC_NUM_READ; i++) // make FC_NUM_READ attempts to read Read the inverter status, if an error occurs, generate a common VT error and stop
  {
    err = Modbus.readHoldingRegisters16(FC_MODBUS_ADR, cmd - 1, &result); // Send a request, MX2 register numbering from ZERO !!!!
    if (err == OK)
      break; // Read successfully
    _delay(FC_DELAY_REPEAT);
    // jprintf(cErrorRS485, name, __FUNCTION__, err); // Display a message about re-reading
    numErr++; // number of errors reading by modbass
              //          jprintf_time (cErrorRS485, name, err); // Output the error code to the log
  }
  check_blockFC(); // check for blocking
  return result;
}

// Function 0x03 read 4 bytes
// Implemented FC_NUM_READ read / write attempts to the inverter
uint32_t devOmronMX2::read_0x03_32(uint16_t cmd)
{
  uint8_t i;
  uint32_t result;
  err = OK;
  if ((!get_present()) || (GETBIT(flags, fErrFC)))
    return 0;                       // exit if there is no inverter or it is blocked by mistake
  for (i = 0; i < FC_NUM_READ; i++) // make FC_NUM_READ attempts to read Read the inverter status, if an error occurs, generate a common VT error and stop
  {
    err = Modbus.readHoldingRegisters32(FC_MODBUS_ADR, cmd - 1, &result); // send a request, Numbering of registers MX2 from ZERO !!!!
    if (err == OK)
      break; // Read successfully
    _delay(FC_DELAY_REPEAT);
    // jprintf(cErrorRS485, name, __ FUNCTION __, err); // Display a message about re-reading
    numErr++; // number of errors reading by modbass
              //        jprintf_time (cErrorRS485, name, err); // Output the error code to the log
  }
  check_blockFC(); // check for blocking
  return result;
}

// Modbus function 0x03 error description num NUMBERING from 0 (total data length 10 words, 2 bytes each)
// Returns the error code and puts a description in the buffer
// Implemented FC_NUM_READ read / write attempts to the inverter
int16_t devOmronMX2::read_0x03_error(uint16_t cmd)
{
  uint8_t i;
  uint16_t tmp;
  err = OK;
  if ((!get_present()) || (GETBIT(flags, fErrFC)))
    return err; // exit if there is no inverter or it is blocked by mistake
  for (i = 0; i < 0x0a; i++)
    error.inputBuf[i] = 0;
  for (i = 0; i < FC_NUM_READ; i++) // make FC_NUM_READ attempts to read Read the inverter status, if an error occurs, generate a common VT error and stop
  {
    err = Modbus.readHoldingRegistersNN(FC_MODBUS_ADR, cmd - 1, 0x0a, error.inputBuf); // send a request, Numbering of registers from ZERO !!!!
    if (err == OK)
      break; // Read successfully
    _delay(FC_DELAY_REPEAT);
    // jprintf(cErrorRS485, name, __ FUNCTION __, err); // Display a message about re-reading
    numErr++; // number of errors reading by modbass
              //         jprintf_time (cErrorRS485, name, err); // Output the error code to the log
  }
  if (err == OK) // For times, rearrange the words (2 bytes), i.e., the leading 2 bytes then the lower
  {
    tmp = error.inputBuf[6];
    error.inputBuf[6] = error.inputBuf[7];
    error.inputBuf[7] = tmp; // Total operating time in the "Run" mode to the time of shutdown
    tmp = error.inputBuf[8];
    error.inputBuf[8] = error.inputBuf[9];
    error.inputBuf[9] = tmp; // The total operating time of the inverter with the power on at the time the output is turned off
  }
  check_blockFC(); // check for blocking
  return err;
}

// Writing a single bit to the cmd register returns an error code
// Implemented FC_NUM_READ read / write attempts to the inverter
int8_t devOmronMX2::write_0x05_bit(uint16_t cmd, boolean f)
{
  uint8_t i;
  err = OK;
  if ((!get_present()) || (GETBIT(flags, fErrFC)))
    return err;                     // exit if there is no inverter or it is blocked by mistake
  for (i = 0; i < FC_NUM_READ; i++) // make FC_NUM_READ write attempts
  {
    if (f)
      err = Modbus.writeSingleCoil(FC_MODBUS_ADR, cmd - 1, 1); // send a request, Numbering of registers from ZERO !!!!
    else
      err = Modbus.writeSingleCoil(FC_MODBUS_ADR, cmd - 1, 0);
    if (err == OK)
      break; // Written successfully
    _delay(FC_DELAY_REPEAT);
    // jprintf(cErrorRS485, name, __ FUNCTION __, err); // Display a message about re-reading
    numErr++; // number of errors reading by modbass
              //          jprintf_time (cErrorRS485, name, err); // Output the error code to the log
  }
  check_blockFC(); // check for blocking
  return err;
}
// Writing data (2 bytes) to the cmd register returns an error code
// Implemented FC_NUM_READ read / write attempts to the inverter
int8_t devOmronMX2::write_0x06_16(uint16_t cmd, uint16_t data)
{
  uint8_t i;
  err = OK;
  if ((!get_present()) || (GETBIT(flags, fErrFC)))
    return err;                     // exit if there is no inverter or it is blocked by mistake
  for (i = 0; i < FC_NUM_READ; i++) // make FC_NUM_READ write attempts
  {
    err = Modbus.writeHoldingRegisters16(FC_MODBUS_ADR, cmd - 1, data); // send a request, Numbering of registers from ZERO !!!!
    if (err == OK)
      break; // Written successfully
    _delay(FC_DELAY_REPEAT);
    // jprintf(cErrorRS485, name, __ FUNCTION __, err); // Display a message about re-reading
    numErr++; // number of errors reading by modbass
              //         jprintf_time (cErrorRS485, name, err); // Output the error code to the log
  }
  check_blockFC(); // check for blocking
  return err;
}
// Writing data (4 bytes) to the cmd register returns an error code
int8_t devOmronMX2 ::write_0x10_32(uint16_t cmd, uint32_t data)
{
  uint8_t i;
  err = OK;
  if ((!get_present()) || (GETBIT(flags, fErrFC)))
    return err;                     // exit if there is no inverter or it is blocked by mistake
  for (i = 0; i < FC_NUM_READ; i++) // make FC_NUM_READ write attempts
  {
    err = Modbus.writeHoldingRegisters32(FC_MODBUS_ADR, cmd - 1, data); // send a request, Numbering of registers from ZERO !!!!
    if (err == OK)
      break; // Written successfully
    _delay(FC_DELAY_REPEAT);
    // jprintf(cErrorRS485, name, __ FUNCTION __, err); // Display a message about re-reading
    numErr++; // number of errors reading by modbass
              //          jprintf_time (cErrorRS485, name, err); // Output the error code to the log
  }
  check_blockFC(); // check for blocking
  return err;
}
#endif // FC_ANALOG_CONTROL // NOT ANALOGUE MANAGEMENT