rm /dev/mqueue/*
rm -rf Logs
./Common/bin/CommThread &
sleep 1
./Console/build-Console-Raspberry_Pi-Debug/Console

