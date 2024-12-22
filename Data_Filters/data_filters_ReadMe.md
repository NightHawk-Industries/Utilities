# Data Filters

## Summary
Data Filters provide basic implementation of:
  - Low-Pass Filter
  - High-Pass Filter
  - FIR Filter
  - Band pass filter

Allowing for smoother data sequences based upon the users needs

## Low Pass Filter
The low pass filter applies the formula `Y[1] = AX[1] + BX[0]` to the value passed to the `val (X[1])` parameter of `LH_Filter_Apply_Filter`. After returning the smoothed value `Y[1]`, `LH_Filter_Apply_Filter` automatically updates the stored value `x[0]` with the current `X[1]`, ensuring that the user only needs to pass the current x value.

### Initialising the Low Pass Filter
To initialise the low pass filter, first create an object such as `lh_filter_params_t my_filter` followed by a call to `LH_Filter_Init`. The init function contains the following parameters:

| Parameter                     |  Function                                                                       |
|-------------------------------|---------------------------------------------------------------------------------|
| `lh_filter_params_t *filter`  | Pointer to the filter object                                                    |
| `filter_type_t type`          | Specifies if the filter is intended to operate as a Low/High pass filter        |
| `float a`                     | The value of the `A` coefficiant                                                |
| `float b`                     | The value of the `B` coefficiant, **Note:** This value should represent `1 - A` |

**Note:** The values of `A` and `B` coefficiants must be positive otherwise `filter_invalid_param` will be returned.

### Applying the filter
To apply the filter to the current `X[1]` value, simply call `LH_Filter_Apply_Filter`. This function requires a pointer to the filter instance, followed by the `X[1]` value. The smoother value will then be returned as a float.


## High Pass Filter
The high pass filter operates in much the same way as the low pass, utilising the same function calls. The exception being that when calling `LH_Filter_Init` the `filter_type_t type` parameter is set to `high_pass`, thus updating the `type` field within the filter object. This instructs future calls to `LH_Filter_Apply_Filter` to apply the formula `Y[1] = AX[1] - BX[0]` to all passed values when this particular filter istance is also passed.



## Finite Impulse Response (FIR) Filter
The FIR filter applies the formula `Y[n] = sum(A[0]X[n] + A[1]X[n-1] + A[2]X[n-2] ... + A[N]X[n-N])` to a given set of data.


### Initialising the FIR Filter
As with the low/high pass filter, an instance of the filter must be created, `nth_order_fir_params_t my_FIR`. The function `Nth_Order_FIR_Init` may now be called which contains the following parameters:

| Parameter                        | Function                                                            |
|----------------------------------|---------------------------------------------------------------------|
| `nth_order_fir_params_t *filter` | Pointer to the filter instance                                      |
| `float *coef`                    | Pointer to the array of coefficiants `A[0], A[1] ... A[N]`          |
| `int n`                          | Size of the coefficiant buffer. **Note:** Currently limited to 10.  |

`Nth_Order_FIR_Init` will return `filter_invalid_buffer_len` if `n > 10` or `*coef > 10`.


### Appling th FIR filter
The filter may be applied to a sample group by calling `Nth_Order_FIR_Apply_Filter` which requires the following parameters:

| Parameter                        | Function                                    |
|----------------------------------|---------------------------------------------|
| `nth_order_fir_params_t *filter` | Pointer to the filter instance              |
| `float *val`                     | Pointer to the data buffer to be processed  |
| `int size`                       | Size of the data buffer                     |

`Nth_Order_FIR_Apply_Filter` will return `-666` if `int size` does not equal the size of the coefficiant buffer.



## Band Pass Filter
The band pass filter takes a plot (the Normal) in the shape of `Y = MX + C` and clones it by applying the upper and lower bound offset. This results in the upper boundary having the shape of `Y = MX + (C + upper_offset)` and the lower bounday having the shape `Y = MX + (C + lower_offset)`.

### Initialising the Band Pass Filter
The filter may be initialise by creating an instance of the filter such as `band_pass_params_t my_bandPass`, then calling `Band_Pass_Init` which contains the following parameters:


| Parameter                       | Function                                                            |
|---------------------------------|---------------------------------------------------------------------|
| `band_pass_params_t *filter`    | Pointer to the filter instance                                      |
| `float grad`                    | Gradient of the Normal (Will be applied to upper and lower bounds)  |  
| `float y_interc`                | Y - intercept of the Normal                                         |
| `float upper_offset`            | Y offset of the upper boundary                                      |
| `float lower_offset`            | Y offset of the lower boundary                                      |


### Applying the Band Pass Filter
The filter is applied by calling `Band_Pass_Apply_Filter`, which requires the non-filtered x and y value.
The y value is then compared to the acceptable y value at the upper and lower bounds at the given x value.
In the event the unfiltered y value lays beyond the boundaries, it will be attenuated to the maximum allowable value as define by the appropriate boundary. If the y value lays within the boundaries, then it will simply be returned.

| Parameter                       | Function                       |
|---------------------------------|--------------------------------|
| `band_pass_params_t *filter`    | Pointer to the filter instance |
| `float x_val`                   | X Coordinate                   |
| `float y_val`                   | Y Coordinate                   |