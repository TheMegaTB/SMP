#![feature(custom_derive, plugin)]
#![plugin(serde_macros)]
extern crate shared_objects;
extern crate net2;
extern crate bincode;

use bincode::serde::*;
use bincode::SizeLimit;

use std::net::{ UdpSocket, Ipv4Addr, SocketAddrV4 };

use shared_objects::{ PluginType, Return, Action, DID };

const MULTICAST_ADDR: &'static str = "233.233.233.233";
const MULTICAST_PORT: u16 = 8000;
const BUFFER_SIZE: usize = 1024;

static mut LOCAL_PORT: u16 = 0;

#[no_mangle]
pub fn get_type(res: Return<PluginType>) {
    res.send_all(vec![PluginType::Input, PluginType::Actor]);
}

pub fn create_multicast_socket(port: Option<u16>) -> UdpSocket {
    let sock_addr = SocketAddrV4::new(Ipv4Addr::new(0, 0, 0, 0), port.unwrap_or(0));
    let sock = UdpSocket::bind(sock_addr).unwrap();
    let multicast_addr: Ipv4Addr = MULTICAST_ADDR.parse().unwrap();
    sock.join_multicast_v4(&multicast_addr, sock_addr.ip()).ok().expect("Failed to join multicast.");
    sock
}

#[no_mangle]
pub fn listen(res: Return<Action>) {
    std::thread::spawn(move || {
        let sock = create_multicast_socket(Some(MULTICAST_PORT));
        let mut buf = vec![0; BUFFER_SIZE];
        loop {
            let (len, src) = sock.recv_from(&mut buf).unwrap();
            if unsafe { src.port() != LOCAL_PORT } { //TODO: Get local ip and ignore packages from it (as it is possible that other devices have the same port)
                buf.truncate(len);
                let action: Action = deserialize(&buf).unwrap();
                res.send(action);
            } else {
                unsafe { LOCAL_PORT = 0 }
            }
        }
    });
}

#[no_mangle]
pub fn get_responsibility(_: Return<DID>) {
    // Sending nothing gets interpreted as a wildcard
}

#[no_mangle]
pub fn execute(action: Action) {
    let sock = create_multicast_socket(None);
    unsafe { LOCAL_PORT = sock.local_addr().unwrap().port() }
    let multicast_addr: Ipv4Addr = MULTICAST_ADDR.parse().unwrap();
    let serialized_action = serialize(&action, SizeLimit::Infinite).unwrap();
    sock.send_to(&serialized_action, (multicast_addr, MULTICAST_PORT)).unwrap();
}
