Ruzzle Cheater
==============

Intro
-----

At work they challenged me at Ruzzle ( http://www.ruzzle-game.com/ ), a wordgame on mobile. I tried playing it, but I find wordgames not that interesting, and they are way to good to beat. It would at least take weeks to beat them...

So I took another approach, let's hack that game!

Watch it running here: http://www.youtube.com/watch?v=atuG0AvA9tg

(It's hacky, I'm not responsible for anything!)

WARNINGS BEFORE YOU START:
--------------------------

*you might need to change the sendevents according to your phone's specs. (run "adb shell getevents" to see what's going on.)*

How it works:
-------------

* This is for android
* I'm using adb to send the sendevent commands (adb shell actually)
* You need to give in all letters on one line
* You'll need a dictionary (call it dic.txt and put it in the same directory)

Compilation instructions
------------------------

This is so simple it doesn't need a make file, just do:

        g++ ruzzle_cheater.cpp -o ruzzle_cheater

How to use it
-------------

* Connect your Android phone to your PC. (Enable development mode on your phone)
* Startup the program
* Start a ruzzle game
* Give in all characters at the console
* hit enter
* watch the magic