# Rotary Dial ESPHome Component

This is a [custom component for ESPHome](https://esphome.io/components/sensor/custom.html) for using an old rotary dial
as a sensor component in ESPHome.

This component is made specifically for [German rotary dials](https://en.wikipedia.org/wiki/Rotary_dial) which have two
contacts as opposed to only one. See the [source code](custom_components/rotary_dial_sensor.h) for more information.

The component will emit each digit it reads and then immediately go back to the default state of `-1`. This
allows you to dial the same digit multiple consecutive times without having to dial another digit in between.


## Installation

1. Copy the `custom_components` folder to the root of your ESPHome configuration folder.
2. Include the custom component in the `esphome` section of your device config file, like so:
```yaml
esphome:
  includes:
    - custom_components/rotary_dial_sensor.h
```
3. Declare a sensor entry for the rotary dial, **changing the pins to the ones you're using**.
   First argument is the NSA pin (`D1` in the example), second pin is the NSI pin (here `D2`).
```yaml
sensor:
  - platform: custom
    lambda: |-
      auto rotary_dial_component = new RotaryDialPollingComponent(D1, D2);
      App.register_component(rotary_dial_component);
      return {rotary_dial_component};
    sensors:
      name: "Rotary Dial"
```

This will give you a sensor in ESPHome which will contain the number dialed on the rotary dial for a short time, before
defaulting back to `-1`.


## A word on timing

The component is pretty time sensitive. If you change anything on your ESP (like adding/removing components), the timing
might be off due to more/less load. This will lead to the sensor not detecting the correct digits anymore.

In that case, you can play around with the number in `PollingComponent(25)` in the `rotary_dial_sensor.h` file. This
value is the update interval in milliseconds.

I'm using a value of `25` on an ESP8266.