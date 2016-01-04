# smartHome
Decentralized smart home network for home automation based on Rust and UDP Multicast

Although this is written in rust with a focus on RaspberryPi's and HummingBoard's running ArchLinux
it should work on any device rust can compile to.

### Current state of development
Currently this project is nowhere near finished. I decided to switch to rust as it is a compiled language which is
more efficient especially regarding this project running on low powered devices. Current focus is on the
networking and a UDP protocol implementation using multicast. As rustc is now running on my raspberry the next step would
be to get some implementations up and running (looking at KNX and GPIO). I've got a alpha version running but for testing
purposes all the addresses are hardcoded so its not ready just yet. After that is done I'll look into some input
methods (most likely a webapp using meteor) so that the basic setup works and is usable.

### Future plans
* Distribution of bigger load (e.g. webserver) to most powerful node
* Auto discovery of network devices
* Encryption of datagrams and multiple concurrent networks on the same address
* Inputs
    * KNX<sup>1</sup>
    * Temperature/Pressure/Humidity sensor
    * Button panels
    * Touchpanel
    * Virtual inputs (apps, website)
* Outputs
    * KNX<sup>1</sup>
    * GPIO relay control
* WebApp for remote access from outside
* Automatic WiFi roaming
    * Connections between nodes via WiFi
    * Network wide change of security/wifi properties with one click
* iOS native app <sub><sup>(Android as soon as AnyMesh supports it)</sup></sub>
    * Indoor localization
    * Fast access to controls in current room
* REST API
* Indoor localization via BLE
    * Music that follows you through the rooms
    * Automated light transitions between rooms
* Video streaming to TVs all over the network <sub><sup>(Steam In-home-streaming anyone?)</sup></sub>
* Voice recognition
* Printer sharing

<sub><sup>1</sup> Running EIBd service required.</sub>

### Compiling on ARMv6/7
* Follow this [guide](https://github.com/warricksothr/RustBuild) to get rustc/cargo running on your raspi
* Execute `cargo build --release` and copy the binary to the binary directory of your trust
* In case you want to use the GPIO plugin you have to install [wiringPi](http://wiringpi.com/download-and-install/)

## Testing it out
* Now just type `cargo run --release`
* Open a second terminal and start the client example `cargo run --example client`
