# Ardubios
A C++ library for Arduino-based boards to define and control hardware LEDs, hardware keys and virtual timers.

## Introduction
This is a collection of C++ classes to control in a straightforward way simple peripherals such as:
* hardware LEDs, with automatic blinking modes;
* hardware keys with debouncing and long-presses detection;
* virtual timers.

## Basic concepts

### Ticks
A tick is a coarse time measure unit used to keep track of time intervals across calls to the API.
The class `ArdubiosTicks` keeps track of elapsed ticks.
A single instance of this class suffices to track time.
Since a tick is a sub-second time interval tracked with 8-bit counters,
API must be called with intervals less than 1/10th of a second to ensure a correct tracking of elapsed time.

### Initialising resources
Any resource may be initialised through the C++ constructor for the controlling class or calling the member function `define()`.
The first method is cleaner, the second may be useful when configuration data can't be provided at class initialisation's time.

### Controlling resources
Any hardware (physical) or software (virtual) resource is controlled through two member functions:
`getState()` and `process()`. Resources such as LEDs, which have programmable state,
have also member functions to set new state under application's control.

`process()` reads or writes new state regarding the resource, based on configuration data and time tracking.
This function is called inside the run loop to animate the resource or collect input from real world.
When this function returns `true`, the code reacts to changes calling `getState()` and parsing the new state.

The example included along the library demonstrates the correct usage of the API.

## Why Ardubios?
Each class is pretty independent from each other.
Each class is coded to require a very light memory footprint even on resource-limited hardware such as 8-bit MCUs.
Support is planned for event-based programming.

#### Why Ardubios, really?
It's my first toy project for Arduino, also an useful use-case to learn Github mechanics :-)
