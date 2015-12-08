extern crate networking;
extern crate rand;
use networking::*;
use std::env;
use std::str::FromStr;

fn main() {     // TODO: Convert this to a VALID working implementation test.
    let sock = Connection::new().port(0).build();
    let tx = sock.send_thread();
    let rx = sock.recv_thread();

    let mut data: [u8; 4] = [0; 4];

    for (i, x) in env::args().enumerate() {
        if i > 0 {
            data[i-1] = FromStr::from_str(x.trim()).unwrap_or(0)
        };
    }

    println!("SEND {}/{}/{} -> {}   @   233.233.233.233:8000", data[0], data[1], data[2], data[3]);
    tx.send((data, socket_addr_from_str("233.233.233.233:8000"))).unwrap();

    for d in rx.iter() { //TODO: Resend datagram if the correct echo is not being received after a certain timeout
        println!("RECV {} <- {}\\{}\\{}   @   {}", d.0[0], d.0[1], d.0[2], d.0[3], d.1);
        let mut x = d.0;
        x.reverse();
        if x == data {
            println!("RECEIVED REVERSE ECHO!");
            break;
        }
    }
}
