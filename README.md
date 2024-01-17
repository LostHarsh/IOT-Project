# NodeMCU Control System

This project involves controlling relays on a NodeMCU microcontroller based on specified time intervals. The NodeMCU hosts a web server that allows clients to send time information and update the status of relays.

## NodeMCU Code

### Libraries Used
- **ESP8266WiFi**: Enables Wi-Fi connectivity on the NodeMCU.
- **ESPAsyncTCP**: Provides asynchronous TCP functionality.
- **ESPAsyncWebServer**: Allows the creation of an asynchronous web server on the NodeMCU.
- **time.h**: Supports time-related functions.

### Configuration
- Set the time zone and NTP server.
- Configure Wi-Fi credentials.
- Define static IP, gateway, and subnet for the NodeMCU.

### Pin Configuration
- `relayPin1`, `relayPin11`, `relayPin2`, `relayPin22`: Pins connected to relays.
- `BackwardLimit`, `ForwardLimit`: Pins representing limit switches.

### Timing Configuration
- `t1Duration`, `t2Duration`, `t3Duration`, `t4Duration`, `t5Duration`: Durations for various time intervals.
- Variables to track start times and timers for different stages.

### Server Initialization
- Establish Wi-Fi connection.
- Configure the server routes for basic requests and time updates.
- Start the server.

### Main Loop
- Check the current time and execute specific actions based on predefined time intervals.

## Client-Side HTML Code

### Structure
- Displays a table with information for two NodeMCU devices.
- Each row contains input fields for hour and minute, a button to send time information, and status indicators for relays.
- Buttons to manually update relay status.

### Scripting
- JavaScript functions for updating relay status, sending time information, and handling button clicks.
- Asynchronous requests to NodeMCU endpoints for status updates and time information.
- Display SVG images based on relay status.

### Styling
- Basic styling using CSS for layout and visual elements.
- Pre-loader to indicate page loading.

## Instructions for Use
1. **NodeMCU Setup:**
   - Flash the provided NodeMCU code onto the microcontroller.
   - Adjust configuration parameters such as Wi-Fi credentials, static IP, and time-related settings.

2. **Client-Side HTML:**
   - Open the HTML file in a web browser.
   - Input the desired hour and minute values for each NodeMCU device.
   - Click the "Send Time" button to schedule relay actions.
   - Use the "Update Status" buttons to manually check and update relay status.

3. **Interacting with NodeMCU:**
   - The NodeMCU will execute relay actions based on the scheduled times.
   - Relay status can be manually updated from the client-side.

**Note:** Ensure proper connectivity and IP addressing for communication between the client and NodeMCU. Adjustments may be necessary based on the specific hardware and network configuration.
