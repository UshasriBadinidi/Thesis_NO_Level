import serial
import csv

# --- CONFIGURE SERIAL PORT ---
COM_PORT = 'COM5'          # Change if needed
BAUD_RATE = 115200
CSV_FILE = 'sensor_data_from_lora.csv'

# Open serial port
ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)

# Open CSV file for writing
with open(CSV_FILE, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)

    try:
        while True:
            line = ser.readline().decode('utf-8').strip()
            if line:  # skip empty lines
                print(line)  # optional: see live readings
                parts = line.split(',')  # split CSV columns
                writer.writerow(parts)

    except KeyboardInterrupt:
        print("\nStopped by user. Data saved to", CSV_FILE)
        ser.close()
