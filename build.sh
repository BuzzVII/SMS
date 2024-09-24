gcc -std=c11 -pedantic -o sms main.c
if [[ $? == 0 ]]; then
    ./sms
fi
