#include "CAN_motor.h"
#include "can.h"

static CAN_TxHeaderTypeDef gimbalTxHeader, chassisTxHeader;
static uint8_t gimbalTxData[8], chassisTxData[8];
motor_measure_data motor_data[10];
CAN_RxHeaderTypeDef rxHeader;
// Callback function:receive measure data from motor
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rx_data);
    switch (rxHeader.StdId)
    {
    case CAN_3508_M1_ID:
    case CAN_3508_M2_ID:
    case CAN_3508_M3_ID:
    case CAN_3508_M4_ID:
    case CAN_PITCH_MOTOR_ID:
    case CAN_YAW_MOTOR_ID:
    case CAN_TRIGGER_MOTOR_ID:
    {
        static uint8_t i;
        i = rxHeader.StdId - CAN_3508_M1_ID;

        motor_data[i].last_angle = motor_data[i].angle;
        motor_data[i].angle = (uint16_t)(rx_data[0] << 8 | rx_data[1]);
        motor_data[i].speed_rpm = (uint16_t)(rx_data[2] << 8 | rx_data[3]);
        motor_data[i].given_current = (uint16_t)(rx_data[4] << 8 | rx_data[5]);
        motor_data[i].temperate = (uint8_t)rx_data[6];
        break;
    }
    default:
    {
        break;
    }
    }
}

void CAN_chassis_tx_message(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t txMailbox;
    chassisTxHeader.IDE = CAN_ID_STD;
    chassisTxHeader.StdId = CAN_CHASSIS_ALL_ID;
    chassisTxHeader.RTR = CAN_RTR_DATA;
    chassisTxHeader.DLC = 0x08;

    chassisTxData[0] = (motor1 >> 8);
    chassisTxData[1] = motor1;
    chassisTxData[2] = (motor2 >> 8);
    chassisTxData[3] = motor2;
    chassisTxData[4] = (motor3 >> 8);
    chassisTxData[5] = motor3;
    chassisTxData[6] = (motor4 >> 8);
    chassisTxData[7] = motor4;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassisTxHeader, chassisTxData, &txMailbox);
}

void CAN_gimbal_tx_message(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev)
{
    uint32_t txMailbox; // temporary variable,to store returned mailbox number
    gimbalTxHeader.IDE = CAN_ID_STD;
    gimbalTxHeader.StdId = CAN_GIMBAL_ALL_ID;
    gimbalTxHeader.RTR = CAN_RTR_DATA;
    gimbalTxHeader.DLC = 0x08;

    gimbalTxData[0] = (yaw >> 8);
    gimbalTxData[1] = yaw;
    gimbalTxData[2] = (pitch >> 8);
    gimbalTxData[3] = pitch;
    gimbalTxData[4] = (shoot >> 8);
    gimbalTxData[5] = shoot;
    gimbalTxData[6] = (rev >> 8);
    gimbalTxData[7] = rev;

    HAL_CAN_AddTxMessage(&GIMBAL_CAN, &gimbalTxHeader, gimbalTxData, &txMailbox);
}
