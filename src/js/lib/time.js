const secondsPerMinute = 60;

export const getTZOffestInSeconds = () => new Date().getTimezoneOffset() * secondsPerMinute;

export const getLocalTimeFromUtc = utc => new Date(utc - getTZOffestInSeconds());

export const getLocalTimeStampFromUtc = utc => utc - getTZOffestInSeconds();
