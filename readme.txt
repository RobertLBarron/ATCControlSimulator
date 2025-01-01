README

-------------------------------------|
For: A6 - ATC Simulation Project     |
By: Robert LaChausse, December 2024  |
-------------------------------------|

Important things to note:

1.) my file path is structured like so

string fileString = home_dir + "/Downloads/a_term_proj/atc_data/atc_instructions.txt";

Note that I have "/Downloads" in here. this is because I wanted to test the home directorys ability to navigate to ANY folder from home. For most users,
deleting that and moving the a_term_proj directly to home will allow for quick access. 

HOWEVER, commented out below that line of code is another line of code that (should) allow the loadInstructions function to load the atc_instructions.txt
straight from the QResource file, potentially removing any need to directory nonsense. I did this to prove I understood both methods, and provide a potential failsafe.
Use whichever one you deem fit/works for you.

2.) my ATC class holds more merit than the updater or airbus. This is purely personal preference, but it was cleaner and easier to faciliate moving between
2 objects most of the time while using updater (or potential actual_updater) and airbus to gain the relevant setters and getters for necessary data.

3.) my simulation has a BUFFER for the dial inputs. This means that as long as you are CLOSE to the desired input, it will accept it and allow you to continue.
This is because I wanted to keep the dials clean and specific, while preventing frustration from users for not being able to pinpoint the value within 30 seconds.

NOTE - the ATC frequency dial/value does NOT have a buffer. this is because there is a keypad which allows for near-instant detailed input, and giving it a buffer
would make the simulation a bit too easy and possibly not require the user to ever interact with the keypad widget that I spent too much time on.

The buffers are as follows:
you can be within a range of 3 from the precise heading. (e.g. if the heading needs to be 170, a 168 or 173 will work.)
you can be within a range of 299 from the precise altitude.
you can be within a range of 4 from the precise airspeed.

4.) I use a QResource for the audio files in this project. It was easier and cleaner than having to use a path. This resource should be included in the zip,
so the user should not have to worry about finding that data. If more audio files want to be added, they must be manually referenced in the ATC class method,
Atc::loadAudio();.

5.) the simulation does not start off the bat to give the player time to acclimate. If you wish to hear the first instruction earlier, playing "restart" will allow you to hear it before the
acclimation period is over (it will also restart the timer, as intended use)!

6.) the extra credit animations (the white numbers slowly matching the magenta ones) do cause lag if you spin them willy nilly. If you just press once every couple seconds, you shouldn't notice it.
Please comment out the relevant functions at the end of mainwindow.cpp and in actual_updater if you want to spin them around and not get any lag.
The animations are also a little wonky on the first dial adjustment, but work smoothly past it!

NOTE - the lag hardly affects the actual values the ATC and Airbus check in order to find out if the instruction is valid or not. It's purely visual/feelwise.
UPDATE (12/18/2024) - The dials now wait until the user has STOPPED moving the dial (for at least 1 second) to start the animation AND changing
the dial during the animation will not affect the current animation.

Final Notes:
Overall, the project is a bit messy and a bit overengineered, but works wonderfully and I'm proud to have accomplished it.

----------

Special thanks to my father, who is a licensed pilot. He helped explained the real lifetools and potential of this simulation, 
which made it more fun to work on.
