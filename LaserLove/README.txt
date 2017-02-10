BulletHell project Readme
------------------------------------

Controls and useful tips:

Console Controls:
	END - puts cursor at the end of the text being typed
	HOME - puts cursor at the start of the text being typed.
	DELETE - removes first character to the right of the cursor
	BACKSPACE - removes first character to the left of the cursor.
	SHIFT - while held, allows for typing in capital letters, default is lower cased.
	Left Arrow - move cursor left
	Right arrow - move cursor right
	Up Arrow - scroll up through past console logs
	Down Arrow - scroll down to present console logs
	Enter - push typed command/text to Console.
Console Run Commands:
	"memory_debug" - toggle in game display of memory tracking information
	"printcallstack" - Prints out the call stack to the the Visual Studio Output window while program is still running
	"printMemoryByAllocationType" - Prints out memory allocated in bytes per available types in the visual studio output window (not that these types are only indicated by numeric value presently).
	"threadedSaveOutAllocationsToLogger" - Prints out current callstack to logger via a thread.
	"addWhiteList" - Adds a tag to the white list for memory logger
	"removeWhiteList" - Removes a tag, if found, from the white list for the memory logger.
	"toggleSpriteLayer" - toggles a layer on the SpriteRenderer on or off.
	"profilingprintlastframe" = prints the profiled nodes from last frame.
	"toggleprofiler" - toggles on or off profiling for the next frame

BulletHell Controls:
	W: Move up (ship forward)
	D: Move Right (ship right)
	S: Move Down (ship back)
	A: Move Left (ship left)
	SPACE: Shoot
	ESC: quit
	SPACE (on menu or lose screen): Start game

Project was branched from Protogame.
SD5 Assignments 1 and 2 were originally tested in the project MemoryLeakTest

------------------------------------------------
Additional Credits for Resources
---
(This section is specifically for giving credit to some of the sound and art creators that specifically requested it)
(All others can be located within the "Resource Library Credit.xlsx")
---
Author - What they created - What role it plays in the game
---
vladart - 361-0.png - Background Tile.
------------------------------------------------

