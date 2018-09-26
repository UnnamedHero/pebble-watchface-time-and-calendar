const secondsPerMinute = 60;

export default () => new Date().getTimezoneOffset() * secondsPerMinute;
