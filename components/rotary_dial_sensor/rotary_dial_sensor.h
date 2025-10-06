#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace rotary_dial_sensor {

/**
 * This polling component will listen to a rotary dial, awaiting a single digit.
 * Whenever it received a single digit, it will report that to ESPHome and then report
 * -1 in the next cycle as the default state. That way, dialing the same digit multiple
 * consecutive times will still trigger it in HomeAssistant.
 * 
 * Please note that this component has been made for German rotary dials, which have
 * two contacts:
 * 1. nsa: Normally closed, opens when the dialing starts & closes on end
 * 2. nsi: Creates the impulses during dialing.
 * 
 * Also, the timing (100ms per impulse, of that 62ms open and 38ms closed) might be
 * different in other countries.
 * 
 * Reference: https://de.wikipedia.org/wiki/Nummernschalter
 */ 
class RotaryDialSensor : public PollingComponent, public sensor::Sensor {
 public:
  RotaryDialSensor() = default;

  void set_nsa_pin(InternalGPIOPin *pin) { this->nsa_pin_ = pin; }
  void set_nsi_pin(InternalGPIOPin *pin) { this->nsi_pin_ = pin; }
  
  void setup() override;
  void update() override;

 protected:
  InternalGPIOPin *nsa_pin_;
  InternalGPIOPin *nsi_pin_;

  // initially, this will be high, as the rotary dial is in resting state
  bool nsa_last_state_ = true;
  // likewise, the resting state is low (contact is connected, not interrupted)
  bool nsi_last_state_ = false;
  int impulse_count_ = 0;
  int nsi_open_count_ = 0;
};

}  // namespace rotary_dial_sensor
}  // namespace esphome