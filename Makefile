compile:
	arduino-cli compile -b esp32:esp32:esp32s3 ./esp_web_server/esp_web_server.ino

upload:
	arduino-cli upload -b esp32:esp32:esp32s3 -p /dev/ttyACM0 ./esp_web_server/esp_web_server.ino

monitor:
	arduino-cli monitor -b esp32:esp32:esp32s3 -p /dev/ttyACM0 --config baudrate=115200

all:
	make compile && make upload && make monitor

get-board:
	arduino-cli board list
