# BruteSSH
brutessh is a simple commandline program written in C++ for GNU/linux to make dictionary attacks to an SSH server

![image](https://raw.githubusercontent.com/CaptainLainist/brutessh/main/test.png)

## Instalation
to install it, just download the brutessh binary and place it in your path

```bash
sudo cp brutessh /bin/brutessh
```

then install those libraries

```bash
sudo apt update
sudo apt install libc6-dev
sudo apt install libssh-dev
```

## About the timeout

this software includes a timeout variable that timeouts the connections since a failed connection will be slower than a correct user/pass connection

be aware that an incredibly low timeout will fail even with the correct user/pass connection, or it wont stop even after it says that login is found

## Warning

I don't make myself responsible for every illegal usage of this software, remember to use pentesting for the good.
