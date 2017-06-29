module.exports = [
  {
    "type": "heading",
    "defaultValue": "TWC-ng 0.0.0:2226"
  },
  {
    "type": "section",
    "items":
    [
      {
        "type": "heading",
        "defaultValue": "ii8n.DateTimeSettings"
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
          "defaultValue": "Weather Configuration"
        },
        {
          "type": "text",
          "defaultValue": "get your own API key at <a href='http://home.openweathermap.org/users/sign_up'>OpenWWeatherMap site</a> for free and input in to a field below."
        },
        {
          "type": "input",
          "messageKey": "WeatherAPIKey",
          "id": "WeatherAPIKey",
          "label": "API Key",
          "defaultValue": ""
        }
    ]
  },
  {
  "type": "submit",
  "defaultValue": "Save Settings"
  }
];
