# SMP | State monitoring protocol
This is a protocol to monitor and control an arbitrary amount of IoT devices.
The repo contains an implementation of this protocol that houses a plugin based server but the protocol is not restricted
to it. Any device can implement just as many or as few parts of the protocol it needs.

## Philosophy

Now since many protocols out there do not provide an easy to use API, are designed around a specific task or simply
not following some design goals (read on for more on that) this project aims to fix some common potholes in the IoT realms. 

### Modularity
The problem of IoT applications is that it only makes sense as a whole. It is not reasonable to replace the lightbulb
in a single room with a 'smart' one and take apart all light switches in that room just to make your home that much more
smart. Actually, it doesn't even get that much smarter by doing so. In the time you took your phone out of your pocket,
opened the App you need for this specific type of bulb, searched for the room your bulb is in and pressed that touchy
button you could've walked up to the light switch a dozen times. That is far from being "smart" and closer to a simple
waste of money there. It is not about that individual light bulb making your house smart, it's about the composition of
ideas that come to people's minds and combining them with devices that provide data to fuel these ideas with life.
Take the light bulb you just eagerly bought in a hope for a smarter living as an example. Wouldn't it be great if that
bulb could react to your position, cloud cover, window orientation, sunlight intensity and many more properties just to
turn on in that very moment you would get up and walk to your light switch or fiddle around in the app?

That's what this is all about. This protocol itself is making almost no restrictions on what a device has to feature in 
order to work with the other ones. The only requirement set is that the devices are capable of supporting [RFC 1112](https://www.ietf.org/rfc/rfc1112.txt) or in
other terms UDP multicast and very basic JSON parsing (string search or regex would do in that case).

### What about security?
_First off let me mention that is is still WIP so no full support on this just yet with the current implementation._

Good question there! Unlike other protocols that aim to provide control over your very house, this project aims to provide
the opt-in ability to protect all your devices by fully encrypting all traffic between them. This is achieved by utilizing
asymmetric encryption algorithms. If you add a new device it will simply not be able to understand what the others are
chatting about until you provide it with an access token from any of the other devices.

Now the question might arise how on earth this prevents somebody evil that gained access to one of your devices from
getting control over the whole network. Let's get back to the previous example. Assuming you've got a new device, that
has just been integrated into the network and you provided it with the aforementioned tokens. The device can now read
the messages everybody else is sending. In other terms: It authorized all other devices to send messages to himself.
Now the device can read from all other devices <strong>but</strong> the other devices can not read from this new device since they
didn't authorize it yet. In order to do this, you need to copy over the device token to all other devices.

This provides additional safety since the other devices are simply not capable of reading the messages from the devices
they have not authorized (aka they do not have a token from). This might sound like a huge hassle having to copy over
every single token from every device to every other device but keep listening now it's starting to get interesting.

The whole system incorporates a so-called 'RAVS' or 
<strong>R</strong>emote<strong>A</strong>uthentication<strong>V</strong>ersioning<strong>S</strong>ystem which enables you to
add and remove devices to and from the network. It provides control over the process of token distribution across the
network. Now since this punches a huge hole into the great great wall of the previously explained security system if the
possible attacker get's access to any device that's already part of the network and is capable of utilizing RAVS, access
to the RAVS API is restricted to a specific set of devices (which by themselves can add new devices to this set so you
shall better keep them secured) which can be configured at the setup time of a node or fetched from an already existing
node.

Now that we've got that out of the way you should keep in mind that this system is solely a definition on what devices
should implement. It is not enforced by any means. On the one hand, this enables very simple devices like the [FINDTracker](https://github.com/TheMegaTB/FINDTracker)
to only implement the device token feature, encrypting it's broadcasts with it, leaving out everything else since it is not
required for that device. On the other hand, though this means that you have to be extra careful when adding new devices
that might not implement this security protocol. On top of that this is all opt-in. It's just another module in the wall.

## That's great and all but how can I use it?!
Actually, you can't. At least not directly. Everything you just read is just a description of the features that are shaping this
protocol. This protocol by itself is useless if nothing utilizes it, so you have to get your hands on a device or software
that makes use of the SMP stack.

### Plugin server (Linux)
Luckily enough there are already some projects that do utilize it including the code in this repository, which is a
plugin based server for Linux `server/`, a plugin library `plugin-library/` and some example plugins that you can use
`plugins/`. In order to start the server run the following commands:
``` bash
    git clone https://github.com/TheMegaTB/SMP.git && cd SMP
    mkdir build && cd build
    cmake ../ && make
    
    # You may change the CONFIG_DIR option here. It is just for reference w/ the example configs provided.
    CONFIG_DIR=../conf PLUGIN_DIR=./            \
        ./server/smp-server
```

### Other projects
* [FINDTracker](https://github.com/TheMegaTB/FINDTracker) — Indoor positioning

If your project supports the SMP protocol please open up an [issue](https://github.com/TheMegaTB/SMP/issues) or create
a pull request with an appropriate description in the list.