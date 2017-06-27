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
      }
    ]
  },
  {
  "type": "submit",
  "defaultValue": "Save Settings"
  }
];
