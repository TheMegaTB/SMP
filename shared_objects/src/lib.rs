#![warn(missing_docs)]
//! Shared objects for smartHome that are required in both the plugins and the host

/// Type of a plugin
#[derive(Debug)]
pub enum PluginType {
    /// Passive receiver that interacts with the real world e.g. turns lights on/off
    Actor,
    /// Active component that provides inputs that are then processed by an actor
    Input
}

/// Type of a specific Device
#[derive(Debug, PartialEq, Eq)]
pub enum DeviceType {
    /// A generic lamp
    Light,
    /// A generic window shutter
    Shutter
}

impl ToString for DeviceType {
    fn to_string(&self) -> String {
        match self {
            &DeviceType::Light => "Light".to_string(),
            &DeviceType::Shutter => "Shutter".to_string()
        }
    }
}

/// Unique identifier of a device
#[derive(Debug, PartialEq, Eq)]
pub struct DID {
    /// Room in which the device rests
    pub room: &'static str,
    /// Type of the device
    pub dtype: DeviceType,
    /// ID of the device
    pub id: usize
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
