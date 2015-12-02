# smartHome
Decentralized smart home network for home automation based on Rust and UDP Multicast

Although this is written in rust with a focus on RaspberryPi's and HummingBoard's running ArchLinux on them
it should run on any device rust can compile to.

### Current state of development
Currently this project is nowhere near finished. I decided to switch to rust as it is a compiled language which is
more efficient especially regarding that this should run on low powered devices. At the moment I'm working on the
networking and created a small UDP protocol implementation. As rustc is now running on my raspberry the next step would
be to get some implementations up and running (looking at KNX and GPIO). KNX as output should be rather easy to implement
as the addressing matches the protocol and GPIO should be fairly easy too. After that is done I'll look into some input
method (most likely a webapp using meteor) so that the basic setup works and is usable.

### Future plans
* Distribution of bigger load (e.g. webserver) to most powerful node
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
* Execute `cargo build --release --bin multicast` and copy the binary to the binary directory of your trust
* In case you want to use the GPIO plugin you have to install [wiringPi](http://wiringpi.com/download-and-install/)
