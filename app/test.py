from serial import Serial

Serial = Serial(port="COM18", baudrate=9600)



while True:
    a, b = map(int, input().split())
    print("Sending...")
    Serial.write(bytes([a]))
    Serial.write(bytes([b]))
    print("Sent!")
