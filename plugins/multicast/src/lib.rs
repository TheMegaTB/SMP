extern crate shared_objects;
extern crate net2;

// use net2::UdpSocketExt;
use std::net::{ UdpSocket, Ipv4Addr, SocketAddrV4 };

use shared_objects::{ PluginType, Return, Action, DID };

const MULTICAST_ADDR: &'static str = "233.233.233.233";
const MULTICAST_PORT: u16 = 8000;
const BUFFER_SIZE: usize = 1024;

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
pub fn listen(_res: Return<Action>) {
    // TODO: Ignore packages from local socket (ip+port) as it would result in a endless loop
    std::thread::spawn(move || {
        let _x = _res; // Don't you deallocate it!
        loop {
            let sock = create_multicast_socket(Some(MULTICAST_PORT));
            let mut buf = vec![0; BUFFER_SIZE];
            let (len, _src) = sock.recv_from(&mut buf).unwrap();
            buf.truncate(len);
            for byte in buf.iter() {
                println!("Value: {:?}", byte);
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
    println!("MULTICAST: Executing {:?}", action);
    let sock = create_multicast_socket(None);
    let multicast_addr: Ipv4Addr = MULTICAST_ADDR.parse().unwrap();
    sock.send_to(&[0, 1, 2, 3], (multicast_addr, MULTICAST_PORT)).unwrap();
}
