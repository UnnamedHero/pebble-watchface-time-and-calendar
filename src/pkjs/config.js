module.exports = [
  {
    "type": "heading",
    "defaultValue": "Watchface configuration"
  },
  {
    "type": "section",
    "items":
    [
      {
        "type": "heading",
        "defaultValue": "Interface settings"
      },
      {
        "type": "color",
        "messageKey": "BackgroundColor",
        "label": "Background color",
        "defaultValue": "000000",
      },
        {
        "type": "color",
        "messageKey": "FontColor",
        "label": "Font color",
        "defaultValue": "FFFFFF",
      }
    ]
  },
  {
    "type": "section",
    "items":
    [
      {
        "type": "heading",
        "defaultValue": "Date and time settings"
      },
      {
        "type": "select",
        "messageKey": "ClockFormat",
        "label": "Clock format",
        "options" : [
          {
            "label": "24h format",
            "value": "%H:%M"
          },
          {
            "label": "AM/PM format",
            "value": "%I:%M"
          },
          {
            "label": "Use watch settings",
            "value": "cf_respect"
          }
        ],
        "defaultValue": "cf_respect"
      },
      {
        "type": "select",
        "label": "Week starts on...",
        "messageKey": "SundayFirst",
        "options": [
          {
            "label": "Sunday",
            "value": "sun"
          },
          {
            "label": "Monday",
            "value": "mon"
          }
        ],
        "defaultValue": "mon"
      },
      {
        "type": "select",
        "label": "Date format",
        "messageKey": "DateFormat",
        "options": [
          {
            "label" : "2017.12.31",
            "value": "%Y.%m.%d"
          },
          {
            "label" : "31.12.2017",
            "value": "%d.%m.%Y"
          },
          {
            "label" : "december 31, 2017",
            "value": "%B %d, %Y"
          }

        ],
        "defaultValue": "%Y.%m.%d"
      },
      {       
        "type": "select",
        "label": "Date separator",
        "messageKey": "DateFormatSeparator",
        "options": [
          {
            "label" : ".",
            "value": "."
          },
          {
            "label" : ",",
            "value": ","
          },
          {
            "label" : "-",
            "value": "-"
          },
          {
            "label" : "/",
            "value": "/"
          },
        ],
        "defaultValue": "."
      },
      {
        "type": "toggle",
        "messageKey": "QuietTime",
        "label": "Enable 'Quiet Time'",
        "defaultValue": "true",
        "capabilities": ["PLATFORM_APLITE"]
      },
      {
        "type": "input",
        "messageKey": "QuietTimeBegin",
        "label": "start at:",
        "defaultValue": "23:00:00",
        "attributes" : {
          "type" : "time"
        },
        "capabilities": ["PLATFORM_APLITE"]
      },
      {
        "type": "input",
        "messageKey": "QuietTimeEnd",
        "label": "end at:",
        "defaultValue": "06:30:00",
        "attributes" : {
          "type" : "time"
        },
        "capabilities": ["PLATFORM_APLITE"]
      },
    ]
  },
  {
    "type": "section",
    "items":
    [
      {
        "type": "heading",
        "defaultValue": "Calendar settings"
      },
      {
        "type": "select",
        "messageKey": "CalendarWeeks",
        "label": "Weeks view",
        "options" : [
          {
            "label": "2 previous weeks, current",
            "value": "cal_ppc"
          },
          {
            "label": "prevoius, current, next",
            "value": "cal_pcn"
          },
          {
            "label": "current, next 2 weeks",
            "value": "cal_cnn"
          },
        ],
        "defaultValue": "cal_pcn"
      },
      {
        "type": "checkboxgroup",
        "messageKey": "CalendarWeekDayMark",
        "label": "Mark current day of a week with",
        "options": [ "Bold text", "Inverted color"],
        "defaultValue" : [true, false]
      },
      {
        "type": "checkboxgroup",
        "messageKey": "CalendarDayMark",
        "label": "Mark current day with",
        "options": [ "Bold text", "Inverted color"],
        "defaultValue" : [false, true]
      }, 
      {
        "type": "toggle",
        "messageKey": "CalendarSmallOtherDays",
        "label": "Reduce font size for other days",
        "defaultValue": false
      },      
    ]
  },
  {
    "type": "section",
    "items": [
        {
          "type": "heading",
          "defaultValue": "Weather configuration"
        },
        {
          "type": "text",
          "defaultValue": "Get your own API key at <a href='http://home.openweathermap.org/users/sign_up'>OpenWeatherMap site</a> for free and input in to a field below."
        },
        {
          "type": "input",
          "messageKey": "WeatherAPIKey",
          "id": "WeatherAPIKey",
          "label": "API Key",
          "defaultValue": ""
        },
        {
          "type": "select",
          "messageKey": "WeatherUpdatePeriod",
          "label": "Update period",
          "options" : [
            {
              "label": "every 15 min",
              "value": "15"
            },
            {
              "label": "every 30 min",
              "value": "30"
            },
            {
              "label": "every hour",
              "value": "60"
            },
            {
              "label": "every 3 hours",
              "value": "3h"
            }
          ],
          "defaultValue": "60"
        },
        {
          "type": "select",
          "messageKey": "WeatherUnits",
          "label": "Weather units",
          "options" : [
            {
              "label": "Fahrenheit",
              "value": "imperial"
            },
            {
              "label": "Celsius",
              "value": "metric"
            },
            {
              "label": "Kelvin",
              "value": "default"
            },
          ],
          "defaultValue": "metric"
        },
        {
          "type": "select",
          "messageKey": "ForecastType",
          "label": "Forecast type",
          "options" : [
            {
              "label": "Disabled",
              "value": "ft_off"
            },
            {
              "label": "3 hours",
              "value": "ft_3h"
            },
            {
              "label": "6 hours",
              "value": "ft_6h"
            }
          ],
          "defaultValue": "ft_3h"
        },
        {
          "type": "text",
          "defaultValue": "Shake your Pebble to view forecast. Set timeout for auto switch back."
        },
        {
          "type": "select",
          "messageKey": "SwitchBackTimeout",
          "label": "Switch back timeout in seconds",
          "options" : [
            {
              "label": "manual switch",
              "value": "0"
            },
            {
              "label": "15",
              "value": "15"
            },
            {
              "label": "20",
              "value": "20"
            },
            {
              "label": "25",
              "value": "25"
            },
            {
              "label": "30",
              "value": "30"
            },
            {
              "label": "45",
              "value": "45"
            },         
          ],
          "defaultValue" : "15"
        }
    ]
  },
  {
  "type": "section",
  "items":
    [
      {
        "type": "heading",
        "defaultValue": "Vibration settings"
      },
      {
        "type": "toggle",
        "messageKey": "RespectQuietTime",
        "label": "Vibe during Quiet Time",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "VibrateDuringCharged",
        "label": "Vibe during Pebble charging",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "VibrateConnected",
        "id": "VibrateConnected",
        "label": "Vibe on phone connect",
        "defaultValue": true
      },
      {
        "type": "select",
        "messageKey": "VibrateConnectedType",
        "id": "VibrateConnectedType",
        "label": "Vibe type",
        "options" : [
          {
            "label": "short pulse",
            "value": "1"
          },
          {
            "label": "double short pulse",
            "value": "2"
          },
          {
            "label": "long pulse",
            "value": "4"
          }
        ],
        "defaultValue": "1"
      },
      {
        "type": "toggle",
        "messageKey": "VibrateDisconnected",
        "id": "VibrateDisconnected",
        "label": "Vide on phone disconnect",
        "defaultValue": true
      },
      {
        "type": "select",
        "messageKey": "VibrateDisconnectedType",
        "id": "VibrateDisconnectedType",
        "label": "Vibe type",
        "options" : [
          {
            "label": "short pulse",
            "value": "1"
          },
          {
            "label": "double short pulse",
            "value": "2"
          },
          {
            "label": "long pulse",
            "value": "4"
          }
        ],
        "defaultValue": "2"
      },
      {
        "type": "toggle",
        "messageKey": "VibratePeriodic",
        "id": "VibratePeriodic",
        "label": "Periodic Vibration",
        "defaultValue": true
      },
      {
        "type": "select",
        "messageKey": "VibratePeriodicPeroid",
        "id": "VibratePeriodicPeroid",
        "label": "Period",
        "options" : [
          {
            "label": "every 15 min",
            "value": "15"
          },
          {
            "label": "every 30 min",
            "value": "30"
          },
          {
            "label": "every hour",
            "value": "60"
          },
          {
            "label": "every 3 hours",
            "value": "3h"
          }
        ],
        "defaultValue": "1h"
      },
      {
        "type": "select",
        "messageKey": "VibratePeriodicType",
        "id": "VibratePeriodicType",
        "label": "Vibe type",
        "options" : [
          {
            "label": "short pulse",
            "value": "1"
          },
          {
            "label": "double short pulse",
            "value": "2"
          },
          {
            "label": "triple short pulse",
            "value": "3"
          },
          {
            "label": "long pulse",
            "value": "4"
          }

        ],
        "defaultValue": "3"
      },

    ]
  },
  {
  "type": "submit",
  "defaultValue": "Save Settings"
  }
];
