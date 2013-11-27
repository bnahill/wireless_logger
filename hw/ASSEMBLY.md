Hardware Assembly
=================
This document refers to rev2c. Rev2a and b are no longer supported and no hardware of those revisions should be assembled.

This document does not teach any skills involved. If you don't have experience with hot air reworking and soldering, you will have a very uncomfortable time and are extremely likely to break things. A lifted pad under any component can mean the entire board and many of the components on it are wasted, so please practice on something else first. THIS IS NOT AN EASY BOARD TO ASSEMBLE.

Assembly of the Strike Force rev2c board requires a number of tools:
* Soldering iron
* Hot-air rework station (prefered) or oven
* Vise
* Fine tweezers
* A solder paste in a syringe
* Solder wire of your choice, preferably thin (0.08cm will do, smaller preferable)
* Some flux, preferably in pen form
* Kapton (or other high-temperature) tape (maybe not required)
* Microscope (optional)

For assembly by hand, I recommend assembling the board in stages, defined by both regions and functionality, to reduce the search space if errors occur.

First, I recommend to assemble the power supply. Add all components in the region, including the USB port. This is to ensure that the rest of the components will receive a safe voltage when they are populated. Verify the 4.2V and ~3V supplies with the USB cable plugged in.

Second, mount the processor. This one is finicky and so I recommend adding all of the nearby passive components on the back before soldering on the processor. This way, you won't end up reflowing any of the processor while adding them. Mount both the 32kHz and 25MHz crystals along with the processor for the exact same reason. Once that's done, add the SWD connector (using the iron) and verify that the board and crystals work.

Next, you are free to add whichever sensors and radio units you please. I advise that the last components be, in sequence, the flash memory, the supercapacitor, and the OLED display. These are each sensitive to falling off the board when reflowed and are at risk of such an accident.

All components are sensitive to heat and care should be taken to minimize the time and temperature while soldering. However, there are a few components that are particularly sensitive to this:
* The microphone must not have hot air blown into its port hole. Just don't do it. Tape over it if you are soldering near it from that side.
* The pressure sensor is sensitive to hot air on the top hole. I recommend covering the device in Kapton tape and trimming the edges before soldering.
Everything else has survived a fair bit of abuse in my experience, but you should still be careful...
