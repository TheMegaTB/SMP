extern crate networking;
extern crate rand;
use networking::*;
use std::io;
use std::str::FromStr;

fn main() {     // TODO: Convert this to a VALID working implementation test.
    let sock = Connection::new().port(0).build();
    let tx = sock.send_thread();
    let rx = sock.recv_thread();

    let mut input = String::new();
    println!("Enter the ID and value seperated by /'s: ");
    io::stdin().read_line(&mut input)
        .ok()
        .expect("failed to read line");

    let input = input.split("/");
    let mut data: [u8; 4] = [0; 4];
    for (i, x) in input.enumerate() {
        data[i] = FromStr::from_str(x.trim()).unwrap_or(0);
    }

    println!("SEND {}/{}/{} -> {}   @   233.233.233.233:8000", data[0], data[1], data[2], data[3]);
    tx.send((data, socket_addr_from_str("233.233.233.233:8000"))).unwrap();

    for data in rx.iter() { println!("RECV {}/{}/{} -> {}   @   {}", data.0[0], data.0[1], data.0[2], data.0[3], data.1) }
}
