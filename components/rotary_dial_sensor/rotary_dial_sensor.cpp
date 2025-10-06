#include "rotary_dial_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rotary_dial_sensor {

static const char *const TAG = "rotary_dial_sensor";

void RotaryDialSensor::setup() {
  this->nsa_pin_->setup();
  this->nsa_pin_->pin_mode(gpio::FLAG_INPUT | gpio::FLAG_PULLUP);
  this->nsi_pin_->setup();
  this->nsi_pin_->pin_mode(gpio::FLAG_INPUT | gpio::FLAG_PULLUP);
  ESP_LOGD(TAG, "Initialized with nsa_pin=%i, nsi_pin=%i", this->nsa_pin_->get_pin(), this->nsi_pin_->get_pin());
}

void RotaryDialSensor::update() {
  bool nsa_state = this->nsa_pin_->digital_read();
  bool nsi_state = this->nsi_pin_->digital_read();

  if (!nsa_state) {  // LOW means dial is active
    // initialize
    if (nsa_state != this->nsa_last_state_) {
      ESP_LOGD(TAG, "Detected start of digit");
      this->impulse_count_ = 0;
      this->nsi_open_count_ = 0;
    } else {
      // we're running every 25ms and we expect nsi to be open for min 62ms => need 2 consecutive times open per impulse
      if (nsi_state) {  // HIGH means contact is open
        this->nsi_open_count_++;
      } else {
        this->nsi_open_count_ = 0;
      }

      if (this->nsi_open_count_ == 2) {
        ESP_LOGD(TAG, "Received 3 consecutive open nsi contacts, adding impulse, now=%i", this->impulse_count_);
        this->impulse_count_++;
      }
    }
  } else {
    if (nsa_state != this->nsa_last_state_) {
      if (this->impulse_count_ > 0) {
        ESP_LOGD(TAG, "nsa closed again, counted %i impulses => sending", this->impulse_count_);
        this->publish_state(this->impulse_count_);

        // publish default state so dialing same digit consecutive will still be picked up
        this->set_timeout(1000, [this]() -> void {
          this->publish_state(-1);
        });
      } else {
        ESP_LOGD(TAG, "nsa closed again, but didn't register any impulses");
      }
    }
  }

  this->nsa_last_state_ = nsa_state;
  this->nsi_last_state_ = nsi_state;
}

}  // namespace rotary_dial_sensor
}  // namespace esphome