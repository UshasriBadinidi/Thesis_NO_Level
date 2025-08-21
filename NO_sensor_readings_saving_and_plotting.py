import serial
import csv
import matplotlib.pyplot as plt

# === CONFIG ===
COM_PORT = 'COM5'         # Replace with your LoRa32 port
BAUD_RATE = 115200
CSV_FILE = 'sensor_data.csv'

# Open Serial
ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
plt.ion()  # Enable interactive mode for live plotting

# Prepare CSV file
with open(CSV_FILE, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['Timestamp(ms)','Vref_ADC','Vref_V','Vgas_ADC','Vgas_V'])

    time_list = []
    vref_list = []
    vgas_list = []

    try:
        while True:
            line = ser.readline().decode('utf-8').strip()
            if line and ',' in line:
                # Skip header line
                if line.startswith("Timestamp"):
                    continue

                parts = line.split(',')
                if len(parts) == 5:
                    # Extract values
                    timestamp = int(parts[0])/1000  # seconds
                    vref_volt = float(parts[2])
                    vgas_volt = float(parts[4])

                    # Save to CSV
                    writer.writerow(parts)

                    # Store for plotting
                    time_list.append(timestamp)
                    vref_list.append(vref_volt)
                    vgas_list.append(vgas_volt)

                    # Live plot
                    plt.clf()
                    plt.plot(time_list, vref_list, label='Vref (V)')
                    plt.plot(time_list, vgas_list, label='Vgas (V)')
                    plt.xlabel('Time (s)')
                    plt.ylabel('Voltage (V)')
                    plt.title('NO Sensor Live Readings')
                    plt.legend()
                    plt.grid(True)
                    plt.pause(0.01)
    except KeyboardInterrupt:
        print("Recording stopped.")
        plt.ioff()
        plt.show()
