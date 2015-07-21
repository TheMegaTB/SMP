# smartHome
Decentralized smart home network for home automation based on HummingBoard's

Theoretically it could run on any OS that is supported by NodeJS but this project is focused on ArchLinux
so in later releases it might be a bit tricky to get it running on other operating systems

### Future plans
* Distribution of server load to most powerful node
* Inputs
    * KNX USB
    * Temperature/Pressure/Humidity sensor
    * Button panels
    * Touchpanel
* Outputs
    * KNX USB
    * GPIO relay control
* WebUI for remote access from outside
* Automatic WiFi roaming
    * Connections between nodes via WiFi
    * Network wide change of security/wifi properties with one click
* iOS native app <sub><sup>Android as soon as AnyMesh supports it</sup></sub>
    * Indoor localization
    * -> Fast access to controls in current room
* REST API
* Music that follows you through the rooms?
* Video streaming to TVs all over the network <sub>Steam In-home-streaming anyone? :D</sub>
* Voice recognition
* Printer sharing

### IDE Setup
* Install nodejs
* Run npm install
* Add `"main": "lib/AnyMesh.js",` to *node_modules/anymesh/package.json*