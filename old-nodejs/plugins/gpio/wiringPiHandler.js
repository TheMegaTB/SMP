var execSync = require("child_process").execSync;

module.exports = function (pin, sync, mode) {
    var startValue = sync[pin];
    if (typeof mode == 'undefined') mode = 'out';
    if (typeof startValue == 'undefined') startValue = 1;

    execSync('gpio -g write ' + pin + ' ' + startValue);
    execSync('gpio -g mode ' + pin + ' ' + mode);


    GPIO = {};

    GPIO.pin = pin;

    GPIO.set = function (value, sync) {
        sync[this.pin] = value;
        execSync('gpio -g write ' + this.pin + ' ' + value);
    };

    GPIO.reset = function () {
        execSync('gpio -g write ' + this.pin + ' 1');
        execSync('gpio -g mode ' + this.pin + ' in');
    };

    return GPIO;
};