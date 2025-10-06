# Rotary Dial ESPHome Component

This is an [external component for ESPHome](https://esphome.io/components/external_components/) for using an old rotary
dial from a phone as a sensor component in ESPHome.

This component is made specifically for [German rotary dials](https://en.wikipedia.org/wiki/Rotary_dial) which have two
contacts as opposed to only one. See the [source code](rotary_dial_sensor/rotary_dial_sensor.h) for more information.

The component will emit each digit it reads and then immediately go back to the default state of `-1`. This
allows you to dial the same digit multiple consecutive times without having to dial another digit in between.


## Installation

You can either include the component directly from its GitHub repository, or copy it into your ESPHome config folder.

When **including via GitHub**, you need to add this to your config:

```yaml
external_components:
  - source: github://misternerd/esphome-rotary-dial-component@master
    components: [ rotary_dial_sensor ]
```

When **copying the component**, copy the `components` folder into your ESPHome config folder and add the 
following to your config:

```yaml
external_components:
  - source:
      type: local
      path: components
    components: [ rotary_dial_sensor ]
```

For both methods, you can then define the sensor in your ESPHome config like the following example. Be sure to 
**change the pins** (`D1` and `D2` in the example) to the ones you're using. In the example, `D1` is used as the NSA pin
while `D2` is used as the NSI pin.

```yaml
sensor:
  # Rotary Dial
  - platform: rotary_dial_sensor
    name: "Rotary Dial"
    nsa_pin: D1
    nsi_pin: D2
    update_interval: 25ms
```

This will give you a sensor in ESPHome which will contain the number dialed on the rotary dial for a short time, before
defaulting back to `-1`.


## A word on timing

The component is pretty time-sensitive. If you change anything on your ESP (like adding/removing components), the timing
might be off due to more/less load. This will lead to the sensor not detecting the correct digits anymore.

In that case, you can play around with the number in `update_interval: 25ms` in the sensor definition. This
value is the update interval in milliseconds.

I'm using a value of `25` on an ESP8266.