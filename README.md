# Electronic Parking Disc

An electronic [parking disc](https://en.wikipedia.org/wiki/Disc_parking) that automatically displays the time when a car was parked. Implemented using a solar-powered Arduino equipped with an accelerometer and e-ink screen.

The set-up process has been implemented in an MVC-like pattern. The state (model) is held in a struct and updated by controller functions as it receives user input. Rendering is done by a render function based on the values of the state struct.

NOTE: Only the set-up process is included in this project.

Developed for the product development course AAN-C2009: Designing an Electronic Device for Business and Production (Aalto University). 
