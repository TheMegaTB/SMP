#![allow(dead_code)]
extern crate net2;

use std::net::{ UdpSocket, Ipv4Addr, SocketAddr, SocketAddrV4 };
use std::thread;
use std::sync::mpsc;
use std::str::FromStr;
use self::net2::UdpSocketExt;

pub fn socket_addr_from_str(addr: &'static str) -> SocketAddr {
    SocketAddr::from_str(addr).ok().expect("Failed to parse socket addr.")
}

struct SmartHomeSocket {
    pub socket: UdpSocket
}

impl SmartHomeSocket {
    pub fn send(&self, data: &[u8], target: SocketAddr) -> usize {
        self.socket.send_to(data, target).ok().expect("Failed to send transmission")
    }

    pub fn send_thread(&self) -> mpsc::Sender<([u8; 4], SocketAddr)> {
        let (tx, rx) = mpsc::channel();
        let send_sock = self.clone();
        thread::Builder::new().name("send_thread".to_string()).spawn(move || {
            loop {
                let (data, addr): ([u8; 4], SocketAddr) = rx.recv().ok().expect("Failed to receive data.");
                send_sock.send(&data, addr);
            }
        }).ok().expect("Failed to spawn send_thread");
        tx
    }

    pub fn receive(&self) -> ([u8; 4], SocketAddr) {
        let mut buf = [0; 4];
        let src = self.socket.recv_from(&mut buf).ok().expect("Failed to receive package.").1;
        (buf, src)
    }

    pub fn recv_thread(&self) -> mpsc::Receiver<([u8; 4], SocketAddr)> {
        let (tx, rx) = mpsc::channel();
        let recv_sock = self.clone();
        thread::Builder::new().name("recv_thread".to_string()).spawn(move || {
            loop {
                let (buf, src) = recv_sock.receive();
                tx.send((buf, src)).ok().expect("Failed to send data to main thread.");
            }
        }).ok().expect("Failed to spawn recv_thread.");
        rx
    }

    pub fn clone(&self) -> SmartHomeSocket {
        SmartHomeSocket {
            socket: self.socket.try_clone().ok().expect("Failed to clone socket.")
        }
    }
}

pub struct Connection {
    local_addr: Ipv4Addr,
    multicast_addr: Ipv4Addr,
    port: u16
}

impl Connection {
    pub fn new() -> Connection {
        Connection {
            local_addr: Ipv4Addr::new(0, 0, 0, 0),
            multicast_addr: Ipv4Addr::new(233, 233, 233, 233),
            port: 8000
        }
    }

    pub fn port(mut self, port: u16) -> Connection {
        self.port = port;
        self
    }

    pub fn local_addr(mut self, ip: &'static str) -> Connection {
        self.local_addr = FromStr::from_str(&ip).ok().expect("Failed to resolve IP.");
        self
    }

    pub fn multicast_addr(mut self, ip: &'static str) -> Connection {
        self.multicast_addr = FromStr::from_str(&ip).ok().expect("Failed to resolve IP.");
        self
    }

    pub fn build(&mut self) -> SmartHomeSocket {
        let sock_addr = SocketAddrV4::new(Ipv4Addr::new(0, 0, 0, 0), self.port);
        let sock = UdpSocket::bind(sock_addr).unwrap();
        sock.join_multicast_v4(&self.multicast_addr, &self.local_addr).ok().expect("Failed to join multicast.");
        SmartHomeSocket {
            socket: sock
        }
    }
}
