# Plant Monitoring System with Arduino and IoT

## Overview

The Plant Monitoring System is a project designed to monitor and manage the health of your plants using Arduino, I2C communication, and cloud connectivity. This system collects data such as temperature, humidity, and more from sensors and sends it to the cloud, allowing you to remotely access and control your plants' environment.

![Overview](images/Overview.png)

## Features

- Real-time monitoring of plant conditions.
- Cloud storage of plant data for historical tracking.
- Remote control of valves and water pumps.
- Mobile access through an IoT remote app.
- Customizable alerts and notifications.

## Hardware Components

- Arduino board (e.g., Arduino Uno, Arduino Mega).
- I2C sensors (e.g., temperature, humidity).
- Relay modules for valve and pump control.
- WiFi or Ethernet shield for internet connectivity.
- Water valves and pumps.
- Power supply for the system.

## Software Components

- Arduino IDE for code development.
- IoT remote app (e.g., Arduino Cloud, custom app).
- Cloud storage service (e.g., AWS, Azure).
- Libraries for Arduino sensor and communication protocols.

## Installation and Setup

1. Clone this repository to your local environment.
   ```sh
   git clone https://github.com/AndreiRaducanu/Plant-Monitoring-System.git
   ```
2. Set up the necessary hardware components and connect them to your Arduino board.
3. Install the required libraries in the Arduino IDE.
4. Configure the Arduino code with your WiFi/Ethernet credentials and cloud service settings.
5. Upload the code to your Arduino board.

## Usage

1. Power on the system.
2. Use the IoT remote app to monitor plant data and control valves/pumps.
3. Access the cloud platform to view historical plant data and trends.
4. Set up alerts and notifications for critical plant conditions.

## Contributing

Contributions to this project are welcome! Feel free to open issues, submit pull requests, or provide suggestions for improvements.

## License

This project is licensed under the [MIT License](LICENSE).
