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
      {
        "type": "toggle",
        "id": "ConfigMarker",
        "messageKey": "ConfigMarker",
        "defaultValue": "true"
      }
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