-------------------------------------------------------------------------------------------------------
SD5 - Assignment 1 - Memory Leak Detection

	- Configurable:
		* Can set how memory is tracked by inserting "#define MEMORY_TRACKING [#]" anywhere in the project; replace "[#]" with a number between 0 and 2
		* 0 - NONE
		* 1 - Basic
		* 2 - Verbose
	- Analysis
		* Memory Window
			- When in basic or verbose mode "memory_debug" to toggle on the ingame display for memory allocation tracking.
			-Will render prior to console rendering, but after the game's renderring.
		* Memory Flush
			* can print out all the callstack information and allocations to the visual studio output window.
			* use "printcallstack" to perform a memory_flush type action
		* Memory Tagging
			* In basic and Verbose mode, memory can be allocated by a specific tag.
			* use the console command "printMemoryByAllocationType" to have the data memory allocated per each type in bites to be displayed in the visual studio output window.
		* 0 - NONE
			-> no special memory tracking will be done.
		* 1 - Basic
			-> Memory allocation count and bytes will be tracked as memory is allocated via new and delete
				# Current number of allocations are tracked
				# Current total bytes allocated is tracked.
				# Highwater of total bytes allocated is tracked
				# number of bytes allocated and freed over the last second is tracked (though requires an additional method be run so as to be accurate)
				# Average rate of change of memory can be tracked.
		* 2 - Verbose
			-> same features as basic, plus some.
			-> Tracks line number, function name, and file name of where memory was allocated but not unallocated.
			-> Also tracks the data lost per leak.
			-> File, function and line number are tracked and shown in the warnings.
			-> Warnings are scrollable when within the Visual Studio output window.
	- Debugging
		* 0 - NONE
			-> No special debugging
		* 1 - Basic
			-> Asserts on shutdown if number of allocations did not match the number of allocations on startup.
			-> Number of memory leaks is announced prior to the warning that says there is memory leaks.
		* 2 - Verbose
			-> Tracks all information that Basic does.
			-> Tracks line number, function name and file name of memory that was not unallocated at shutdown.
				* on shutdown, this information will be presented as a series of pop up warning messages. (as well as warning # out of total warning number)
					*Full callstack is printed out in warnings.
					*warnings are double clickable to reach the file/Line location
			-> Leaks are categorized by unique callstack signature, listed out with the total number of callstacks. As well as Callstack line number of total # of callstack lines.
			-> Leaks print out how many bytes were allocated for it.
	- No Leaks
		* On shutdown, there are no memory leaks!
-------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------
SD5 - Assignment 2 - Logger System
	- LogPrintf() exists
		* can accept a tag.
		* can have a level
	- Lines can be printed to a file
		* Logged lines have a time stamp
		* Logged lines will contain the tag, and level
			-> will be in order of tag, level, and then time stamp.
			-> message will be on line following those three things.
	- File IO operations occur via a dedicated thread.
	- LogPrintf should be safe for use with multiple threads
	- There is a thread-safe LogFlush
	- On shutdown a default file and a file with a time stamp will be generated in the Data/Logs/ folder with the callstack in it.
		*If there a memory leaks, it will print them out.
		* If the console is still alive, then it will also print the console lines out.
	- A file with the log's time stamp is generated alongside the "debug.log" file
	- Since Callstack is only supported in Verbose mode, the Logger will only be used in Verbose mode.
	- White Listing
		* use console command: "addwhitelist" to add a given string to the logger's list of white listed tags.
		* use console command: "removewhitelist" to remove a given string, if found, from the logger's list of white listed tags.
		* NOTE: everything is given the tag "general" and "console" for right now.
		* When the logger is called to print off the call stack, it will only accept in the files that meet the white list of tags.
			Thus this will filter out the data from the callstack.
	- 
	
-------------------------------------------------------------------------------------------------------

	
-------------------------------------------------------------------------------------------------------
SD5 - Assignment 3 - Sprite Game Renderer
	- SpriteRenderer
		* Can specify a screen clear color
		* manages all layers and creates them as needed.
		* All drawing for a scene is done within RenderFrame
		* can toggle layers on and off via code and console commands (use "toggleSpriteLayer" to toggle a layer on and off)
		* User can specify a virtual screen size
		* Can tell the Renderer your current pixel resolution
		* Generates Screen Bounds using virtual coordinates
		* Can specify a camera position.
	- SpriteDatabase
		* User can register new sprites by ID and texture
		* New sprite resources have sane default values
		* There is an EditSprite method that gives a non-const pointer to the registered SpriteResource of ID
		* There is a GetSprite method that gives a const pointer to the registered SpriteResource of ID
		* Multiple Resources can be loaded/updated from XML files.
		* Resources can be saved out to an XML file.
	- SpriteResource
		* Is not passable by value nor copy-able
	- Sprite
		* Can create a sprite from a given sprite resource ID
		* Sprites will self register themselves with the renderer on enable
		* Sprites will self unregister themselves with the renderer on disable/destruction.
		* Sprites are capable of changing layers at any time.
		* Contains transform data (position/scale/rotation)
		* User may Specify a material or uniform override that applies to just one sprite instance.
		* Sprites can be parented to other sprites

-------------------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------------------
SD5 - Assignment 4 - Game
	- In Game Controls:
		WASD - Movement, based on the ship's forward vector.
		Space - Shoot towards the ship's forwards direction.
	- Game States:
		* Main Menu - Background scrolls on, there is a title, and some basic control instructions for starting up the game
		* Game - Control a ship using the above controls, and shoot of an infinite wave of enemies that shoot different kinds of bullets.
		* Lose - Upon death, the controls for quitting or restarting the game appear on the screen, no more enemies spawn, but the ones currently on screen will continue to fly on past.
	- Animated Sprites
		* Everything inherets from animated sprites at some level, but the bullets are the only ones that actually display this functionallity
	- Bullets:
		* Data driven
		* 8 types of bullet configurations
		* defaultBullet just goes forward
		* ExplodingBullet explodes on impact
		* spiralExplodingBullet has a timer before it explodes, but it will explode into more bullets that start to curve after impact.
			-> Note, death timer for this one is kept at 4 seconds to prevent game from being unplayable due to lag.
		* spiralExplodingBullet2 - same as spiralExplodingBullet, but spirals the opposite direction after explosion.
		* defaultBullet1 - same as defaultBullet, but customized to shoot two of the same bullets from off to the left and right
		* defaultBullet2 - same as defaultBullet but customized to shoot one bullet off further towards the front of the ship it is assigned to.
		* defaultBullet3 - same as defaultBullet but curves left
		* defaultBullet4 - same as defaultBullet but curves right
	- Enemy Ships
		* Data driven
		* 3 types of enemy ships
		* enemyShip1 is a big ship that shoots regular bullets
		* enemyShip2 is a small ship that shoots explosive bullets
		* enemyShip3 is a slow big ship that shoots spiralexplosion bullets
		* enemyShip4 same as ship1, but shoots defaultBullet3 and defaultBullet4
	- Enemy Ship Formations/Configurations
		* Data driven
		* 9 enemy formations
		* 1 - enemyShip1
		* 2 - enemyShip2
		* 3 - enemyShip3
		* 4 - 2 enemyShip2's and 1 enemyShip1
		* 5 - 5 enemyShip2's
		* 6 - 2 enemyShip3's
		* 7 - 3 enemyShip3's
		* 8 - 2 enemyShip1's and 1 enemyShip3
		* 9 - 2 enemyShip1's, 2 enemyShip2's, and 1 enemyShip3
		* 10 - 2 enemyShip4's and 1 enemyShip1
		* 11 - 1 enemyShip4
	- The bullets run on sprite animation sequences, also all GameEntities are AnimatedSprites, but the ships generally don't use animations.
-------------------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------------------
SD5 - Assignment 5 - Profiler
	- General
		* GetCurrentPerformanceCount in the Performance class returns the result of QueryPerformanceCounter
		* PerformanceCountToSeconds converts high resolution timing to seconds (aka, uses what was gotten in GetCurrentPerformanceCount and converts it to seconds)
		* There is a macro called "PROFILE_LOG_SECTION" that takes an int for an id, and uses that to time a section. The id is made into the node's tag.
		* All Profiling can be disabled via a buildconfig define
		* All profiling can be enabled/disabled at runtime
		* There is a console command that toggles profiling on and off "toggleprofiler"
		* void ProfilerSystemStartup sets up all subsequent profiling functions
		* void ProfilerSystemShutdown stops the profiler.
	- Main Thread Profiling
		* There is a Custom Fast Allocator; aka an ObjectPool, for the Profiler
		* ProfileFrameMark exists, and creates a new frame.
		* ProfileFrameMark asserts that there are presently no active samples on the stack.
		* There is a method for pushing samples onto the stack via Profiler::Push(tag);
		* There is a method for popping samples off the stack via Profiler::Pop();
		* ProfilerPopSample, although is not set to assert_or_die, ensures that the Current Sample exists before it does any popping action; if equals null, then it returns early.
		* There is a method for getting the last Frame called ProfilerGetLastFrame();
		* (Console command: "ProfilerPrintLastFrame")There is a console command that prints all profiled times, in order of the biggest amount of time used, to the Console window.
		* At the same time, one can use the same command, and add a space, plus a file location, and it will also save those times out to a text file. Save Files will appear in the Run_Win32 folder.
		* 
		* Reports are sortable by time consumed as percent of frame time.
	- Visualization
		* 
		* 
		* 
		* 
		* Frame History is configurable via PROFILER_MAX_TRACKED_FRAMES, which if it is not equal to 0, profiling will only keep track of the number of frames specified by that variable.
		* 
	- Lighter Weight & Thread Safe Profiling

-------------------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------------------
SD5 - Assignment 6 - Simple Particle System
	*Note: Presently there is a particle Emitter definition, System Definition, and System set up, but there is presently no rendering capabilities with this.
	- Registration
		* User is able to register a new particle system definition by name, via FindOrCreateParticleSystemDefinition(tag)
		* System definitions are able to hold onto and have Emitter Definitions.
		* 
		* For each individual ParticleEmitter, it is possible to edit the individual emitter definitions.
		* 
		* 
		* Might have support for a non-looping particle system.
		* It is possible to load in ParticleSystemDefinitions from XML
			-> Root XMLNode for file = ParticleSystemDefinitions
			-> ParticleSystemDefinition XMLNode name = ParticleSystemDefinition2D
			-> Children of ParticleSystemDefinition XMLNode, for particle Emitter definitions, must be name ParticleEmitterDefinition2D
			-> Attributes for ParticleSystemDefinition:
				Only takes "name", and this MUST be set to something
			-> Attributes for ParticleEmitterDefinition:
				name - for the user's convenience, does not get used in registration or anything.
				StartSpawnCount - the number of particles spawned at the Emitter's creation/reset
				LocalPosition - the position relative to the ParticleSystem's center
				particleSpawnRate - how many seconds pass before a particle should be spawned.
				lifeTime - how long should the particles live
				tint - what is the default color of the particles; not useful when color transitioning is enabled.
				scale - what is the relative scale of the particles
				radius - what is the spawn radius in which to randomly spawn a particle within from the Emitter's center
				secondsBeforeStopSpawningParticles - how long should the Emitter live before it stops spawning particles (set to a a number < 0 if don't wish to use)
				secondsbeforeDestroyEmitterRegardlessOfParticles - how long before the emitter just automatically destroys itself regardless of the state of it's particles? (set to a number less than 0 if don't wish to use)
				enableColorTransition - set to "t" or "true" in order to enable color transitioning.
				startColor - used as the start color when color transitioning is enabled; goes based off total age versus lifetime of the particle.
				endColor - used as the end color when color transitioning is enabled; goes based off total age versus lifetime of the particle
	- Usage
		* It is possible to create a looping particle system via CreateParticleSystem(tag, layer, position); (to note, need to have an already registered ParticleSystemDefinition, to use this)
		* Particle systems register themselves with the appropriate layer upon creation and on layer change.
		* Particle Systems update themselves inside of SpriteRenderer::Update
		* Particles are spawned each frame based on spawn rate
		* Particles move and destroy themselves over the course of their life.
			-> Can define whether or not to have the particles color fade out over time and also the start and end colors.
		* Particle System has a call to be Rendered from within SpriteRenderer::Render
		*
		* User can destroy a particle system by calling ParticleSystemDestroy(system)
		* Particle System is freed when all of it's held emitters no longer spawn particles nor have any particles.
		* Assuming that I actually do have the previously mentioned non-looping particle system set up, then technically should be able to 
create a one off particle system just via the same CreateParticleSystem; just have to set up the ParticleSystemDefinition 
correctly.
		* Can See how many particles are currently active via a console command "PrintTotalNumberOfActiveParticles2D"
			-> Considering the functionallity is there for the console, it would be very easy to set up a live count of active particles.
-------------------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------------------
SD5 - Assignment 7 - Instrumented Profiler & Display
	- On System Startup:
		* Can specify the number of categories for the system.
		* Can specify the number of worker threads to spawn.
		* Can use a negative number to specify the number of worker threads, which leads to the JobSystem attempting to allocate Number of cores + that negative number
		* Handing the system 0 cause it try to allocate per number of cores.
	- On System Shutdown:
		* all threads are stopped are stopped, and try to finish all remaining jobs.
	- Worker Threads:
		* System will always spawn at least one worker thread.
		* System supports GENERIC and GENERiC_SLOW job categories (JobSpeedType)
		* Only have the ability to consume GENERIC and GENERIC_SLOW jobs.
		* If multiple job threads, the job threads should be able to alternate which type of job they prioritize
	- JobConsumer
		* Can creat a job consumer object that specifies JobSpeedType, and also a number of categories
		* Has a method to consume all available Jobs
		* Has a method to consume jobs for a given time frame or until all available jobs are gone.
		* has the ability to consume a single job.
	- Job
		* Can Pass arbitrary data to a job.
	- General
		* Bounds Testing is currently done through the job system
			->Old Scores:
				Node 3 took 0.00004374 seconds
				Node 3 took 0.00004768 seconds
				Node 3 took 0.00004689 seconds
				Node 3 took 0.00005281 seconds
				Node 3 took 0.00004571 seconds
				Node 3 took 0.00004926 seconds
			->Scores after job system:
				Node 3 took 0.00001497 seconds
				Node 3 took 0.00001497 seconds
				Node 3 took 0.00001537 seconds
				Node 3 took 0.00001734 seconds
				Node 3 took 0.00002010 seconds
				Node 3 took 0.00002325 seconds

-------------------------------------------------------------------------------------------------------
