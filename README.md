# Bela-Spectrogram
A C++ Implementation of a spectrogram to run on (but not constrained to) the Bela Mini (https://bela.io) 



# Install

To `make` the project, clone it

```bash
git clone git@github.com:alexandrusoloms/Bela-Spectrogram.git
```

`cd` into it:

```bash
cd Bela-Spectrogram/
```

create a directory called build:

```bash
mkdir build
```

`cd` into build:

```bash
cd build/
```

run `cmake` on the previous directory:

```bash
cmake ..
```

run `make` in the current directory (build):

```bash
make
```

# Example:

```bash
./Bela-Spectrogram fileName.wav outputFileName
```



# TODO:

- Thank <https://rosettacode.org/wiki/Fast_Fourier_transform> for FFT code
- Thank <https://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file> for reading wav file code

