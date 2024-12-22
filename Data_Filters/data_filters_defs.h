#ifndef DATA_FILTERS_DEFS_H
#define DATA_FILTERS_DEFS_H

typedef enum
{
 low_pass,
 high_pass
}filter_type_t;

typedef struct
{
 filter_type_t type;
 float         a_coef;
 float         b_coef;
 float         last_lp_val;
}lh_filter_params_t;

typedef struct
{
  int   n_size;
  float coef[10];
}nth_order_fir_params_t;

typedef struct
{
  float m_coef;
  float c_coef;
  float lower_bound_offset;
  float higher_bound_offset;
}band_pass_params_t;

typedef enum
{
  filter_success,
  filter_invalid_param,
  filter_invalid_precision_size,
  filter_invalid_buffer_len
}filter_err_t;

#endif //DATA_FILTERS_DEFS_H