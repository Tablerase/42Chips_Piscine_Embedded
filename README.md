# 42Chips_Piscine_Embedded

<img src="https://images.squarespace-cdn.com/content/v1/65d8c36ecf88631d9355461f/5d162657-c8b5-4353-a895-639e3141f510/logo42chips.png" width="200" height="200" align="right" />

## Resources

- [AVR-Microcontroller-ATmega328P](https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf)
- [Calculator](https://www.rapidtables.com/convert/number/decimal-to-binary.html?x=16)

## Config

### VSCode

- [C/C++ Config]

```vscode
@ext:ms-vscode.cpptools
```

```json
{
  "configurations": [
    {
      "name": "linux-avr-gcc",
      "includePath": ["${workspaceFolder}/**", "/usr/lib/avr/include/**"],
      "defines": [],
      "mergeConfigurations": false,
      "compilerPath": "/usr/bin/avr-gcc",
      "cStandard": "gnu11",
      "cppStandard": "gnu++17",
      "intelliSenseMode": "linux-gcc-x64",
      "compilerArgs": ["-mmcu=atmega328p", "-DF_CPU=16000000UL", "-Os"],
      "browse": {
        "path": ["${workspaceFolder}"],
        "limitSymbolsToIncludedHeaders": true
      }
    }
  ],
  "version": 4
}
```

## Information

### AVR Programming

- [AVR Programming](https://www.instructables.com/AVR-Programming-with-Arduino-AVRdude-and-AVR-gcc/)

- AVR stands for Alf and Vegard's RISC processor
- `avr-gcc` is a compiler that takes C code and turns it into machine code that the AVR microcontroller can execute
  - `mmcu=atmega328p` specifies the microcontroller model
  - `-Os` optimizes the code for size
  - `-DF_CPU=16000000UL` specifies the clock speed of the microcontroller
  - `-o` specifies the output file

```mermaid
flowchart LR
    subgraph ELF["ELF File (.elf)"]
        direction TB
        E1["• Machine Code
        • Debug Info
        • Symbol Tables
        • Relocation Data"]
    end

    subgraph Process["Conversion Process"]
        direction TB
        P1["avr-objcopy
        -O ihex
        -R .eeprom"]
    end

    subgraph HEX["HEX File (.hex)"]
        direction TB
        H1["• Pure Binary Code
        • Memory Addresses
        • Program Data Only"]
    end

    ELF --> Process --> HEX

    style ELF fill:#e6f3ff,stroke:#333,stroke-width:2px,color:#000000
    style Process fill:#fff,stroke:#333,stroke-width:2px,color:#000000
    style HEX fill:#f5f5f5,stroke:#333,stroke-width:2px,color:#000000
```

- ELF (Executable and Linkable Format) is a standard file format for executables, object code, shared libraries, and core dumps
- HEX (Intel Hexadecimal Object File Format) is a file format that conveys binary information in ASCII text form
- `avr-objcopy` is a utility program that copies only the parts of the object files that are needed
  - `-O ihex` specifies the output format as Intel HEX
  - `-R .eeprom` removes the EEPROM data from the HEX file
    - EEPROM (Electrically Erasable Programmable Read-Only Memory) is a type of non-volatile memory used in computers and other electronic devices to store small amounts of data that must be saved when power is removed
- `avrdude` is a utility program that ahttps://www.rapidtables.com/convert/number/decimal-to-binary.html?x=16

#### AVR Libraries

##### AVR I/O

- `avr/io.h` is a library that provides access to the I/O registers of the AVR microcontroller
- I/O registers are memory locations that control the behavior of the microcontroller
  - I/O registers are used to configure the pins of the microcontroller as inputs or outputs, read the state of the pins, and write data to the pins

```mermaid
flowchart LR
    classDef main fill:#1976d2,stroke:#01579b,stroke-width:2px,color:#ffffff
    classDef included fill:#6a1b9a,stroke:#4a148c,stroke-width:2px,color:#ffffff
    classDef purpose fill:#ffffff,stroke:#616161,stroke-width:1px,color:#000000,text-align:left

    io["<b>avr/io.h</b><br/><i>Main Header File</i>"]:::main

    subgraph Included_Files["Included Components"]
        direction TB
        sfr["<b>sfr_defs.h</b><br/>Special Function Registers"]:::included
        pins["<b>portpins.h</b><br/>Port Pin Definitions"]:::included
        common["<b>common.h</b><br/>Common Register Names"]:::included
        version["<b>version.h</b><br/>Version Information"]:::included
    end

    subgraph Purposes["Key Functions"]
        direction TB
        reg["Register Access<br/>• Direct register manipulation<br/>• Bit-level operations"]:::purpose
        def["Definitions<br/>• Device-specific mappings<br/>• Interrupt handling"]:::purpose
        prot["Protection<br/>• CCP mechanism support<br/>• Protected register writes"]:::purpose
    end

    io --> Included_Files
    io --> Purposes
```

#### AVR I/O Registers

##### IO Ports

| Register | Description             | Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| -------- | ----------------------- | ------- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- |
| DDRx     | Data Direction Register | 0x0A    | D7    | D6    | D5    | D4    | D3    | D2    | D1    | D0    |
| PORTx    | Port Output Register    | 0x0B    | P7    | P6    | P5    | P4    | P3    | P2    | P1    | P0    |
| PINx     | Port Input Register     | 0x09    | I7    | I6    | I5    | I4    | I3    | I2    | I1    | I0    |

```mermaid
flowchart LR
    subgraph "Port Configuration"
        DDR["DDRx Register<br/><i>Direction Control</i><br/>0 = Input<br/>1 = Output"]
    end

    subgraph "Output Control"
        PORT["PORTx Register<br/><i>Output Driver</i><br/>0 = Low Level<br/>1 = High Level"]
    end

    subgraph "Pin Monitoring"
        PIN["PINx Register<br/><i>Physical Pin State</i><br/>Read-only"]
    end

    DDR --> |"Controls"| PORT
    PORT --> |"Drives"| PIN
    PIN --> |"Reflects"| PIN

    style DDR fill:#ffcdd2,stroke:#c62828,color:#000000
    style PORT fill:#bbdefb,stroke:#1976d2,color:#000000
    style PIN fill:#c8e6c9,stroke:#388e3c,color:#000000
```

- DDRx (Data Direction Register)
  - Controls whether each pin acts as input (0) or output (1)
  - Bit n controls pin n (e.g., DDRB0 controls PORTB0)
  - Writing 1 sets the corresponding pin as output
  - Writing 0 sets the corresponding pin as input
- PORTx (Port Register)
  - Controls the output value when a pin is configured as output
  - Can also enable/disable internal pull-up resistors for input pins
  - Bit n controls the output driver for pin n
- PINx (Pin Register)
  - Read-only register showing actual pin states
  - Always reflects the physical state of the pins
  - Bit n reads the state of pin n

[DDR-PORT-PIN](http://embeddedwithanshul.blogspot.com/2012/06/registers-ddr-port-pin.html)

#### AVR Timers

- Timers are used to generate precise time delays, measure time intervals, and control the frequency of waveforms
- Timers are implemented using Timer/Counter Registers
  - Timer/Counter Registers are memory locations that control the behavior of the timers
  - Timer/Counter Registers are used to configure the timers, read the current count value, and write new count values

##### Timer/Counter Registers

| Register | Description                  | Address | Bit 7  | Bit 6  | Bit 5  | Bit 4  | Bit 3  | Bit 2  | Bit 1  | Bit 0  |
| -------- | ---------------------------- | ------- | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
| TCCRnA   | Timer/Counter Control Reg A  | 0x2n    | COMnA1 | COMnA0 | COMnB1 | COMnB0 | -      | -      | WGMn1  | WGMn0  |
| TCCRnB   | Timer/Counter Control Reg B  | 0x2n+1  | FOCnA  | FOCnB  | -      | -      | WGMn2  | CSn2   | CSn1   | CSn0   |
| TCNTn    | Timer/Counter Register       | 0x2n+2  | TCNTn7 | TCNTn6 | TCNTn5 | TCNTn4 | TCNTn3 | TCNTn2 | TCNTn1 | TCNTn0 |
| OCRnA    | Output Compare Reg A         | 0x2n+3  | OCRnA7 | OCRnA6 | OCRnA5 | OCRnA4 | OCRnA3 | OCRnA2 | OCRnA1 | OCRnA0 |
| OCRnB    | Output Compare Reg B         | 0x2n+4  | OCRnB7 | OCRnB6 | OCRnB5 | OCRnB4 | OCRnB3 | OCRnB2 | OCRnB1 | OCRnB0 |
| ICRn     | Input Capture Register       | 0x2n+4  | ICRn7  | ICRn6  | ICRn5  | ICRn4  | ICRn3  | ICRn2  | ICRn1  | ICRn0  |
| TIFRn    | Timer/Counter Interrupt Flag | 0x3n    | -      | -      | -      | -      | -      | -      | OCFnA  | OCFnB  |

- `n` is the timer number (e.g., Timer0, Timer1, Timer2)
- `COMnA1` and `COMnA0` control the output behavior of the `OCnA` pin
- `COMnB1` and `COMnB0` control the output behavior of the `OCnB` pin
- `WGMn2`, `WGMn1`, and `WGMn0` control the waveform generation mode
- `FOCnA` and `FOCnB` are used to force an output compare match
- `CSn2`, `CSn1`, and `CSn0` control the clock source
- `TCNTn` is the timer/counter register that holds the current count value
- `OCRnA` and `OCRnB` are the output compare registers that hold the comparison values
- `ICRn` is the input capture register that holds the capture value
- `OCFnA` and `OCFnB` are the output compare match flags

###### Prescaler

- The prescaler is a counter that divides the clock frequency before it reaches the timer
- The prescaler is used to reduce the clock frequency to a manageable level

  - This allows the timer to count at a slower rate
  - This prevents the timer from overflowing too quickly
    - Example: Timer1 is an 16-bit counter that can count up to $2^{16} = 65,536$
      - $F_{timer} = \frac{System\ clock}{Prescaler}$
      - $F_{timer} = \frac{16\ \text{MHz}}{1024} = 15.625\ \text{KHz}$
      - $T_{tick} = \frac{1}{15.625\ \text{KHz}} = 64\ \mu s$
      - $T_{total} = 64\mu s \times 65,536 = 4.194\ \text{s}$
      - The timer will overflow every 4.194 seconds

- The prescaler is controlled by the CSn2, CSn1, and CSn0 bits in the TCCRnB register

| CSn2 | CSn1 | CSn0 | Prescaler | Description      |
| ---- | ---- | ---- | --------- | ---------------- |
| 0    | 0    | 0    | 0         | No prescaling    |
| 0    | 0    | 0    | 1         | $clk_{I/O}$      |
| 0    | 0    | 1    | 8         | $clk_{I/O}$/8    |
| 0    | 1    | 0    | 64        | $clk_{I/O}$/64   |
| 0    | 1    | 1    | 256       | $clk_{I/O}$/256  |
| 1    | 0    | 0    | 1024      | $clk_{I/O}$/1024 |

```mermaid
flowchart TB
    subgraph "Timer Configuration"
        TCCR["TCCRnA/B Registers<br/><i>Control Settings</i>"]
    end

    subgraph "Timer Counter"
        TCNT["TCNTn Register<br/><i>Counter Value</i>"]
    end

    subgraph "Output Compare"
        OCR["OCRnA/B Registers<br/><i>Comparison Value</i>"]
    end

    subgraph "Input Capture"
        ICR["ICRn Register<br/><i>Capture Value</i>"]
    end

    subgraph "Interrupt Flags"
        TIFR["TIFRn Register<br/><i>Interrupt Flags</i>"]
    end

    TCCR --> |"Control"| TCNT
    TCCR --> |"Control"| OCR
    TCCR --> |"Control"| ICR
    TCCR --> |"Control"| TIFR

    style TCCR fill:#ffcdd2,stroke:#c62828,color:#000000
    style TCNT fill:#bbdefb,stroke:#1976d2,color:#000000
    style OCR fill:#c8e6c9,stroke:#388e3c,color:#000000
    style ICR fill:#dcedc8,stroke:#689f38,color:#000000
    style TIFR fill:#f0f4c3,stroke:#afb42b,color:#000000
```

##### Waveform Generation Modes

- The waveform generation mode is controlled by the WGMn2, WGMn1
  , and WGMn0 bits in the TCCRnA and TCCRnB registers
- The waveform generation mode determines how the timer counts and when it overflows
- There are several waveform generation modes available
  - Normal Mode
  - CTC (Clear Timer on Compare Match) Mode
  - Fast PWM Mode
  - Phase Correct PWM Mode
- PWM: Pulse Width Modulation
  - PWM is a technique used to generate analog-like signals using digital outputs
  - The frequency of the signal is controlled by changing the period of the pulse

```mermaid
flowchart TB
    subgraph "Waveform Generation Modes"
        WGM["WGMn2/1/0 Bits<br/><i>Waveform Mode</i>"]
    end

    subgraph "Normal Mode"
        NM["Normal Mode<br/><i>Overflow at MAX</i>"]
    end

    subgraph "CTC Mode"
        CTC["CTC Mode<br/><i>Clear on Compare Match</i>"]
    end

    subgraph "Fast PWM Mode"
        FPM["Fast PWM Mode<br/><i>Non-inverting</i>"]
    end

    subgraph "Phase Correct PWM Mode"
        PCP["Phase Correct PWM Mode<br/><i>Symmetrical</i>"]
    end

    WGM --> |"Control"| NM
    WGM --> |"Control"| CTC
    WGM --> |"Control"| FPM
    WGM --> |"Control"| PCP

    style WGM fill:#ffcdd2,stroke:#c62828,color:#000000
    style NM fill:#bbdefb,stroke:#1976d2,color:#000000
    style CTC fill:#c8e6c9,stroke:#388e3c,color:#000000
    style FPM fill:#dcedc8,stroke:#689f38,color:#000000
    style PCP fill:#f0f4c3,stroke:#afb42b,color:#000000
```

- Normal Mode
  - The timer counts from 0 to 255 (8-bit) or 65,535 (16-bit) and then overflows
  - The timer overflows when it reaches the maximum count value
  - The timer is reset to 0 after overflowing
- CTC Mode
  - The timer counts from 0 to a comparison value and then resets
  - The timer resets when it reaches the comparison value
  - The timer is reset to 0 after resetting
    <img src="https://wolles-elektronikkiste.de/wp-content/uploads/2020/12/F_PWM_vs_PandF_correct_Grafic-1024x578.png" align="right" width="400px" alt="Fast PWM vs Phase Correct PWM" />
- Fast PWM Mode
  - The timer counts from 0 to 255 (8-bit) or 65,535 (16-bit) and then overflows
  - The timer overflows when it reaches the maximum count value
  - The timer is reset to 0 after overflowing
  - The output pin is set high when the timer is less than the comparison value
  - The output pin is set low when the timer is greater than the comparison value
- Phase Correct PWM Mode
  - The timer counts up to the comparison value and then counts down to 0
  - The timer counts up when the timer is less than the comparison value
  - The timer counts down when the timer is greater than the comparison value
  - The output pin is set high when the timer is less than the comparison value
  - The output pin is set low when the timer is greater than the comparison value

### Logic Gates

- [Logic Gate](https://en.wikipedia.org/wiki/Logic_gate)
- [Logic Gate - Simulator](https://logic.ly/demo)

#### Flip-Flop

- [Flip-Flop - Wiki](<https://en.wikipedia.org/wiki/Flip-flop_(electronics)>)
- [Flip-Flop - Detailled - T,D,JK](https://www.electronicsforu.com/technology-trends/learn-electronics/flip-flop-rs-jk-t-d)
- JK Flip-Flop
  <img src="https://www.electronics-tutorials.ws/wp-content/uploads/2018/05/sequential-seq13.gif" align="right" width="300px" />

  -
  - Truth table:
    | J | K | Q(t) | Q(t+1) | Comment |
    |---|---|------|--------| ------- |
    | 0 | 0 | Q | Q | No change |
    | 0 | 1 | Q | 0 | Reset |
    | 1 | 0 | Q | 1 | Set |
    | 1 | 1 | Q | !Q | Toggle |

- T Flip-Flop
  <img src="https://www.electronicsforu.com/wp-contents/uploads/2019/04/t-flip-flop-1-500x266.jpg" align="right" width="300px" alt="t flip flop"/>

  - Truth table:
    | T | Q(t) | Q(t+1) | Comment |
    |---|------|--------| ------- |
    | 0 | Q | Q | No change |
    | 1 | Q | !Q | Toggle |

- D Flip-Flop
  <img src="https://www.electronicsforu.com/wp-contents/uploads/2017/08/d-flip-flop.png" align="right" width="300px" alt="d flip flop" />
  - Truth table:
    | D | Q(t) | Q(t+1) | Comment |
    |---|------|--------| ------- |
    | 0 | Q | 0 | Reset |
    | 1 | Q | 1 | Set |

## Bitwise Operations

[BitWise op in embedded programming](https://binaryupdates.com/bitwise-operations-in-embedded-programming/)

| Operation   | Symbol | Purpose                | Description                     | Example             |
| ----------- | ------ | ---------------------- | ------------------------------- | ------------------- |
| AND         | &      | Compare bits           | returns 1 if both bits are 1    | 1010 & 1100 = 1000  |
| OR          | \|     | Set bits               | returns 1 if any bit is 1       | 1010 \| 1100 = 1110 |
| XOR         | ^      | Toggle bits            | returns 1 if bits are different | 1010 ^ 1100 = 0110  |
| NOT         | ~      | Invert bits            | returns 1 if bit is 0           | ~1010 = 0101        |
| Left Shift  | <<     | Multiply by power of 2 | moves bits to the left          | 1010 << 1 = 10100   |
| Right Shift | >>     | Divide by power of 2   | moves bits to the right         | 1010 >> 1 = 0101    |

### Bit manipulation

<table>
  <thead>
    <tr>
      <th>Pattern</th>
      <th>Description</th>
      <th>Example</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>1 << n</code></td>
      <td>Mask Set bit n</td>
      <td>
      
```c
n = 3
1 << 3 = 00001000
```
</td>
    </tr>
    <tr>
      <td><code>~(1 << n)</code></td>
      <td>Mask Clear bit n</td>
      <td>

```c
n = 3
~(1 << 3) = 11110111
```

</td>
    </tr>
    <tr>
      <td><code>x & (1 << n)</code></td>
      <td>Check bit n</td>
      <td>

```c
x = 1010
n = 3
x & (1 << 3)
1010 & 1000 = 1000
```

</td>
    </tr>
    <tr>
      <td><code>x | (1 << n)</code></td>
      <td>Set bit n</td>
      <td>

```c
x = 1010
n = 2
x | (1 << 2)
1010 | 0100 = 1110
```

</td>
    </tr>
    <tr>
      <td><code>x & ~(1 << n)</code></td>
      <td>Clear bit n</td>
      <td>

```c
x = 1010
n = 1
x & ~(1 << 1)
1010 & 1101 = 1000
```

</td>
    </tr>
    <tr>
      <td><code>x ^ (1 << n)</code></td>
      <td>Toggle bit n</td>
      <td>

```c
x = 1010
n = 1
x ^ (1 << 1)
1010 ^ 0010 = 1000
```

</td>
    </tr>
    <tr>
      <td><code>x & ((1 << n) - 1)</code></td>
      <td>Clear bits above n</td>
      <td>

```c
x = 1110
n = 2
x & ((1 << 2) - 1)
1110 & (0100 - 1) = 1110 & 0011 = 0010
```

</td>
    </tr>
    <tr>
      <td><code>x & ~((1 << n) - 1)</code></td>
      <td>Clear bits below n</td>
      <td><code>x & ~((1 << 3) - 1)</code></td>
    </tr>
    <tr>
      <td><code>x & ((1 << (n + 1)) - 1)</code></td>
      <td>Clear bits above and including n</td>
      <td><code>x & ((1 << (3 + 1)) - 1)</code></td>
    </tr>
    <tr>
      <td><code>x & ~((1 << (n + 1)) - 1)</code></td>
      <td>Clear bits below and including n</td>
      <td><code>x & ~((1 << (3 + 1)) - 1)</code></td>
    </tr>
  </tbody>
</table>

## Modules

### Module 00

- [Info - AVR Programming](https://www.instructables.com/AVR-Programming-with-Arduino-AVRdude-and-AVR-gcc/)

- Find the USB port

  ```bash
  # List all USB devices
  lsusb

  # List all USB serial devices
  ls /dev/ttyUSB*

  # Show detailed device information
  dmesg | grep tty
  ```

- Include the necessary libraries `<avr/io.h>`
- PB0 is the LED pin here (same search for other pins)

  - ICP1/CLKO/PCINT0 – Port B, Bit 0
  - ICP1: Input Capture Pin. The PB0 pin can act as an Input Capture Pin for Timer/Counter1.
  - CLKO: Divided System Clock. The divided system clock can be output on the PB0 pin. The divided system clock will be output if the CKOUT Fuse is programmed, regardless of the PORTB0 and DDB0 settings. It will also be output during reset.
  - PCINT0: Pin Change Interrupt source 0. The PB0 pin can serve as an external interrupt
    source.
  - 📑 : Atmel ATmega328/P [DATASHEET] Atmel-42735B-ATmega328/P_Datasheet_Complete-11/2016 page 106)

### Module 01

Infos about frequency and time:

- `1Hz` = 1s = 1000ms
  - `1kHz` = 1,000Hz = 1ms
  - `1MHz` = 1,000,000Hz = 1µs
- `1 tick` = One cycle of the system clock
  - For a 16MHz Clock = 16,000,000 ticks per second
  - 1 tick = 1 / 16,000,000 seconds = 0.0625µs = 62.5ns
  - 1 tick = around 1-2 instructions
- **Clock** counter
  - Different counters have different maximum values
    - 8-bit counter = 256 ticks
    - 16-bit counter = 65,536 ticks
  - So using prescaling is important to avoid overflow
- Prescaling: Allow to skip a certain number of ticks
  - **Prescaler** values: `1, 8, 64, 256, 1024` (AVR Microcontrollers)
  - Example:
    - 16MHz / 1 = 16,000,000Hz
    - 16MHz / 8 = 2,000,000Hz
    - 16MHz / 64 = 250,000Hz
    - 16MHz / 256 = 62,500Hz
    - 16MHz / 1024 = 15,625Hz

Delay in code :

- [Microcontrollers - Timers Counters - Instructables](https://www.instructables.com/Beginning-Microcontrollers-Part-11-Timers-Counters/)
- [Builtin AVR Functions - GNU GCC Docs](https://gcc.gnu.org/onlinedocs/gcc/AVR-Built-in-Functions.html)
- `__builtin_avr_delay_cycles`

  - `void __builtin_avr_delay_cycles (uint32_t ticks)`
  - Delays the specified number of CPU cycles.
  - The delay parameter must be a compile-time constant.

Timer registers:

- [AVR - Timer programming - ExploreEmbedded](https://exploreembedded.com/wiki/AVR_Timer_programming)

<img src="./Media/Timer/Timer1 - OC1A - LED D2 - 1Hz.png" alt="Ex01 - 1Hz - LED D2 (OC1A)" />

- LED at 1Hz = 1s

  - $F_{timer} = \frac{F_{CPU}}{Prescaler}$
  - $F_{timer} = \frac{16\ \text{MHz}}{1024} = 15.625\ \text{KHz}$
  - $T_{tick} = \frac{1}{15.625\ \text{KHz}} = 64\ \mu s$
  - $Cycles/Count = \frac{Wait\ Time}{T_{tick}} = \frac{1\ s}{64\ \mu s} = 15625$

- Duty Cycle

  - Pulse Width(PW) to Total period (T) of the Waveform/Signal
    - Generally expressed in percentage
    - Represents the amount of time the signal is high(1) compared to the total time
  - $Duty\ Cycle = \frac{PW}{T} $
  - $Duty\ Cycle = \frac{ON\ Time}{Total\ Time} = \frac{ON\ Time}{ON\ Time + OFF\ Time}$
    - $Duty\ Cycle = \frac{1\ s}{1\ s + 1\ s} = 50\%$

- $F_{desired} = \frac{F_{CPU}}{Prescaler \times (1 + TOP)}$
  - $TOP = \frac{F_{CPU}}{F_{desired} \times Prescaler} - 1$
  - $TOP = \frac{16\ \text{MHz}}{1\ \text{Hz} \times 1024} - 1 = 15624$
