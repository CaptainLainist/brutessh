# BruteSSH
brutessh is a simple commandline program written in C++ for GNU/linux to make dictionary attacks to an SSH server

![](https://raw.githubusercontent.com/CaptainLainist/brutessh/main/test.png)

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

this software includes a timeout variable that timeouts the connections since a failed connection will be slower than a correct user/pass connection

be aware that an incredibly low timeout will fail even with the correct user/pass connection, or it wont stop even after it says that login is found
