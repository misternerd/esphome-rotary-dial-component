#include "esphome.h"

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
class RotaryDialPollingComponent : public PollingComponent, public Sensor
{
    const int nsaPin;
    const int nsiPin;

    // initially, this will be high, as the rotary dial is in resting state
    int nsaLastState = HIGH;
    // likewise, the resting state is low (contact is connected, not interrupted)
    int nsiLastState = LOW;
    int impulseCount = 0;
    int nsiOpenCount = 0;

public:
    RotaryDialPollingComponent(const int nsaPin, const int nsiPin) : PollingComponent(25), nsaPin(nsaPin), nsiPin(nsiPin) {}

    void setup() override
    {
        pinMode(nsaPin, INPUT_PULLUP);
        pinMode(nsiPin, INPUT_PULLUP);
        ESP_LOGD("RotaryDial", "Initialized with nsaPin=%i, nsiPin=%i", nsaPin, nsiPin);
    }

    void update() override
    {
        int nsaState = digitalRead(nsaPin);
        int nsiState = digitalRead(nsiPin);

        if (nsaState == LOW)
        {
            // initialize
            if(nsaState != nsaLastState) {
                ESP_LOGD("RotaryDial", "Detected start of digit");
                impulseCount = 0;
                nsiOpenCount = 0;
            }
            else {
                // we're running every 25ms and we expect nsi to be open for min 62ms => need 2 consecutive times open per impulse
                if(nsiState == HIGH) {
                    nsiOpenCount++;
                }
                else {
                    nsiOpenCount = 0;
                }

                if(nsiOpenCount == 2) {
                    ESP_LOGD("RotaryDial", "Received 3 consecutive open nsi contacts, adding impulse, now=%i", this->impulseCount);
                    impulseCount++;
                }
            }
        }
        else
        {
            if (nsaState != nsaLastState)
            {
                if(impulseCount > 0) {
                    ESP_LOGD("RotaryDial", "nsa closed again, counted %i impulses => sending", this->impulseCount);
                    publish_state(impulseCount);

                    // publish default state so dialing same digit consecutive will still be picked up
                    defer([=]() -> void {
                        publish_state(-1);
                    });
                }
                else {
                    ESP_LOGD("RotaryDial", "nsa closed again, but didn't register any impulses");
                }
            }
        }

        nsaLastState = nsaState;
        nsiLastState = nsiState;
    }

};