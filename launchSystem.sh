rm /dev/mqueue/*
rm -rf Logs
./Common/bin/CommThread &
sleep 5
./Console/build-Console-Raspberry_Pi-Debug/Console

