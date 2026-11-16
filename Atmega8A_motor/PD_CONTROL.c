/**
  * Пропорционально-дифференциальный (ПД) регулятор.
  *
  * Этот модуль реализует простой ПД-регулятор:
  * 
  *   u = [ kp * e(t) + kd * (e(t)-е(t-1)) ]
  * 
  *
  * Предотвращение насыщения:
  * - Высчитывается новое выходное значение.
  * - Если оно превышает предел, и последнее изменение постоянной времени
  *   интегратора имеет то же направление (на увеличение или на уменьшение),
  *   то новое значение постоянной времени не сохраняется.
  * - Иначе, это значение сохраняется.
  */
//-------------------------------------------------------
#include "PD_CONTROL.h"
//-------------------------------------------------------
int16_t PD_control (struct PDControl *p, int16_t Set_value, int16_t Process_value)
{
   int16_t E;           /*ошибка*/
   int16_t P;
   int16_t D;
   int32_t U;           /* выходная величина */

   E = Set_value-Process_value;

   /* Считаем новую дифференцильную составляющую и выходную величину */
   if (p->kp==0)
   {
	   P=0;
   }
   else if(E>p->max_e)
   {
	   P=MAX_INT;
   }
   else if(E<-p->max_e)
   {
	   P=-MAX_INT;
   }
   else
   {
	   P=(p->kp * E);
   }

   if (p->kd==0)
   {
	   D=0;
   }
   else
   {
	   D=p->kd * (p->last_process_value-Process_value);
	   p->last_process_value=Process_value;
   }

   U = (P+D);

/* Проверяем на насыщение. */
   /* Оно положительное? */
   if(U > MAX_INT)
   {
     U = MAX_INT;
   }

/* То же самое для отрицательного значения */
   else if(U < -MAX_INT)
   {
     U = -MAX_INT;
   }
   return ((int16_t)U);
}

/**
  * Инициализация ПД-регулятора.
  *
  * Эта функция сбрасывает ПД-регулятор в ноль..
  *
  */
void PD_control_reset (struct PDControl *p)
{
   p->last_process_value = 0;
}

void PD_control_init  (struct PDControl *p, uint16_t PD_control_Kp, uint16_t PD_control_Kd)
{
   p->kp = PD_control_Kp;
   p->kd = PD_control_Kd;
   p->last_process_value = 0;
   p->max_e = MAX_INT / (p->kp + 1);
}

void PD_control_Kp_Kd  (struct PDControl *p, uint16_t PD_control_Kp, uint16_t PD_control_Kd)
{
   p->kp = PD_control_Kp;
   p->kd = PD_control_Kd;
}
