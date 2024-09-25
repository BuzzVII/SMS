gcc -std=c11 -pedantic -Iraylib-5.0_linux_amd64/include/ -lm -o sms main.c raylib-5.0_linux_amd64/lib/libraylib.a
if [[ $? == 0 ]]; then
    ./sms
fi
