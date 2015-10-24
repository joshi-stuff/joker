#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define CLI              asm volatile("cli")
#define STI              asm volatile("sti")
#define INT(int_number)  asm volatile("int "#int_number)

/* TODO: this should be an assembled header in a standalone executable */
#define DEFINE_DRIVER(driver_id)\
  static void load();\
  static void unload();\
  static void on_set_device(symbol class, symbol id, device* dev);\
  static void on_remove_device(symbol class, symbol id);\
  static driver driver_id##_driver_ = {\
    #driver_id, load, unload, on_set_device, on_remove_device\
  };\
  driver* driver_id##_driver = &driver_id##_driver_;

#define DEFINE_DEVICE(driver_id, dev_class, fields) typedef struct _##driver_id##_##dev_class {\
   dev_class dev;\
   fields;\
} driver_id##_##dev_class;

#define DEV_LOAD(driver_id, dev_class) driver_id##_##dev_class* dev_class = (driver_id##_##dev_class*)dev;

#define DEV_C0(dev, method)       ((dev)->method((dev)))
#define DEV_CN(dev, method, ...)  ((dev)->method((dev), __VA_ARGS__))

#define DEV_F_STATIC
#define DEV_F0(ret_type, method)      DEV_F_STATIC ret_type (*method)(void* dev);
#define DEV_FN(ret_type, method, ...) DEV_F_STATIC ret_type (*method)(void* dev, __VA_ARGS__);

#define SET_DEVICE_DEP(var, driver_class, driver_id)\
  if (is(class, #driver_class) && is(id, driver_id)) {\
    var = (driver_class*)dev;\
  }
#define REMOVE_DEVICE_DEP(var, driver_class, driver_id)\
if (is(class, #driver_class) && is(id, driver_id)) {\
  var = NULL;\
}
#define CHECK_DEVICE_DEPS(driver_class, driver_id, pdev, cond)\
  if (cond) {\
    set_device(driver_class, driver_id, pdev);\
  }\
  else {\
    remove_device(driver_class, driver_id);\
  }

#endif
