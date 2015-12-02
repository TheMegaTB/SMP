extern crate networking;
extern crate rand;
use networking::*;
use std::thread;
use rand::Rng;

fn main() {     // TODO: Convert this to a VALID working implementation test.
    let sock = Connection::new().port(8001).build();
    let rx = sock.recv_thread();
    let tx = sock.send_thread();

    let rx_thread = thread::spawn(move || {
        for _ in 0..20 {     // Infinite iterator over incoming messages
            let (buf, src) = rx.recv().ok().expect("Failed to receive results!");
            println!("RECV {}/{}/{} -> {}   @   {}", buf[0], buf[1], buf[2], buf[3], src);
        }
    });

    let target = socket_addr_from_str("233.233.233.233:8000");
    let r = rand::thread_rng().gen::<[u8; 4]>();
    println!("SEND {}/{}/{} -> {}   @   233.233.233.233:8000", r[0], r[1], r[2], r[3]);
    tx.send((r, target)).unwrap();

    rx_thread.join().unwrap();
}
