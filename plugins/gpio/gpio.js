module.exports = function (data, host, options) {
    if (host && host.debug) {
        console.log("Registering GPIO plugin");
    }
    
    
    return {
        onEvent: function (eventType, payload) {
            console.log("GPIO: ", eventType, payload);
        }
    }    
    
    // return {
    //     firstName: "John",
    //     lastName: "Smith",
    //     getDisplayName: function () {
    //         return this.firstName + ' ' + this.lastName;
    //     }
    // };
};
