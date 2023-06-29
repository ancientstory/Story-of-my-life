#ifndef _CAN_motor_H_
#define _CAN_motor_H_

#include <stdint.h>

#define CHASSIS_CAN hcan1
#define GIMBAL_CAN hcan2

#define MOTOR_3508_CURRENT_MAX ((int16_t)16384)
#define MOTOR_3508_CURRENT_MIN ((int16_t)-16384)

/* CAN send and receive ID */
typedef enum
{
  CAN_CHASSIS_ALL_ID = 0x200,
  CAN_3508_M1_ID = 0x201,
  CAN_3508_M2_ID = 0x202,
  CAN_3508_M3_ID = 0x203,
  CAN_3508_M4_ID = 0x204,

  CAN_GIMBAL_ALL_ID = 0x1FF,
  CAN_YAW_MOTOR_ID = 0x205,
  CAN_PITCH_MOTOR_ID = 0x206,
  CAN_TRIGGER_MOTOR_ID = 0x207,

} CAN_Message_Ids;

// rm motor data
typedef struct
{
  uint16_t angle;
  int16_t speed_rpm;
  int16_t given_current;
  uint8_t temperate;
  int16_t last_angle;
} motor_measure_data;

void CAN_gimbal_tx_message(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev);
void CAN_chassis_tx_message(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
extern motor_measure_data motor_data[10];
#endif
