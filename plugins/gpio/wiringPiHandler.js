var execSync = require("child_process").execSync;

module.exports = function (pin, mode, startValue) {
    if (startValue === null) startValue = 1;
    if (mode === null) mode = 'out';

    execSync('gpio -g write ' + pin + ' ' + startValue);
    execSync('gpio -g mode ' + pin + ' ' + mode);

    GPIO = {};

    GPIO.pin = pin;

    GPIO.set = function (value) {
        execSync('gpio -g write ' + this.pin + ' ' + value)
    };
    GPIO.reset = function () {
        execSync('gpio -g write ' + this.pin + ' 1');
        execSync('gpio -g mode ' + this.pin + ' in');
    }
};