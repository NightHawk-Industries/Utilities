#ifndef DATA_FILTERS_H
#define DATA_FILTERS_H

#include "data_filters_defs.h"

/*****************************
*  @brief High/Low pass filter
*******************************/
filter_err_t LH_Filter_Init(lh_filter_params_t *filter, 
                            filter_type_t type, 
                            float a, 
                            float b);

filter_err_t LH_Filter_Set_Params(lh_filter_params_t *filter, 
                                  float a, 
                                  float b);

void LH_Filter_Clr_Last(lh_filter_params_t *filter);

float LH_Filter_Apply_Filter(lh_filter_params_t *filter, float val);


/***************************************
*  @brief Nth order High/Low pass filter
****************************************/
filter_err_t Nth_Order_FIR_Init(nth_order_fir_params_t *filter,
                        float *coef, 
                        int n);

void Nth_Order_FIR_Set_Params(nth_order_fir_params_t *filter, 
                              float *coef, 
                              int n);

void Nth_Order_FIR_Clr_Last(nth_order_fir_params_t *filter);

float Nth_Order_FIR_Apply_Filter(nth_order_fir_params_t *filter, float *val, int size);

/************************
* @brief Band Pass filter
*************************/
void Band_Pass_Init(band_pass_params_t *filter, 
                    float grad, 
                    float y_interc,
                    float upper_offset,
                    float lower_offset);

float Band_Pass_Apply_Filter( band_pass_params_t *filter,
                              float x_val, 
                              float y_val);


/*************************
*  @brief Test Functions
************************/
#ifdef FILTER_TEST
#define NUM 100
void Test_HL(void);
void Test_Nth_Order_FIR(void);
#endif //FILTER_TEST
#endif //DATA_FILTERS_H