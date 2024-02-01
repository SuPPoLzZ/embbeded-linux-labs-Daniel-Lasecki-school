# Using interrupts with GPIO
## Part 1 – playing with the  event handler 

So far we have used GPIO outputs (for driving leds) and inputs (for reading buttons). Specifically, reading a GPIO input using the method introduced in libgpiod-input-example, gpiod_line_get_value(), has a severe shortcoming: The input needs to be actively read aka polled. If we need to design a device that should be in a low power state (preserving battery), it would be problematic if we need to wake up every now and then, just to see if the state of GPIO pin has changed.  

Furthermore, for polling a pin, there needs to be an endless loop with some delay (sleep) involved. The length of the delay defines the minimum response time to the change of the pin state. If you need to have fast response time, you need to keep the polling interval very short. Naturally, you can use timers as well to trigger the polling, which takes place in the timer interrupt handler. Still, running the timer at the highs speed consumes resources and the same response time issue stays. 

A much more effective way is to use external GPIO interrupts. In lab3 you have already downloaded the example code (from here: [https://github.com/starnight/libgpiod-example](https://github.com/starnight/libgpiod-example)) for using GPIO events.

You can use lab3 as a basis (but create a new project, lab5), get main.c from libgpiod-example/libgpiod-event to your src-directory under lab5 and run it terminal in raspi. You can still build/launch it from debugger, but then stop it, ssh to raspi and execute lab5 binary there.  

To set up the input-pin, see this: [Configuring the input pin to have internal pull-up](/lab3/configuring-gpio-pin-pullup.md)   

Observe the behavioural.  

Let's analyze the code a little bit: 

The example code sets GPIO line 25 to be sensitive to rising edge events.
```
ret = gpiod_line_request_rising_edge_events(line, CONSUMER); 
```
Then it goes to a loop (length 20), where it waits for events until a timeout (1 sec) occurs (ret <0 ) or it gets an event (ret = 0). 
```
ret = gpiod_line_event_wait(line, &ts); 
```
Then, the event is being "read": 
```
ret = gpiod_line_event_read(line, &event); 
```
And finally the program goes to sleep for 1 second and loops over.

Change the code so that the  loop is endless and re-run it. Now press the button rapidly 10 times. 

Note how the program "buffers" the events – the 1 second sleep is applied for every loop round, but still once the program wakes from sleep, it memorizes the button presses that happened during the beauty nap and the following **gpiod_line_event_wait()** returns with an event detected. 

How many button presses it actually detected? 

Repeat this a few times (10 rapid presses, then see how many events were counted). 

Hook up an oscilloscope to the button pin, set the trigger to rising edge, repeat mode (not auto!), roughly 1.5V level. Set horizontal scale to 50us/div. 

Press the button several times, observe the waveform. 

The phenomenon you see is called **switch bouncing**, which causes multiple fast rising/falling edges to the signal.  

#### Switch bouncing

See this video:  [Debounce a Switch](https://youtu.be/e1-kc04jSE4)

The video explains a hardware-method to filter out (=debounce) those multiple edges. However, it is possible to do debouncing also with software. This will be addressed in part 3 below. 

Before jumping into that, there is another, bigger problem to solve: The function call gpiod_line_event_wait() is **blocking**. We will handle this problem first in part 2 below.  

## Part 2: Creating a separate thread for handling the gpio events 

Above we discussed that gpiod_line_event_wait()  is blocking. This means that the main program execution will stay in the wait loop (that is, the execution of the next code lines is blocked), until an event or timeout occurs.   

First, build a hardware having a push button and leds (like in the figure below). The pins for the button and leds are:  


| **Signal** | **GPIO** |
| ---- | ---- |
| BUTTON_PIN | 22 |
| RED_PIN | 23 |
| YELLOW_PIN | 24 |
| GREEN_PIN | 25 |


![Example circuit](Images/example_circuit.png)

Add  event_test_without_threads.c to your project and compile it. It should work out of the box, just make sure that you don't have several main()-functions in your active source code files. Observe the blocking nature of the code: The led changes state only after a time out or a button event has occurred. 

In the code the timeout is 5 seconds, where as the "main delay" on line is 200 ms. Now, in case there are no events, the main loop executes once in 5.2 seconds (timeout + 200 ms). On the other if there is an event, the delay will the time until the event arrives  + 200 ms.  You can think that the main loop has two tasks, which are executed sequentially, one after another. 

Tip: If lines 87-89 seem odd, think variable counter as a binary counter, you can "extract" individual bits of the counter and use them to to drive the leds. This method utilizes *bitwise operation*s. 

The solution to blocking problem is to create a separate *thread* – that is, a piece of code that runs in parallel to our main loop – responsible for handling the gpio events. The main loop has still responsibility to blink the LEDs.  As we are running Linux, we can utilise POSIX threads, (pthreads) for that purpose. 

Have a look at the basics of POSIX threads here: [https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html](https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html) 

Next, have a look at the next sample code: event_test_with_threads.c 

In order to compile it, you need to link also the pthread-library, so add the following line to CmakeLists.txt 

```
target_link_libraries(lab5 pthread) 
```

Observe the behaviour of the code carefully.  

Note, in order to see the output of printf()-functions, you need to launch it in the terminal, not in the debugger. Or, you can print to file instead and observe the contents of that file in RasPi (refer to lab2 how to do it). 

## Part 3: Handling the switch bouncing 

Have a look at the next sample code: event_test_with_threads_debounce.c 

Add it to your project and compile.  

This is a skeleton, which you can modify so that multiple events that occur during a certain time window are filtered out. Use an oscilloscope to detect  what would be a suitable "dead" time for the debouncer. 

Ideas for implementing the debouncer can be found from here: 

[https://raspberrypi.stackexchange.com/questions/8544/gpio-interrupt-debounce](https://raspberrypi.stackexchange.com/questions/8544/gpio-interrupt-debounce)