# Time-and-Calendar watchface

Welcome to a "Time-and-Calendar" watchface for Pebble smartwatch. Watch was was created as a trubute to such watchfaces as TCW (https://github.com/alexsum/TCW) and Timely (https://github.com/cynorg/PebbleTimely). The main goal of watchface creation is self-education process on mine. 

Watchwace mainly designed for Pebble classic watches (I have Pebble classic). Written in C, configuration part in js.

The code is licenced under GPLv3 license. 

Main features:
- bluetooth connection status
- smartwatch battery status
- shows date and time
- shows a three-week calendar grid
- shows current weather status (OpenWeatherMap API key required)
- weather forecast
- various vibration settings
- font and background color options
- simple quiet time support for pebble classic watches (other platforms have native support I suppose)
- english and russian translations.


Configurable with Clay for Pebble framework. It means - no external website is needed to load configurartion page, all is done on your smartphone.

Feel free to post any comments on GitHub page, report bugs, ask for features.

Nearest plans:
- add more configurable options;
- support for color screens (color icons) and pebble time screen size;
- code cleanup and optimizations, more locales support.

Development versions can be downloaded from [google drive](https://drive.google.com/open?id=0B9g5sjcPqSJfRXpMUFE3Y2c1RGs)

Lastest stable version is 0.24.0 (available in [Pebble Store](https://apps.getpebble.com/applications/596fb1650dfc321335000144))

Lastest development version is 0.20.0

See CHANGELOG for more details.

---

Credits:
- Time font: Bebas Neue, [official site](http://www.fontfabric.com/bebas-neue/)
- Climate icons: based on [Climacons font](https://github.com/christiannaths/Climacons-Font) and [Weather-icons](https://github.com/erikflowers/weather-icons)