/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "csi_adc.h"
#include "hal_adc.h"

#define ADC_FREQ_DEFAULT 25000000
#define DEFUALT_CSI_ADC_OPERATION_TIMEOUT    1000U   // 1 second
#define MAX_CHANNEL_NUM                      3U

static void csi_adc_irqhandler(unsigned int irqn, void *args)
{
	uint8_t i;
    csi_adc_t *adc;

    adc = (csi_adc_t *)args;

    hal_adc_clear_int_flag(HANDLE_REG_BASE(adc));

	for (i=0; i<adc->num; i++)
		adc->data[i] = csi_adc_read(adc);

    if (adc->callback) {
        adc->callback(adc, 0, adc->arg);
    }

    // csi_adc_start(adc);
}

csi_error_t csi_adc_init(csi_adc_t *adc, uint32_t idx)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);

    csi_error_t ret = CSI_OK;

    if (target_get(DEV_CVI_ADC_TAG, idx, &adc->dev) != CSI_OK) {
        pr_err("adc %d init failed!\n", idx);
        ret = CSI_ERROR;
    }

    adc->state.writeable = 1U;
    adc->state.readable = 1U;
    adc->state.error = 0U;
    adc->num = 0U;

    adc->priv = adc;
    hal_adc_enable_clk();
    return ret;
}

void csi_adc_uninit(csi_adc_t *adc)
{
    CSI_PARAM_CHK_NORETVAL(adc);
    unsigned long reg_base = GET_DEV_REG_BASE(adc);
    hal_adc_reset_sel_channel(reg_base, ADC_CHANNEL_SEL_Msk);
    hal_adc_stop(reg_base);
    hal_adc_disable_clk();
}

csi_error_t csi_adc_set_buffer(csi_adc_t *adc, uint32_t *data, uint32_t num)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    CSI_PARAM_CHK(data, CSI_ERROR);
    CSI_PARAM_CHK(num, CSI_ERROR);
    adc->data = data;
    adc->num = num;
	return  CSI_OK;
}

csi_error_t csi_adc_start(csi_adc_t *adc)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    csi_error_t   ret = CSI_OK;
    unsigned long reg_base = GET_DEV_REG_BASE(adc);

    hal_adc_cyc_setting(reg_base);

    do {
        /* rx buffer not full */
        if (adc->state.readable == 0U) {
            ret = CSI_BUSY;
            break;
        }

        /* last conversion existed */
        if (adc->state.writeable == 0U) {
            ret = CSI_BUSY;
            break;
        }

        hal_adc_start(reg_base);

    } while (0);

    return ret;
}


csi_error_t csi_adc_start_async(csi_adc_t *adc)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    csi_error_t   ret = CSI_OK;

    do {
        if ((adc->data == NULL) || (adc->num == 0U)) {
            ret = CSI_ERROR;
            break;
        }

        /* rx buffer not full */
        if (adc->state.readable == 0U) {
            ret = CSI_BUSY;
            break;
        }

        /* last conversion existed */
        if (adc->state.writeable == 0U) {
            ret = CSI_ERROR;
            break;
        }

        if (adc->start) {
            adc->start(adc);
        }
    } while (0);

    return ret;
}

csi_error_t csi_adc_stop(csi_adc_t *adc)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    hal_adc_stop(GET_DEV_REG_BASE(adc));

    adc->state.readable = 1U;
    adc->state.writeable = 1U;
    adc->data = NULL;
    adc->num = 0U;

    return CSI_OK;
}

csi_error_t csi_adc_stop_async(csi_adc_t *adc)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    csi_error_t ret = CSI_OK;

    if (adc->stop) {
        adc->stop(adc);
        adc->state.readable = 1U;
        adc->state.writeable = 1U;
        adc->data = NULL;
        adc->num = 0U;
    } else {
        ret = CSI_ERROR;
    }

    return ret;
}

csi_error_t csi_adc_channel_enable(csi_adc_t *adc, uint8_t ch_id, bool is_enable)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    csi_error_t ret = CSI_OK;

    unsigned reg_base = GET_DEV_REG_BASE(adc);

    if (ch_id < 1 || ch_id > 3) {
        pr_err("invalid ch_id\n");
        ret = CSI_ERROR;
    } else {
        if (is_enable)
            hal_adc_set_sel_channel(reg_base, ((uint32_t)1U << (ADC_CTRL_ADC_SEL_Pos + ch_id)));
        else
            hal_adc_reset_sel_channel(reg_base, ((uint32_t)1U << (ADC_CTRL_ADC_SEL_Pos + ch_id)));
    }

    return ret;
}

