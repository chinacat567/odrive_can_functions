#include "signal_generator.hpp"

int subdev = 1;     /* change this to your input subdevice */
int chan = 0;     /* change this to your channel */
int range = 0;      /* more on this later */
int aref = AREF_GROUND;   /* more on this later */
const char filename[] = "/dev/comedi0";

double adc_reading(comedi_t *device_t)
{
  
  lsampl_t data;
  double physical_value;
  int retval;
  comedi_range *range_info;
  lsampl_t maxdata;
  retval = comedi_data_read(device_t, subdev, chan, range, aref,
          &data);
  if (retval < 0) {
    comedi_perror(filename);
    return 1;
  }

  comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
  range_info = comedi_get_range(device_t, subdev, chan, range);
  maxdata = comedi_get_maxdata(device_t, subdev, chan);
  physical_value = comedi_to_phys(data, range_info, maxdata);
  if (isnan(physical_value)) {
    printf("Out of range [%g,%g]",
           range_info->min, range_info->max);
  } else {
    switch(range_info->unit) {
    case UNIT_volt: printf(" V"); break;
    case UNIT_mA: printf(" mA"); break;
    case UNIT_none: break;
    default: printf(" (unknown unit %d)",
        range_info->unit);
    }
  }
  return physical_value;
}