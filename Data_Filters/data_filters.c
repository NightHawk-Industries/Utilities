#include "data_filters.h"

#include <string.h>
#include <math.h>

/*****************************
*  @brief High/Low pass filter
******************************/
filter_err_t LH_Filter_Init(lh_filter_params_t *filter, 
                            filter_type_t type, 
                            float a, 
                            float b)
{
  memset(filter, 0, sizeof(lh_filter_params_t));
  filter->type = type;
  return LH_Filter_Set_Params(filter, a, b);
}

filter_err_t LH_Filter_Set_Params(lh_filter_params_t *filter, float a, float b)
{
  if(a < 0 || b < 0)
  {
    return filter_invalid_param;
  }
  
  filter->a_coef = a;
  filter->b_coef = b;
  
  return filter_success;
}

void LH_Filter_Clr_Last(lh_filter_params_t *filter)
{
 filter->last_lp_val = 0;
}

float LH_Filter_Apply_Filter(lh_filter_params_t *filter, float val)
{
  float new_val = 0.0;
  
  if(filter->type == low_pass)
  {
    new_val = ((filter->a_coef * val) + (filter->b_coef * filter->last_lp_val));
  }
  else
  {
    new_val = ((filter->a_coef * val) - (filter->b_coef * filter->last_lp_val));
  } 
  
  filter->last_lp_val = val;
  return new_val;
}

/***************************************
*  @brief Nth order High/Low pass filter
****************************************/
filter_err_t Nth_Order_FIR_Init(nth_order_fir_params_t *filter, 
                        float *coef, 
                        int n)
{
  memset(filter, 0, sizeof(nth_order_fir_params_t));
  
  if((sizeof(*coef) > 10) || (n > 10))
  {
    return filter_invalid_buffer_len;
  }
  
  Nth_Order_FIR_Set_Params(filter, coef, n);
  return filter_success;
}


void Nth_Order_FIR_Set_Params(nth_order_fir_params_t *filter, float *coef, int n)
{
  memcpy(filter->coef, coef, n);
  filter->n_size = n;
}

float Nth_Order_FIR_Apply_Filter(nth_order_fir_params_t *filter, float *val, int size)
{
  if(size != filter->n_size)
  {
    return -666;
  }
  
  float new_val = 0.0;

  for(int i = 0; i < size; i++)
  {
    new_val += (filter->coef[i] * val[i]);
  }
  
 return new_val;
}

/************************
* @brief Band Pass filter
*************************/
void Band_Pass_Init(band_pass_params_t *filter, 
                    float grad, 
                    float y_interc,
                    float upper_offset,
                    float lower_offset)
{
  memset(filter, 0, sizeof(band_pass_params_t));
  filter->m_coef = grad;
  filter->c_coef = y_interc;
  filter->higher_bound_offset = upper_offset;
  filter->lower_bound_offset = lower_offset;
}

float Band_Pass_Apply_Filter( band_pass_params_t *filter, 
                              float x_val, 
                              float y_val)
{
  float upper = (filter->m_coef * x_val) + (filter->c_coef + filter->higher_bound_offset);
  float lower = (filter->m_coef * x_val) + (filter->c_coef + filter->lower_bound_offset);

  if(y_val > upper)
  {
    return upper;
  }
  else if(y_val < lower)
  {
    return lower;
  }
  else
  {
    return y_val; 
  }
}


/*************************
*  @brief Test Functions
************************/
#ifdef FILTER_TEST
static void Test_HL(void)
{
  lh_filter_params_t filter;
  float filtered;

  //LH_Filter_Init(&filter, high_pass, 1.0, 1.0);
  LH_Filter_Init(&filter, low_pass, 1.0, 1.0);

  for(int i = 10; i < 20; i++)
  {
    printf("Last: %.2f\n", filter.last_lp_val);

    filtered = LH_Filter_Apply_Filter(&filter, i);

    printf("A Coef: %.2f\n", filter.a_coef);
    printf("B Coef: %.2f\n", filter.b_coef);
    printf("Last: %.2f\n", filter.last_lp_val);
    printf("Filtered: %.2f\n\n", filtered);
  }
}

static void Test_Nth_Order_FIR(void)
{
  nth_order_fir_params_t Nth_Order;

  float coefficents[NUM];
  float val[NUM];

  for(int i = 0; i < NUM; i++)
  {
    coefficents[i] = i;
    val[i] = i;

    printf("Coef: %.2f  Val: %.2f\n", coefficents[i], val[i]);
  }

  Nth_Order_FIR_Init(&Nth_Order, &coefficents[0], NUM);
  float total = Nth_Order_FIR_Apply_Filter(&Nth_Order, &val[0], NUM);

  printf("Sum: %.2f\n", total);
}
#endif //FILTER_TEST