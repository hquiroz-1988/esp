set(COMPONENT_SRCS
    "gpio.cpp"
    "i2c_bus.cpp"
    "i2c_device.cpp"
    "interrupt_base.cpp"
    "i2c_task.c"
)
set(COMPONENT_ADD_INCLUDEDIRS "include")
set(COMPONENT_REQUIRES common)

register_component()
