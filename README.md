<!-- PROJECT SHIELDS -->
<!-- REPO SHOULD BE PUBLIC TO SHOW THIS FEATURE!

[![Contributors]][contributors-url]

-->

<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/AlvaroMartinezQ/mini-kernel">
    <img src="imgs/logo.jpg" alt="Logo" width="150" height="150">
  </a>

  <h3 align="center">PARKING MPI</h3>

  <p align="center">
    OPERATIVE SYSTEMS 
    <br />
    <a href="https://github.com/AlvaroMartinezQ/mpi_parking"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/AlvaroMartinezQ/mpi_parking">View Demo</a>
    ·
    <a href="https://github.com/AlvaroMartinezQ/mpi_parking/issues">Report Bug</a>
    ·
    <a href="https://github.com/AlvaroMartinezQ/mpi_parking/issues">Request Feature</a>
  </p>
</p>

<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Contributing](#contributing)
* [Contact](#contact)
* [Usage](#usage)
* [Acknowledgements](#Acknowledgements)

<!-- ABOUT THE PROJECT -->
## About The Project

Parking simulation with MPI, development of operative systems, 2020 - 2021. 

### Built With

* [Linux](https://www.linux.org/)
* [C language](https://en.wikipedia.org/wiki/C_(programming_language))
* [MPI](https://www.open-mpi.org/)

<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

Technologies used to build the software and how to install them.

* Linux
* C compiler (usually included in any Linux/Ubuntu distribution)
* Git, install it directly from core

```sh
sudo apt-get update
```
```sh
sudo apt-get install git
```

* MPI (Message Passing Interface)

Executor parallel program(mpirun)
```sh
sudo apt-get install openmpi-bin
```
Debugger
```sh
sudo apt-get install xterm
```

### Installation

1. Make yourself a folder where you would like to work

2. Get a nice editor, VS code recommended

3. Clone the repo
```sh
git clone https://github.com/<github_username>/<repo_name>.git
```

<!-- CONTRIBUTING -->
## Contributing

Any new code should be pushed into a new branch. Please do not push any commits directly into the <strong>master</strong> branch! 

1. Create your Feature Branch (`git checkout -b feature-feature_name`)
2. Commit your Changes (`git commit -m 'feature: <description of the feature>'`)
3. Push to the Branch (`git push origin feature-feature_name`)
4. Open a Pull Request

<strong>If you want to merge branches follow these steps:</strong>

First, from the branch you want to merge:

1. Check updates (`git pull`)
2. Merge the main development branch you want FIRST into YOUR branch, in this case 'dev' (`git merge dev`) 
3. If no conflicts pop up, change to the main development branch (`git checkout dev`)

Second and last from the main development branch do as follows:

1. Check updates (`git pull`)
2. Merge the branch you finished working with (`git merge [branch_to_be_merged]`)
3. Push the new development branch to the repo (`git push`)
4. Delete your working branch from your local development enviroment (`git branch -d [branch to be deleted]`)
5. Delete your working branch from the repo (`git push -d origin [branch to be deleted]`)

<!-- LICENSE -->
<!--
## License
-->
<!-- CONTACT -->
## Contact

Alvaro Martinez Quiroga - a.martinezq.2017@alumnos.urjc.es


<!-- USAGE EXAMPLES -->
## Usage
In order to run .c files first configure your hostfile.config file

1. Create the file (if you don't have it)
```sh
touch hostfile.config
```

2. Add the number of nodes you want to run the program with, for example
```sh
localhost slots=4
```

3. Compile .c files into .o
```sh
mpicc [filename].c -o [filename].o
```

4. Run the nodes
```sh
mpirun --hostfile hostfile.config -np 4 [filename].o
```

5. If you want to run the debug
```sh
mpirun --hostfile hostfile.config -np 4 xterm -e gdb ./[filename].o
```

In order to run the code in each console, copy the following command
```sh
run
```

<!-- ACKNOWLEDGEMENTS -->

## Acknowledgements
TODO

<!-- MARKDOWN LINKS & IMAGES -->
<!-- REPO SHOULD BE PUBLIC TO SHOW THIS FEATURE!

[contributors-url]: https://github.com/AlvaroMartinezQ/mini-kernel/graphs/contributors

-->