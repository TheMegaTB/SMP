use std::thread;
use std::sync::mpsc;
use std::net::SocketAddr;
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
    let (cb, cb_rx): (mpsc::Sender<([u8; 4], SocketAddr)>, _) = mpsc::channel();
    if cfg!(feature = "gpio") { plugin_tx.push(plugins::gpio::new_listener(cb.clone())) }
    if cfg!(feature = "knx") { plugin_tx.push(plugins::knx::new_listener(cb.clone())) }


    println!("LISN {}", sock.socket.local_addr().unwrap());

    thread::Builder::new().name("callback_listener".to_string()).spawn(move || {
        for data in cb_rx.iter() {
            print!("Sending reverse echo...");
            sock.send_confirmation(data);
        }
    }).unwrap();

    let plugin_invocator = thread::Builder::new().name("plugin_invocator".to_string()).spawn(move || {
        for data in rx.iter() {
            println!("RECV {}/{}/{} -> {}   @   {}", data.0[0], data.0[1], data.0[2], data.0[3], data.1);

            for tx in plugin_tx.iter() { tx.send(data).unwrap() }
        }
    }).unwrap();

    plugin_invocator.join().unwrap();
}
