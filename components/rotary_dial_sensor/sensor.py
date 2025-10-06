import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor
from esphome.const import CONF_ID

DEPENDENCIES = []

rotary_dial_sensor_ns = cg.esphome_ns.namespace('rotary_dial_sensor')
RotaryDialSensor = rotary_dial_sensor_ns.class_('RotaryDialSensor', sensor.Sensor, cg.PollingComponent)

CONF_NSA_PIN = "nsa_pin"
CONF_NSI_PIN = "nsi_pin"

CONFIG_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(RotaryDialSensor),
    cv.Required(CONF_NSA_PIN): pins.gpio_input_pin_schema,
    cv.Required(CONF_NSI_PIN): pins.gpio_input_pin_schema,
}).extend(cv.polling_component_schema("25ms"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    nsa_pin = await cg.gpio_pin_expression(config[CONF_NSA_PIN])
    cg.add(var.set_nsa_pin(nsa_pin))

    nsi_pin = await cg.gpio_pin_expression(config[CONF_NSI_PIN])
    cg.add(var.set_nsi_pin(nsi_pin))