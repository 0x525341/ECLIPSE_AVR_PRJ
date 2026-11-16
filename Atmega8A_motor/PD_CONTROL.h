/* Заголовочный файл */
#ifndef PD_CONTROL_H_
#define PD_CONTROL_H_
#include <stdint.h> //uint
#define MAX_INT         INT16_MAX

/** Структура данных ПД-регулятора.  Она содержит настройки
(коэффициент усиления, интегральная часть, а также конечный делитель),
выходные пределы, и аккумулятор дифференциальной части. */
struct PDControl
{
   uint8_t kp;    /**Коэф-т усиления*/
   uint8_t kd;    /** дифференциальнаф составляющая*/
   int16_t last_process_value; /** предыдущее значение ошибки */
   int16_t max_e;
};

int  PD_control (struct PDControl *p, int16_t Set_value, int16_t Process_value);
void PD_control_reset (struct PDControl *p);
void PD_control_init  (struct PDControl *p, uint16_t PD_control_Kp, uint16_t PD_control_Kd);
void PD_control_Kp_Kd (struct PDControl *p, uint16_t PD_control_Kp, uint16_t PD_control_Kd);

#endif /* PD_CONTROL_H_ */
