extern crate ini;
use self::ini::Ini;
use std::str::FromStr;

const MAIN_CONF: &'static str = "config/smart_home.conf";
const GPIO_CONF: &'static str = "config/plugins/gpio.conf";

pub fn read_conf() {
    let conf = Ini::load_from_file(MAIN_CONF).unwrap();
    let general = conf.section(Some("General")).unwrap();
    let port = general.get("port").unwrap();

    let plugins = conf.section(Some("Plugins")).unwrap();
    for (plugin, _) in plugins.iter() {
        if plugin == "gpio" {//&& cfg!(feature = "gpio"){
            let gpio = Ini::load_from_file(GPIO_CONF).unwrap();
            for (sec, prop) in gpio.iter() {
                let section_name = sec.as_ref().unwrap();
                println!("-- Section: {:?} begins", section_name);
                for (k, v) in prop.iter() {
                    if k == "id" {
                        let id:&[u8] = FromStr::from_str(v).unwrap();
                        println!("{}/{}/{}", *v[0], *v[1], *v[2]);
                    }
                    println!("{}: {:?}", *k, *v);
                }
            }
            println!("Loaded Plugin 'GPIO'.");
        }
    }

    println!("{}", port);
}
