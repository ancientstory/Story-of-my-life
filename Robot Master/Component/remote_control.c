#include <remote_control.h>
#include <dma.h>
#include <usart.h>
#include <CAN_motor.h>

// 遥控器变量
uint8_t sbus_rx_buffer[18];
RC_ctrl_t rc_ctrl;

// 遥控器协议解析函数
static void sbus_to_rc(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl);

/*************************** main funciton ********************************/

void remote_control_init(void)
{
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, sbus_rx_buffer, 18);
}

//void USART1_IRQHandler(void)
//{
//    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
//    {
//        __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_IDLE);
//        HAL_UART_DMAStop(&huart1);
//        sbus_to_rc(sbus_rx_buffer, &rc_ctrl);
//        HAL_UART_Receive_DMA(&huart1, sbus_rx_buffer, 18);
//    }
//}
uint16_t RC_To_MOTOR(void)
{
    int16_t rate = (rc_ctrl.rc.ch[2] - RC_CH_VALUE_OFFSET) / (RC_CH_VALUE_MAX - RC_CH_VALUE_OFFSET);
    return MOTOR_3508_CURRENT_MAX * rate;
}
/************************* auxiliary function *****************************/
static void sbus_to_rc(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl)
{
    if (sbus_buf == NULL || rc_ctrl == NULL)
    {
        return;
    }

    rc_ctrl->rc.ch[0] = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff;        //!< Channel 0
    rc_ctrl->rc.ch[1] = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff; //!< Channel 1
    rc_ctrl->rc.ch[2] = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) |          //!< Channel 2
                         (sbus_buf[4] << 10)) &
                        0x07ff;
    rc_ctrl->rc.ch[3] = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff; //!< Channel 3
    rc_ctrl->rc.s[0] = ((sbus_buf[5] >> 4) & 0x0003);                       //!< Switch left
    rc_ctrl->rc.s[1] = ((sbus_buf[5] >> 4) & 0x000C) >> 2;                  //!< Switch right
    rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //!< Mouse X axis
    rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //!< Mouse Y axis
    rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //!< Mouse Z axis
    rc_ctrl->mouse.press_l = sbus_buf[12];                                  //!< Mouse Left Is Press ?
    rc_ctrl->mouse.press_r = sbus_buf[13];                                  //!< Mouse Right Is Press ?
    rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);                    //!< KeyBoard value
    rc_ctrl->rc.ch[4] = sbus_buf[16] | (sbus_buf[17] << 8);                 // NULL

    rc_ctrl->rc.ch[0] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[1] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[2] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[3] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[4] -= RC_CH_VALUE_OFFSET;
}
