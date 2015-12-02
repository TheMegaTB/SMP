mod networking;
use networking::*;

fn main() {
    let sock = Connection::new().build();
    let rx = sock.recv_thread();

    for data in rx.iter() {     // Infinite iterator over incoming messages
        let (mut buf, src) = data;
        println!("RECV {}/{}/{} -> {}   @   {}", buf[0], buf[1], buf[2], buf[3], src);
        let buf = &mut buf[..4];
        buf.reverse();
        println!("SEND {}/{}/{} -> {}   @   {}", buf[0], buf[1], buf[2], buf[3], src);
        sock.send(&buf, src);
    }
}