csi_error_t csi_adc_sampling_time(csi_adc_t *adc, uint16_t clock_num)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    if (clock_num > 0xF)
        return CSI_ERROR;

    hal_adc_set_sample_cycle(GET_DEV_REG_BASE(adc), clock_num);
    return CSI_OK;
}

csi_error_t csi_adc_continue_mode(csi_adc_t *adc, bool is_enable)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    return CSI_UNSUPPORTED;
}

uint32_t csi_adc_freq_div(csi_adc_t *adc, uint32_t div)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    uint32_t ret;
    unsigned long reg_base = GET_DEV_REG_BASE(adc);

    if ((div << ADC_CYC_SET_ADC_CYC_CLK_DIV_Pos) <= ADC_CYC_SET_ADC_CYC_CLK_DIV_16) {
        hal_adc_set_clk_div(reg_base, div);
        ret = hal_adc_get_clk_div(reg_base);
    } else {
        ret = 0U;
    }

    return ret;
}

int32_t csi_adc_read(csi_adc_t *adc)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    int32_t ret = CSI_OK;

    unsigned long reg_base = GET_DEV_REG_BASE(adc);

    uint32_t timestart = millis();

    while (hal_adc_get_data_ready(reg_base) == 0U) {
        if ((millis() - timestart) > DEFUALT_CSI_ADC_OPERATION_TIMEOUT) {
            ret = CSI_TIMEOUT;
            break;
        }
        delay(1);
    }

    if (ret == CSI_OK) {
        ret = (int32_t)hal_adc_get_channel1_data(reg_base);
    }

    return ret;
}

csi_error_t csi_adc_get_state(csi_adc_t *adc, csi_state_t *state)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    *state = adc->state;
    return CSI_OK;
}

uint32_t csi_adc_get_freq(csi_adc_t *adc)
{
    CSI_PARAM_CHK(adc, 0U);
    uint32_t div;

    div = hal_adc_get_clk_div(GET_DEV_REG_BASE(adc)) >> ADC_CYC_SET_ADC_CYC_CLK_DIV_Pos;

    // return soc_get_adc_freq((uint32_t)adc->dev.idx) / (2U * (div + 1U));
    // return soc_get_adc_freq((uint32_t)adc->dev.idx) / (div + 1U);
    return ADC_FREQ_DEFAULT / (div + 1U);
}

static csi_error_t csi_adc_start_intr(csi_adc_t *adc)
{
    unsigned long reg_base = HANDLE_REG_BASE(adc);
    enable_irq((uint32_t)adc->dev.irq_num);
    hal_adc_en_int_all_channel_done(reg_base);
    hal_adc_start(reg_base);
    return CVI_OK;
}

static csi_error_t csi_adc_stop_intr(csi_adc_t *adc)
{
    unsigned long reg_base;
    cvi_error_t   ret = CVI_OK;

    reg_base = HANDLE_REG_BASE(adc);
    hal_adc_dis_int_all_channel_done(reg_base);
    hal_adc_stop(reg_base);

#if 0
    uint32_t timestart = csi_tick_get_ms();

    if (cvi_adc_get_work_mode(reg_base) == CVI_ADC_CONFIG0_WORK_MODE_SCAN) {

        cvi_adc_en_stop(reg_base);

        while (cvi_adc_get_idle(reg_base) != CVI_ADC_CTRL_ADC_DATA_IDLE_Msk) {
            if ((csi_tick_get_ms() - timestart) > DEFUALT_CVI_ADC_OPERATION_TIMEOUT) {
                ret = CSI_TIMEOUT;
                break;
            }
        }
    }
#endif

    return ret;
}

csi_error_t csi_adc_configure_irq(csi_adc_t *adc, void *callback, void *arg)
{
    adc->callback = callback;
    adc->arg = arg;
    adc->start = csi_adc_start_intr;
    adc->stop = csi_adc_stop_intr;

	if (callback) {
		hal_adc_int_en(GET_DEV_REG_BASE(adc));
		request_irq((uint32_t)(adc->dev.irq_num), &csi_adc_irqhandler, 0, "adc int", adc);
	} else {
		hal_adc_int_dis(GET_DEV_REG_BASE(adc));
		disable_irq((uint32_t)adc->dev.irq_num);
	}

    return CSI_OK;
}

csi_error_t csi_adc_attach_callback(csi_adc_t *adc, void *callback, void *arg)
{
    CSI_PARAM_CHK(adc, CSI_ERROR);
    CSI_PARAM_CHK(callback, CSI_ERROR);
	return csi_adc_configure_irq(adc->priv, callback, arg);
}

void csi_adc_detach_callback(csi_adc_t *adc)
{
    CSI_PARAM_CHK_NORETVAL(adc);
	csi_adc_configure_irq(adc->priv, NULL, NULL);
}
