
![Logo](https://github.com/UTATRocketry/FlightAvionicsSystem/blob/main/FAS.png?raw=true)


# Flight Avionics System

Repository containing all firmware and drivers for the Flight Avionics System (FAS). The system comprises of 4 individual boards:
- **AVI_FAS_FMC:** Flight Management Computer (FMS) -  STM32H755ZI
    - Central computer responsible for processing data/telemetry, sending commands to and from all peripheral boards.
- **AVI_FAS_PMS:** Power Management System - STM32L4R5ZI
    - Supplies power to entire FAS, supplying various voltage levels for diffrent systems.
- **AVI_FAS_SPB:** Sensor peripheral Board: STM32L4R5ZI
    - Functions as data aquisition device for processing analog sensor data to be sent to FMC.
- **AVI_FAS_APB:** Actuator peripheral Board: STM32L4R5ZI
    - Board responsible with interfacing with onboard actuators (Servos, solenoids, relays, etc.) upon command of FMC
For more information FAS Hardware see [here]().

## Contributing

Bellow are the general steps to Contributing to the FAS project

1. **Fork the Repository**: Start by forking our repository to your GitHub account. This creates a copy of the project for you to work on.
2. **Clone the Repository**: Clone your forked repository to your local machine using `git clone`.\
3. **Create a Branch**: Before making any changes, create a new branch for your feature or bug fix. This keeps the main branch clean and allows for easier review. Use a descriptive name for your branch.
4. **Make Changes**: Now you're ready to make your changes! Write your code, add new features, or fix bugs. Ensure that your changes adhere to our coding standards and guidelines.
5. **Commit Changes**: Once you're satisfied with your changes, commit them to your branch using `git commit`. Be sure to write clear and concise commit messages.
6. **Push Changes**: Push your changes to your forked repository on GitHub using `git push`.
7. **Submit a Pull Request**: When you're ready for your changes to be reviewed and merged into the main project, submit a pull request. Provide a detailed description of the changes you've made and any relevant information for the reviewers.
8. **Code Review**: Your pull request will undergo a code review process. Be responsive to feedback and make any necessary changes.
9. **Merge**: Once your pull request has been approved and all discussions are resolved, it will be merged into the main project.

### Develompent Enviroment
The Development of the FAS firmware relies on the following development tools:
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
    -  Graphical tool that allows a very easy configuration of STM32 microcontrollers and microprocessors, as well as the generation of the corresponding initialization C code.
>[!Tip]
> It is recommended that contriubtors use [Visual Studio Code]() with the following extensions to aide development process:
>- [Serial Monitor](https://marketplace.visualstudio.com/items?itemName=ms-vscode.vscode-serial-monitor)
>- [stm32-for-vscode](https://marketplace.visualstudio.com/items?itemName=bmd.stm32-for-vscode)
>    - Be sure to install the dependancies specified by the extension
>- [RTOS Views](https://marketplace.visualstudio.com/items?itemName=mcu-debug.rtos-views)
>- [Embedded Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.vscode-embedded-tools)
>- [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)

### File Structure

```
├── AVI_FAS_SBP
├── AVI_FAS_FMC
│   ├── Core
|   |   ├── build
│   │   ├── CM4
│   │   ├── CM7
│   │   │   └── Core
│   │   │       ├── Inc
│   │   │       │   ├── FreeRTOSConfig.h...
│   │   │       │       \\Header files go here\\
│   │   │       └── Src
│   │   │           ├── freertos.c 
│   │   │           ├── main.c 
│   │   │               \\source files go here\\
│   │   ├── Common
│   │   ├── Drivers
│   │   ├── Middleware
│   │   ├── Makefile
│   │   ├── .mxproject
│   │   └── AVI_FAC_FMC.ioc
│   └── ...
└── README.md
```
### Building the Project
>[!Note]
> It is recommended to always open the indidual board directory in VSCode rather than the root `FlightAvionicsSystem` to ensure extensions function correctly and prevent issues during build.
#### AVI_FAS_FMC
As of March 2024, STM32-for-VSCode does not support dual core MCUs, thus, the project must be built manually. In order to build the project begin by opening a terminal at the following directory:
```
cd C:/.../AVI_FAS_FMC/Makfile
```
Run the makefile using the following command:
```
> make
```
Build files (.elf, binaries, etc.) should now be avilable for each core under
```
> cd C:/.../AVI_FAS_FMC/Makefile/CM4/build
```
For the Cortex-M4 and
```
> cd C:/.../AVI_FAS_FMC/Makefile/CM7/build
```
For the Cortex-M7
#### AVI_FAS_SPB, AVI_FAS_APB, AVI_FAS_PMS
The project can be built using the STM32-for-VSCode Extension by clicking build.

Alternatively, the project can be built manually using `make`.
In order to build the project begin by opening a terminal at the following directory:
```
cd C:/.../AVI_FAS_SPB
```
Run the makefile using the following command:
```
> make
```
Build files (.elf, binaries, etc.) should now be avilable under
```
> cd C:/.../AVI_FAS_FMC/build
```
### Flashing the Project
#### AVI_FAS_FMC
To be populated
#### AVI_FAS_SPB, AVI_FAS_APB, AVI_FAS_PMS
Simply selected the flash STM32 option with the board plugged in
