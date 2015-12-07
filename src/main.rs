mod networking;
use networking::*;
mod plugins;
//mod config;
//use config::read_conf;

fn main() {
    //read_conf();
    let sock = Connection::new().build();
    let rx = sock.recv_thread();

    let mut plugin_tx = Vec::with_capacity(1);
    plugin_tx.push(plugins::gpio::new_listener());


    println!("LISN {}", sock.socket.local_addr().unwrap());
    for data in rx.iter() {     // Infinite iterator over incoming messages
        //let (mut buf, src) = data;
        println!("RECV {}/{}/{} -> {}   @   {}", data.0[0], data.0[1], data.0[2], data.0[3], data.1);

        for tx in plugin_tx.iter() { tx.send(data).unwrap() }

        // let buf = &mut buf[..4];
        // buf.reverse();
        // println!("SEND {}/{}/{} -> {}   @   {}", buf[0], buf[1], buf[2], buf[3], src);
        // sock.send(&buf, src);
    }
}
