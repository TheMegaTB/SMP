//#![warn(missing_docs)]
//! Shared objects for smartHome that are required in both the plugins and the host
#![feature(custom_derive, plugin)]
#![plugin(serde_macros)]

extern crate bincode;

use std::sync::mpsc;

/// Type of a plugin
#[derive(Debug, PartialEq)]
pub enum PluginType {
    /// Passive receiver that interacts with the real world e.g. turns lights on/off
    Actor,
    /// Active component that provides inputs that are then processed by an actor
    Input
}

pub type ID = u8;

/// Type of a specific Device
#[derive(Debug, PartialEq, Eq, Hash, Clone, Serialize, Deserialize)]
pub enum DeviceType {
    /// A generic lamp
    Light,
    /// A generic window shutter
    Shutter,
    /// Unknown device
    Unknown
}

impl ToString for DeviceType {
    fn to_string(&self) -> String {
        match self {
            &DeviceType::Light => "Light".to_string(),
            &DeviceType::Shutter => "Shutter".to_string(),
            &DeviceType::Unknown => "Unknown".to_string()
        }
    }
}

/// Unique identifier of a device
#[derive(Debug, PartialEq, Eq, Hash, Clone, Serialize, Deserialize)]
pub struct DID {
    /// Room in which the device rests
    pub room: ID,
    /// Type of the device
    pub dtype: DeviceType,
    /// ID of the device
    pub id: ID
}

impl ToString for DID {
    fn to_string(&self) -> String {
        format!("{}/{}/{}", self.room.to_string(), self.dtype.to_string(), self.id)
    }
}

impl PartialEq<String> for DID {
    fn eq(&self, other: &String) -> bool {
        self.to_string() == *other
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum StateValue {
    On,
    Value(u8),
    Off
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum State {
    Automatic(StateValue),
    Manual(StateValue)
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Action {
    pub change: State,
    pub target: DID
}

pub struct Return<T: Sized> {
    sender: mpsc::Sender<T>
}

impl<T: Sized> Return<T> {
    pub fn send(&self, val: T) {
        self.sender.send(val).unwrap();
    }

    pub fn send_all(&self, items: Vec<T>) {
        for item in items.into_iter() {
            self.send(item);
        }
    }

    pub fn return_val(self, val: T) {
        self.send(val);
        drop(self);
    }
}

pub struct ReturnValue<T: Sized> {
    receiver: mpsc::Receiver<T>
}

impl<T: Sized> ReturnValue<T> {
    pub fn new() -> (Return<T>, ReturnValue<T>) {
        let (tx, rx) = mpsc::channel();
        (Return {
            sender: tx
        }, ReturnValue {
            receiver: rx
        })
    }

    pub fn receive(&self) -> Result<T, mpsc::TryRecvError> {
        self.receiver.try_recv()
    }

    pub fn receive_all(self) -> Vec<T> {
        let mut res = Vec::new();
        loop {
            match self.receiver.recv() {
                Ok(val) => {
                    res.push(val);
                },
                Err(_) => { break; }
            }
        }
        res
    }
}
